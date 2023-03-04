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
    this->open_camera->move(260, 385);

    
    /* connect signal with slot */

    QWidget::connect(this->open_camera, SIGNAL(released()), 
            this, SLOT(slots_open_camera()), Qt::AutoConnection);
}

SSI_Widget::~SSI_Widget() {
    delete this->open_camera;
    delete this->settings;
}

/* slot function */

void SSI_Widget::slots_open_camera() {
    qDebug() << "click button";
}
