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
            QString("/Config.ini"), 
        QSettings::IniFormat );

    qDebug() << QCoreApplication::applicationDirPath() + QString("/Config.ini");

    this->parent = parent;

    /* open camera button */
    this->open_camera = new QPushButton(this);
    this->open_camera->setText(tr("打开摄像头"));
    this->open_camera->move(350, 520);
    this->open_camera->resize(80, 30);

    this->cameras_list = new QList<std::pair<QString, QCamera*>>;
    this->camera_name_list = new QStringList;

    this->selected_camera = nullptr;

    /* 获取摄像头列表 */
    this->get_camera_list();

    /* camera list */
    this->camera_list = new QComboBox(this);
    this->camera_list->move(50, 520);
    this->camera_list->resize(190, 30);
    this->camera_list->addItems(*this->camera_name_list);

    /* camera view */
    this->camera_view = new QVideoWidget(this);
    this->camera_view->move(1, 1);
    this->camera_view->resize(500, 450);
    this->camera_view->hide();

    this->captureSession = new QMediaCaptureSession();

    this->imageCapture = new QImageCapture();

    this->capture_timer = new QTimer(this);

    // this->ser = new SSI_Expression_Recognition(QCoreApplication::applicationDirPath() + QString("/SVM_DATA.xml"));
    
    /* connect signal with slot */

    QWidget::connect(this->open_camera, SIGNAL(released()), 
            this, SLOT(slots_open_camera()), Qt::AutoConnection);
    QWidget::connect(this->camera_list, SIGNAL(currentTextChanged(const QString&)), 
            this, SLOT(slots_select_camera(const QString&)), Qt::AutoConnection);
    QWidget::connect(this->imageCapture, SIGNAL(imageCaptured(int, const QImage&)), 
            this, SLOT(slots_capture_camera_frame(int, const QImage&)), Qt::AutoConnection);
    QWidget::connect(this->capture_timer, SIGNAL(timeout()), 
            this, SLOT(slots_timer_out()), Qt::AutoConnection);
}

SSI_Widget::~SSI_Widget() {
    delete this->open_camera;
    delete this->settings;
}

void SSI_Widget::get_camera_list() {
    if (QMediaDevices::videoInputs().count() <= 0) {
        qDebug() << "[ERROR] no video inputs devices";
        QMessageBox::critical(this, tr("错误"), tr("找不到摄像头"));
        return ;
    }

    const QList<QCameraDevice> cameras  = QMediaDevices::videoInputs();

    bool select_flag = false;
    for (const QCameraDevice &cameraDevice : cameras) {
        QCamera *camera = new QCamera(cameraDevice);

        this->cameras_list->append(std::pair(cameraDevice.description(), camera));
        this->camera_name_list->append(cameraDevice.description());

        this->selected_camera = camera;
        select_flag = true;
    }

    if (false == select_flag) 
        QMessageBox::critical(this, tr("错误"), tr("无摄像头"));

    return ;
}

/* slot function */

void SSI_Widget::slots_open_camera() {
    if (nullptr == this->selected_camera) {
        QMessageBox::critical(this, tr("错误"), tr("未选择摄像头"));
        return ;
    }


    this->captureSession->setCamera(this->selected_camera);
    this->captureSession->setVideoOutput(this->camera_view);

    /* 用this->imageCapture 捕获帧 */
    this->captureSession->setImageCapture(this->imageCapture);

    /* 显示摄像头的实时图像 */
    this->camera_view->show();

    /* 打开摄像头 */
    this->selected_camera->start();

    this->capture_timer->start(500);

    return ;
}

void SSI_Widget::slots_select_camera(const QString& selected_name) {
    for (const std::pair<QString, QCamera*> &camera : *this->cameras_list) {
        if (selected_name != camera.first)
            continue;
        this->selected_camera = camera.second;
        break;
    }
    
    return ;
}

void SSI_Widget::slots_capture_camera_frame(int id, const QImage& frameImage) {
#if 1
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

#if 0
    int face_type = 0;
    this->ser->recognize(frameImage, face_type);

    std::cout << face_type << ": ";
    switch(face_type) {
        /*
        case 170: {
            std::cout << "平静" << std::endl;
        } break;
        case 250: {
            std::cout << "开心" << std::endl;
        } break;
        case 300: {
            std::cout << "厌恶" << std::endl;
        } break;
        default: {
            std::cout << "unknow" << std::endl;
        }
        */
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
    this->imageCapture->capture();
}
