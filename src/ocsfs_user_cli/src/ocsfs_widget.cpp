/*
	* File     : ocsfs_widget.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月05日 星期日 01时18分08秒
*/

#include "ocsfs_widget.h"

OCSFS_Widget::OCSFS_Widget(QMainWindow *parent) 
    : QWidget(parent) {
    /* save config */
    this->settings = new QSettings(QCoreApplication::applicationDirPath() + 
        QString("/Config.ini"), QSettings::IniFormat );

    /* open camera button */
    this->open_camera = new QPushButton(this);
    this->open_camera->setText(tr("打开摄像头"));
    this->open_camera->move(300, 520);
    this->open_camera->resize(80, 30);

    /* close camera button */
    this->close_camera = new QPushButton(this);
    this->close_camera->setText(tr("关闭摄像头"));
    this->close_camera->move(450, 520);
    this->close_camera->resize(80, 30);

    /* camera list */
    this->cameras_list = nullptr;

    /* 当前摄像头 */
    this->selected_camera = new QCamera();

    /* 媒体连接器 */
    this->capture_session = new QMediaCaptureSession();
    
    /* 图片转换器 */
    this->image_capture = new QImageCapture();

    /* 摄像头画面预览窗口 */
    this->camera_view = new QVideoWidget(this);
    this->camera_view->move(1, 1);
    this->camera_view->resize(500, 450);
    this->camera_view->hide();

    /* 画面帧定制捕捉定时器 */
    this->capture_timer = new QTimer(this);
    this->capture_timer->setInterval(300);
    this->capture_timer->stop();
    
    /* connect signal with slot */

    QWidget::connect(this->open_camera, SIGNAL(released()), 
        this, SLOT(slots_open_camera()), Qt::AutoConnection);
    QWidget::connect(this->close_camera, SIGNAL(released()), 
        this, SLOT(slots_close_camera()), Qt::AutoConnection);
    QWidget::connect(this->image_capture, SIGNAL(imageCaptured(int, const QImage&)), 
        this, SLOT(slots_capture_camera_frame(int, const QImage&)), Qt::AutoConnection);
    QWidget::connect(this->capture_timer, SIGNAL(timeout()), 
        this, SLOT(slots_timer_out()), Qt::AutoConnection);

    /* camera list */
    this->camera_combobox = new QComboBox(this);
    this->camera_combobox->move(30, 520);
    this->camera_combobox->resize(190, 30);

    /* 获取摄像头设备列表信息 */
    this->camera_devices_refresh();

    QWidget::connect(this->camera_combobox, SIGNAL(currentTextChanged(const QString&)), 
        this, SLOT(slots_select_camera(const QString&)), Qt::AutoConnection);

    /* 学生签到按钮 */
    this->checkin_button = new QPushButton(this);
    this->checkin_button->setText(tr("签到"));
    this->checkin_button->move(320, 520);
    this->checkin_button->resize(80, 30);
    /* 初始设置默认不可点击，只有教师发起签到才能点击 */
    this->checkin_button->setEnabled(false);

    /* 绑定按钮点击信号 */
    QWidget::connect(this->checkin_button, SIGNAL(released()), 
        this, SLOT(slots_checkin_click()), Qt::AutoConnection);

    /* 学生确认点名按钮 */
    this->rollcall_button = new QPushButton(this);
    this->rollcall_button->setText(tr("应答点名"));
    this->rollcall_button->move(340, 520);
    this->rollcall_button->resize(80, 30);
    /* 初始设置默认不可点击，只有教师发起点名才能点击 */
    this->rollcall_button->setEnabled(false);

    /* 绑定按钮点击信号 */
    QWidget::connect(this->rollcall_button, SIGNAL(released()), 
        this, SLOT(slots_rollcall_click()), Qt::AutoConnection);

    return ;
}

OCSFS_Widget::~OCSFS_Widget() {
    delete this->open_camera;
    delete this->close_camera;
    delete this->settings;
    delete this->capture_session;
    delete this->image_capture;
    delete this->camera_view;
    delete this->capture_timer;
    if (nullptr != this->cameras_list)
        delete this->cameras_list;
    delete this->camera_combobox;
    delete this->checkin_button;
    delete this->rollcall_button;

    return ;
}

