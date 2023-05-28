/*
	* File     : ocsfs_widget.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月05日 星期日 01时18分08秒
*/

#include "ocsfs_widget.h"

OCSFS_Widget::OCSFS_Widget(QMainWindow *parent) 
    : QWidget(parent) {
    /* save config */
    this->settings = new QSettings(QCoreApplication::applicationDirPath() + 
        QString("/Config.ini"), QSettings::IniFormat );

    this->parent = parent;

    /* 网格布局 */
    this->layout = new QGridLayout(this);

    /* 学生信息区域 */
    this->info_area = new OCSFS_Info_Dialog(this);
    /* 滚轮 */
    this->info_scrollarea = new QScrollArea(this);

    this->info_scrollarea->setWidgetResizable(true);
    this->info_scrollarea->setAlignment(Qt::AlignRight);
    this->info_scrollarea->setWidget(this->info_area);

    this->layout->addWidget(this->info_scrollarea, 0, 1, 1, 1);
    // this->layout->addWidget(this->info_area, 0, 1, 1, 1);
    
    /* 图像展示区域 */
    this->picture_area = new OCSFS_Pic_Show_Dialog(this);
    this->layout->addWidget(this->picture_area, 0, 0);

    /* 互动区域 */
    this->interactive_area = new OCSFS_Interactive_Dialog(this);
    this->layout->addWidget(this->interactive_area, 1, 0);

    this->layout->setColumnStretch(0, 2);
    this->layout->setColumnStretch(1, 1);
    this->layout->setRowStretch(0, 2);
    this->layout->setRowStretch(1, 1);

    this->setLayout(this->layout);

    this->show_student_id = new QString();

    return ;
}

OCSFS_Widget::~OCSFS_Widget() {
    delete this->settings;
    delete this->info_area;
    delete this->info_scrollarea;
    delete this->picture_area;
    delete this->interactive_area;
    delete this->layout;
    delete this->show_student_id;
    return ;
}

/* slot function */

/* 有学生应答签到 */
void OCSFS_Widget::have_user_check_in(QString &src_client_id) {
    
}

/* 有学生应答点名 */
void OCSFS_Widget::have_user_roll_call(QString &src_client_id) {
    
}

/* 有学生应答警告 */
void OCSFS_Widget::have_user_warning_res(QString &src_client_id) {
    
}

/* 收到学生状态 */
void OCSFS_Widget::have_user_status(int status) {
    
}

/* 收到学生图像 */
void OCSFS_Widget::have_user_status_image(QString &src_client_id, QImage &recv_image) {
    if (src_client_id != *this->show_student_id)
        return ;
    this->picture_area->recv_show_image(recv_image);
}

/* 有学生上线 */
void OCSFS_Widget::have_user_ready(QString &src_client_id) {
    this->info_area->add_student_label(src_client_id);
}


/* 鼠标悬浮 */
void OCSFS_Widget::slot_mouse_enter(const QString &client_id) {
    
}

/* 鼠标离开 */
void OCSFS_Widget::slot_mouse_leave(const QString &client_id) {
    
}

/* 鼠标点击 */
void OCSFS_Widget::slot_mouse_press(const QString &client_id) {
}

/* 鼠标松开 */
void OCSFS_Widget::slot_mouse_release(const QString &client_id) {
    *this->show_student_id = client_id;
}

