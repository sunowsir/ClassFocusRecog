/*
	* File     : ocsfs_student_info_dialog.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月28日 星期日 13时01分52秒
*/

#include "ocsfs_student_info_dialog.h"

OCSFS_Student_Info_Dialog::OCSFS_Student_Info_Dialog(QDialog *parent) {
    this->parent = parent;
    this->info_label = new QLabel(this);
    this->info_label->setText(QString("学生：0123456789"));
}

OCSFS_Student_Info_Dialog::~OCSFS_Student_Info_Dialog() {
    delete this->info_label;
}

bool OCSFS_Student_Info_Dialog::set_status(const QString &status) {
    return true;
}
