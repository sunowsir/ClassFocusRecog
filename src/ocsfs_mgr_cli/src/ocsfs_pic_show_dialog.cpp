/*
	* File     : ocsfs_pic_show_dialog.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月28日 星期日 13时01分52秒
*/

#include "ocsfs_pic_show_dialog.h"

OCSFS_Pic_Show_Dialog::OCSFS_Pic_Show_Dialog(QWidget *parent) {
    this->parent = parent;

    this->layout = new QVBoxLayout(this);
    this->setLayout(this->layout);

    this->show_label = new QLabel(this);
    this->layout->addWidget(this->show_label);
}

OCSFS_Pic_Show_Dialog::~OCSFS_Pic_Show_Dialog() {
    delete this->show_label;
}

bool OCSFS_Pic_Show_Dialog::recv_show_image(const QImage& recv_show_image) {
    QPixmap pix = QPixmap::fromImage(recv_show_image);
    QPixmap dest=pix.scaled(this->show_label->size(),Qt::KeepAspectRatio);
    this->show_label->setPixmap(QPixmap(dest));
    return true;
}