void OCSFS_Widget::camera_devices_refresh() {
    if (QMediaDevices::videoInputs().count() <= 0) {
        qDebug() << "[ERROR] no video inputs devices";
        QMessageBox::critical(this, tr("错误"), tr("找不到摄像头"));
        return ;
    }

    const QList<QCameraDevice> cameras  = QMediaDevices::videoInputs();
    this->cameras_list = new QList<QCameraDevice>(std::move(cameras));

    this->camera_combobox->clearEditText();

    bool select_flag = false;
    for (const QCameraDevice &device : *(this->cameras_list)) {
        this->camera_combobox->insertItem(this->camera_combobox->count(), 
            device.description());

        if (true == select_flag)
            continue;

        this->selected_camera->setCameraDevice(device);
        select_flag = true;
    }

    if (false == select_flag) {
        QMessageBox::critical(this, tr("错误"), tr("无摄像头"));
        return ;
    }

    /* 使用连接器连接摄像头与预览框 */
    this->capture_session->setCamera(this->selected_camera);
    this->capture_session->setVideoOutput(this->camera_view);

    /* 用this->image_capture 捕获帧 */
    this->capture_session->setImageCapture(this->image_capture);

    return ;
}

/* slot function */

void OCSFS_Widget::slots_open_camera() {
    this->camera_devices_refresh();
    if (nullptr == this->selected_camera) {
        QMessageBox::critical(this, tr("错误"), tr("未选择摄像头"));
        return ;
    }

    /* 显示摄像头的实时图像 */
    if (this->camera_view->isHidden())
        this->camera_view->show();

    /* 打开摄像头 */
    if (!this->selected_camera->isActive())
        this->selected_camera->start();

    /* 开启定时器 */
    if (!this->capture_timer->isActive())
        this->capture_timer->start();

    return ;
}

void OCSFS_Widget::slots_close_camera() {
    /* 关闭定时器 */
    if (this->capture_timer->isActive())
        this->capture_timer->stop();

    /* 关闭摄像头 */
    if (this->selected_camera->isActive()) 
        this->selected_camera->stop();

    /* 隐藏预览框 */
    if (this->camera_view->isActiveWindow())
        this->camera_view->hide();

    return ;
}

void OCSFS_Widget::slots_select_camera(const QString& selected_name) {
    for (auto& device : *this->cameras_list) {
        if (selected_name != device.description())
            continue;
        this->selected_camera->setCameraDevice(device);
        this->capture_session->setCamera(this->selected_camera);
        break;
    }
    
    return ;
}

void OCSFS_Widget::slots_capture_camera_frame(int id, const QImage& frameImage) {
    this->send_image_to_server(frameImage);
}

void OCSFS_Widget::slots_timer_out() {
    this->image_capture->capture();
}

/* 学生点击了签到按钮后该做什么事 */
void OCSFS_Widget::slots_checkin_click() {
    this->checkin_button->setEnabled(false);
}

/* 学生点击了提问按钮后应该做什么事 */
void OCSFS_Widget::slots_rollcall_click() {
    this->rollcall_button->setEnabled(false);
}

/* 教师端发来签到请求 */
void OCSFS_Widget::have_mgr_check_in(const QString &src_client_id) {
    /* 设置按钮为可点击状态 */
    this->checkin_button->setEnabled(true);
    this->response_mgr_check_in();
}

/* 教师端发来点名请求 */
void OCSFS_Widget::have_mgr_roll_call(const QString &src_client_id) {
    /* 设置按钮为可点击状态 */
    this->rollcall_button->setEnabled(true);
    this->response_mgr_roll_call();
}

/* 教师端发来的警告 */
void OCSFS_Widget::have_mgr_warning(const QString &src_client_id) {
    QMessageBox::critical(this, tr("警告"), 
        tr("编号") + src_client_id + tr("教师发来警告，请您注意状态!"));
    this->response_mgr_warning();
}

