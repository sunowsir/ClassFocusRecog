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
    this->open_camera->move(350, 400);
    this->open_camera->resize(80, 30);

    this->cameras_list = new QList<std::pair<QString, QCamera*>>;
    this->camera_name_list = new QStringList;

    /* 获取摄像头列表 */
    this->get_camera_list();

    /* camera list */
    this->camera_list = new QComboBox(this);
    this->camera_list->move(50, 400);
    this->camera_list->resize(190, 30);
    this->camera_list->addItems(*this->camera_name_list);

    
    /* connect signal with slot */

    QWidget::connect(this->open_camera, SIGNAL(released()), 
            this, SLOT(slots_open_camera()), Qt::AutoConnection);
}

SSI_Widget::~SSI_Widget() {
    delete this->open_camera;
    delete this->settings;
}

void SSI_Widget::get_camera_list() {
    if (QMediaDevices::videoInputs().count() <= 0) {
        qDebug() << "[ERROR] no video inputs devices";
        return ;
    }

    const QList<QCameraDevice> cameras  = QMediaDevices::videoInputs();

    for (const QCameraDevice &cameraDevice : cameras) {
        QCamera *camera = new QCamera(cameraDevice);
        this->cameras_list->append(std::pair(cameraDevice.description(), camera));
        this->camera_name_list->append(cameraDevice.description());

        qDebug() << cameraDevice.description();
    }
}

/* slot function */

void SSI_Widget::slots_open_camera() {
    qDebug() << "click button";
}
