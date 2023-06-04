/*
	* File     : ocsfs_info_dialog.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月28日 星期日 13时01分52秒
*/

#include "ocsfs_info_dialog.h"

bool OCSFS_Info_Dialog::student_status_get(int &status_num, QString &status) {
    switch(status_num) {
        case OCSFS_head_PROFILE : {
            status = QString("侧脸");
        } break;
        case OCSFS_face_COMM : {
            status = QString("平静");
        } break;
        case OCSFS_face_HAPPY : {
            status = QString("高兴");
        } break;
        case OCSFS_face_HADE : {
            status = QString("厌恶");
        } break;
        case OCSFS_face_ANGRY : {
            status = QString("生气");
        } break;
        case OCSFS_face_CLOSEEYES : {
            status = QString("闭眼");
        } break;
        case OCSFS_face_YAWN : {
            status = QString("哈欠");
        } break;
        case 0 : {
            status = QString("无人脸");
        }
        default: {
            status = QString("未知");
        }
    }
    return true;
}

OCSFS_Info_Dialog::OCSFS_Info_Dialog(QWidget *parent) {
    this->parent = parent;
    this->layout = new QVBoxLayout(this);
    // 设置垂直布局的对齐方式
    this->layout->setAlignment(Qt::AlignTop);

    this->setLayout(this->layout);

    // this->stu_dialog_list = new QMap<QString, OCSFS_BarChartLabel*>();
    this->stu_dialog_list = new QMap<QString, OCSFS_Student_Info_Dialog*>();
}

OCSFS_Info_Dialog::~OCSFS_Info_Dialog() {
    delete this->stu_dialog_list;
    delete this->layout;
}

void OCSFS_Info_Dialog::have_user_status(QString &src_client_id, int &status_num) {

    QString status;
    this->student_status_get(status_num, status);

    // for (auto stu_info: *(this->stu_dialog_list)) {
    //     QString now_client_id;
    //     stu_info->get_client_id(now_client_id);
    //     if (now_client_id != src_client_id)
    //         continue;

    //     stu_info->setText(QString("学生: ") + src_client_id  + 
    //         QString(" 状态: ") + status);
    //     return ;
    // }

    if (this->stu_dialog_list->contains(src_client_id))  {
        auto stu_info = (*this->stu_dialog_list)[src_client_id];
        stu_info->setText(QString("学生: ") + src_client_id  + 
            QString(" 状态: ") + status);
        return ;
    }

    OCSFS_Student_Info_Dialog *label = new OCSFS_Student_Info_Dialog();
    label->setText(QString("学生: ") + src_client_id + 
        QString(" 状态: ") + status);
    label->set_client_id(src_client_id);
    label->resize(190, 50);

    // 设置QLabel的样式
    QString style = 
        "border: 1px solid gray;"
        "border-radius: 5px;";
    label->setStyleSheet(style);

    // 设置QLabel的大小策略
    QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    policy.setHeightForWidth(true);
    label->setSizePolicy(policy);

    QWidget::connect(this, SIGNAL(set_info_percent(const int&, const int&, const int&)), 
        label, SLOT(set_info_percent(const int&, const int&, const int&)), Qt::AutoConnection);

    QWidget::connect(label, SIGNAL(signal_mouse_enter(const QString&)), 
        this->parent, SLOT(slot_mouse_enter(const QString&)), Qt::AutoConnection);

    QWidget::connect(label, SIGNAL(signal_mouse_leave(const QString&)), 
        this->parent, SLOT(slot_mouse_leave(const QString&)), Qt::AutoConnection);

    QWidget::connect(label, SIGNAL(signal_mouse_press(const QString&)), 
        this->parent, SLOT(slot_mouse_press(const QString&)), Qt::AutoConnection);

    QWidget::connect(label, SIGNAL(signal_mouse_release(const QString&)), 
        this->parent, SLOT(slot_mouse_release(const QString&)), Qt::AutoConnection);

    this->layout->addWidget(label);

    this->stu_dialog_list->insert(src_client_id, label);

    return ;
}

bool OCSFS_Info_Dialog::add_student_label(const QString &client_id) {
    if (this->stu_dialog_list->contains(client_id))
        return true;

    // for (auto stu_info: *(this->stu_dialog_list)) {
    //     QString now_client_id;
    //     stu_info->get_client_id(now_client_id);
    //     if (now_client_id != QString("0000000000"))
    //         continue;
    //     stu_info->set_client_id(client_id);
    //     stu_info->setText(QString("学生: ") + client_id);
    //     return true;
    // }

    if (this->stu_dialog_list->contains(client_id)) {
        auto stu_info = (*this->stu_dialog_list)[client_id];
        stu_info->set_client_id(client_id);
        stu_info->setText(QString("学生: ") + client_id);
        return true;
    }

    OCSFS_Student_Info_Dialog *label = new OCSFS_Student_Info_Dialog();
    label->setText(QString("学生: ") + client_id);
    label->set_client_id(client_id);
    label->resize(190, 50);

    // 设置QLabel的样式
    QString style = "QLabel {"
                "background-color: gray;"
                "color: black;"
                "border: 1px solid gray;"
                "border-radius: 5px;"
                "}";
    label->setStyleSheet(style);

    // 设置QLabel的大小策略
    QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    policy.setHeightForWidth(true);
    label->setSizePolicy(policy);

    QWidget::connect(this, SIGNAL(set_info_percent(const int&, const int&, const int&)), 
        label, SLOT(set_info_percent(const int&, const int&, const int&)), Qt::AutoConnection);

    QWidget::connect(label, SIGNAL(signal_mouse_enter(const QString&)), 
        this->parent, SLOT(slot_mouse_enter(const QString&)), Qt::AutoConnection);

    QWidget::connect(label, SIGNAL(signal_mouse_leave(const QString&)), 
        this->parent, SLOT(slot_mouse_leave(const QString&)), Qt::AutoConnection);

    QWidget::connect(label, SIGNAL(signal_mouse_press(const QString&)), 
        this->parent, SLOT(slot_mouse_press(const QString&)), Qt::AutoConnection);

    QWidget::connect(label, SIGNAL(signal_mouse_release(const QString&)), 
        this->parent, SLOT(slot_mouse_release(const QString&)), Qt::AutoConnection);

    this->layout->addWidget(label);

    this->stu_dialog_list->insert(client_id, label);

    return true;
}

/* 收到学生图像 */
void OCSFS_Info_Dialog::have_user_status_image(QString &src_client_id, QImage &recv_image) {
    this->signal_have_user_status_image(src_client_id, recv_image);
}

