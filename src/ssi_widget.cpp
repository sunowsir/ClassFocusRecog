/*
	* File     : ssi_widget.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月05日 星期日 01时18分08秒
*/

#include "ssi_widget.h"

SSI_Widget::SSI_Widget(QMainWindow *parent) 
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

    /* 人脸识别器 */
    this->ser = new SSI_Expression_Recognition(QCoreApplication::applicationDirPath() + QString("/SVM_DATA.xml"));
    
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

    return ;
}

SSI_Widget::~SSI_Widget() {
    delete this->open_camera;
    delete this->close_camera;
    delete this->settings;
    delete this->capture_session;
    delete this->image_capture;
    delete this->camera_view;
    delete this->capture_timer;
    delete this->ser;
    delete this->cameras_list;
    delete this->camera_combobox;

    return ;
}

void SSI_Widget::camera_devices_refresh() {
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

void SSI_Widget::slots_open_camera() {
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

void SSI_Widget::slots_close_camera() {
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

void SSI_Widget::slots_select_camera(const QString& selected_name) {
    for (auto& device : *this->cameras_list) {
        if (selected_name != device.description())
            continue;
        this->selected_camera->setCameraDevice(device);
        this->capture_session->setCamera(this->selected_camera);
        break;
    }
    
    return ;
}

void SSI_Widget::slots_capture_camera_frame(int id, const QImage& frameImage) {
#if 0
    /* 使用图像训练器训练模型 */
    /* 设置有几种表情类型，以及每种类型的训练图片有多少 */
    SSI_Module_Trainer smt(3, 50);
    

    smt.load_train_data(QCoreApplication::applicationDirPath() + QString("/comm"), SSI_face_COMM);
    smt.load_train_data(QCoreApplication::applicationDirPath() + QString("/happy"), SSI_face_HAPPY);
    smt.load_train_data(QCoreApplication::applicationDirPath() + QString("/hade"), SSI_face_HADE);

    smt.train_module_2_xml();

    smt.train_module_test(QCoreApplication::applicationDirPath() + QString("/comm"), SSI_face_COMM);
    smt.train_module_test(QCoreApplication::applicationDirPath() + QString("/happy"), SSI_face_HAPPY);
    smt.train_module_test(QCoreApplication::applicationDirPath() + QString("/hade"), SSI_face_HADE);

    exit(0);
#endif

#if 1
    int face_type = 0;
    this->ser->recognize(frameImage, face_type);

    std::cout << face_type << ": ";
    switch(face_type) {
        case SSI_face_COMM: {
            std::cout << "平静" << std::endl;
        } break;
        case SSI_face_HAPPY: {
            std::cout << "开心" << std::endl;
        } break;
        case SSI_face_HADE: {
            std::cout << "厌恶" << std::endl;
        } break;
        default: {
            std::cout << "unknow" << std::endl;
        }
    }

#endif

    return ;
}

void SSI_Widget::slots_timer_out() {
    this->image_capture->capture();
}


