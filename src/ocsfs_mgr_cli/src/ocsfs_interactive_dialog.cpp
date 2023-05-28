/*
	* File     : ocsfs_interactive_dialog.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月28日 星期日 13时01分52秒
*/

#include "ocsfs_interactive_dialog.h"

OCSFS_Interactive_Dialog::OCSFS_Interactive_Dialog(QWidget *parent) {
    this->parent = parent;

    this->layout = new QVBoxLayout(this);
    this->setLayout(this->layout);

    this->checkin = new QPushButton(this);
    this->checkin->setText(tr("签到"));
    this->layout->addWidget(this->checkin);

    this->rollcall = new QPushButton(this);
    this->rollcall->setText(tr("抽取点名"));
    this->layout->addWidget(this->rollcall);
}

OCSFS_Interactive_Dialog::~OCSFS_Interactive_Dialog() {
    delete this->layout;
    delete this->checkin;
    delete this->rollcall;
}
