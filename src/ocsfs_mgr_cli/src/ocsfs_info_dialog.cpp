/*
	* File     : ocsfs_info_dialog.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月28日 星期日 13时01分52秒
*/

#include "ocsfs_info_dialog.h"

OCSFS_Info_Dialog::OCSFS_Info_Dialog(QWidget *parent) {
    this->parent = parent;
    this->layout = new QVBoxLayout(this);
    this->setLayout(this->layout);

    this->stu_dialog_list = new QMap<QString, OCSFS_BarChartLabel*>();
}

OCSFS_Info_Dialog::~OCSFS_Info_Dialog() {
    delete this->stu_dialog_list;
    delete this->layout;
}

bool OCSFS_Info_Dialog::add_student_label(const QString &client_id) {
    if (this->stu_dialog_list->contains(client_id))
        return true;

    for (auto stu_info: *(this->stu_dialog_list)) {
        QString now_client_id;
        stu_info->get_client_id(now_client_id);
        if (now_client_id != QString("0000000000"))
            continue;
        stu_info->set_client_id(client_id);
        stu_info->setText(QString("学生: ") + client_id);
        return true;
    }

    OCSFS_BarChartLabel *label = new OCSFS_BarChartLabel(client_id);
    label->setText(QString("学生: ") + client_id);
    this->layout->addWidget(label);
    
    QWidget::connect(label, SIGNAL(signal_mouse_enter(const QString&)), 
        this->parent, SLOT(slot_mouse_enter(const QString&)), Qt::AutoConnection);
    
    QWidget::connect(label, SIGNAL(signal_mouse_leave(const QString&)), 
        this->parent, SLOT(slot_mouse_leave(const QString&)), Qt::AutoConnection);
    
    QWidget::connect(label, SIGNAL(signal_mouse_press(const QString&)), 
        this->parent, SLOT(slot_mouse_press(const QString&)), Qt::AutoConnection);
    
    QWidget::connect(label, SIGNAL(signal_mouse_release(const QString&)), 
        this->parent, SLOT(slot_mouse_release(const QString&)), Qt::AutoConnection);

    this->stu_dialog_list->insert(client_id, label);

    return true;
}

/* 收到学生图像 */
void OCSFS_Info_Dialog::have_user_status_image(QString &src_client_id, QImage &recv_image) {
    this->signal_have_user_status_image(src_client_id, recv_image);
}

