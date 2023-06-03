/*
	* File     : ocsfs_student_info_dialog.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月28日 星期日 13时01分52秒
*/

#include "ocsfs_student_info_dialog.h"

OCSFS_Student_Info_Dialog::OCSFS_Student_Info_Dialog(QWidget *parent) {
    this->parent = parent;

    this->layout = new QVBoxLayout(this);
    // 设置垂直布局的对齐方式
    this->layout->setAlignment(Qt::AlignTop);

    this->setLayout(this->layout);

    this->info_label = new QLabel(this);
    // this->info_label->setText(QString("学生：0123456789"));

    // 设置QLabel的样式
    QString style = "QLabel {"
                "background-color: gray;"
                "color: black;"
                "border: 1px solid gray;"
                "border-radius: 5px;"
                "}";
    this->info_label->setStyleSheet(style);

    // 设置QLabel的大小策略
    QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    policy.setHeightForWidth(true);
    this->info_label->setSizePolicy(policy);

    this->layout->addWidget(this->info_label);

    this->percent_info = new OCSFS_Float_Widget(this);

    this->layout->addWidget(this->percent_info);
    this->percent_info->show();

    this->client_id = new QString();
}

OCSFS_Student_Info_Dialog::~OCSFS_Student_Info_Dialog() {
    delete this->info_label;
    delete this->percent_info;
    delete this->layout;
    delete this->client_id;
}

bool OCSFS_Student_Info_Dialog::get_client_id(QString &client_id) {
    client_id = *this->client_id;
    return true;
}

bool OCSFS_Student_Info_Dialog::set_client_id(const QString &client_id) {
    *this->client_id = client_id;
    return true;
}

void OCSFS_Student_Info_Dialog::setText(const QString &text) {
    this->info_label->setText(text);
}
