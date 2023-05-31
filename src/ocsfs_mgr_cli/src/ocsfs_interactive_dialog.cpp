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

    this->total_stu_num = 0;

    this->layout = new QVBoxLayout(this);
    this->setLayout(this->layout);

    this->checkin = new QPushButton(this);
    this->checkin->setText(QString(tr("签到") + ", 0/0"));
    this->layout->addWidget(this->checkin);

    this->rollcall = new QPushButton(this);
    this->rollcall->setText(tr("抽取点名"));
    this->layout->addWidget(this->rollcall);

    QWidget::connect(this->checkin, SIGNAL(released()), 
        this->parent, SLOT(slot_checkin_click()), Qt::AutoConnection);

    QWidget::connect(this->rollcall, SIGNAL(released()), 
        this->parent, SLOT(slot_rollcall_click()), Qt::AutoConnection);

    this->is_checkin = new QMap<QString, bool>();
}

OCSFS_Interactive_Dialog::~OCSFS_Interactive_Dialog() {
    delete this->layout;
    delete this->checkin;
    delete this->rollcall;
    delete this->is_checkin;
}

/* 有学生上线 */
void OCSFS_Interactive_Dialog::have_user_ready(QString &client_id) {
    this->is_checkin->insert(client_id, false);
    this->total_stu_num++;

    int already_checkin_num = 0;
    for (auto stu : *(this->is_checkin)) {
        if (stu) already_checkin_num++;
    }

    this->checkin->setText(QString(tr("签到") + ", " + 
        QString::number(already_checkin_num) + 
        "/" + QString::number(this->total_stu_num)));
}

/* 有学生签到 */
void OCSFS_Interactive_Dialog::have_user_check_in(QString &client_id) {
    this->is_checkin->insert(client_id, true);

    int already_checkin_num = 0;
    for (auto stu : *(this->is_checkin)) {
        if (stu) already_checkin_num++;
    }

    this->checkin->setText(QString(tr("签到") + ", " + 
        QString::number(already_checkin_num) + 
        "/" + QString::number(this->total_stu_num)));
}

void OCSFS_Interactive_Dialog::have_user_roll_call(QString &client_id) {
    this->rollcall->setText(QString(tr("抽取点名") + ", " + 
        client_id + "已收到点名"));

    // QMessageBox::critical(this, tr("信息"), 
    //     tr("学生: ") + client_id + tr("已确认点名"));
}

