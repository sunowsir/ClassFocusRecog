/*
	* File     : ocsfs_widget.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月05日 星期日 01时18分08秒
*/

#include "ocsfs_widget.h"

bool OCSFS_Widget::student_status_type_get(int &status_num, int &status_type) {
    switch(status_num) {
        case OCSFS_head_PROFILE : {
            status_type = OCSFS_STATUS_TYPE_negative;
        } break;
        case OCSFS_face_COMM : {
            status_type = OCSFS_STATUS_TYPE_neutral;
        } break;
        case OCSFS_face_HAPPY : {
            status_type = OCSFS_STATUS_TYPE_active;
        } break;
        case OCSFS_face_HADE : {
            status_type = OCSFS_STATUS_TYPE_negative;
        } break;
        case OCSFS_face_ANGRY : {
            status_type = OCSFS_STATUS_TYPE_negative;
        } break;
        case 0: {
            status_type = OCSFS_STATUS_TYPE_negative;
        }
        default: {
            status_type = -1;
        }
    }
    return true;
}

OCSFS_Widget::OCSFS_Widget(QMainWindow *parent) 
    : QWidget(parent) {
    /* save config */
    this->settings = new QSettings(QCoreApplication::applicationDirPath() + 
        QString("/Config.ini"), QSettings::IniFormat );

    this->parent = parent;

    /* 网格布局 */
    this->layout = new QGridLayout(this);

    int row_count = layout->rowCount(); // 获取网格布局的总行数
    int current_row = layout->rowCount() - 1; // 获取当前行数
    
    /* 图像展示区域 */
    this->picture_area = new OCSFS_Pic_Show_Dialog(this);
    this->layout->addWidget(this->picture_area, 0, 0);

    /* 互动区域 */
    this->interactive_area = new OCSFS_Interactive_Dialog(this);
    this->layout->addWidget(this->interactive_area, 1, 0);

    /* 学生信息区域 */
    this->info_area = new OCSFS_Info_Dialog(this);
    /* 滚轮 */
    this->info_scrollarea = new QScrollArea(this);
    this->info_scrollarea->setWidgetResizable(true);
    this->info_scrollarea->setAlignment(Qt::AlignRight);
    this->info_scrollarea->setWidget(this->info_area);
    // this->layout->addWidget(this->info_scrollarea, current_row + 1, 1, row_count - current_row + 1, 1);
    this->layout->addWidget(this->info_scrollarea, 0, 1, 2, 1);
    // this->layout->addWidget(this->info_area, 0, 1, 1, 1);

    this->layout->setColumnStretch(0, 2);
    this->layout->setColumnStretch(1, 1);
    this->layout->setRowStretch(0, 2);
    this->layout->setRowStretch(1, 1);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setLayout(this->layout);

    this->show_student_id = new QString();

    // this->float_pie = new OCSFS_Float_Widget();

    this->status_map = new QMap<QString, std::tuple<uint64_t, uint64_t, uint64_t>>();
    this->sleep_status_map = new QMap<QString, std::tuple<uint64_t, uint64_t>>();

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
    // 调用notify-send命令发送系统通知
    QProcess::execute("notify-send", 
        QStringList() << "通知" << QString("学生:" + src_client_id + "已签到"));
    this->interactive_area->have_user_check_in(src_client_id);
}

/* 有学生应答点名 */
void OCSFS_Widget::have_user_roll_call(QString &src_client_id) {
    // 调用notify-send命令发送系统通知
    QProcess::execute("notify-send", 
        QStringList() << "通知" << QString("学生:" + src_client_id + "已收到点名"));
    this->interactive_area->have_user_roll_call(src_client_id);
}

/* 有学生应答警告 */
void OCSFS_Widget::have_user_warning_res(QString &src_client_id) {
    // 调用notify-send命令发送系统通知
    QProcess::execute("notify-send", 
        QStringList() << "通知" << QString("学生:" + src_client_id + "已收到警告"));
}

/* 收到学生状态 */
void OCSFS_Widget::have_user_status(QString &src_client_id, int &status_num) {
    // qDebug() << "src_client_id: " << src_client_id << "status_num: " << status_num;
    int status_type;
    /* 将面部状态对应成听课状态 */
    this->student_status_type_get(status_num, status_type);

    /* 将状态传递给学生信息面板来显示面部状态 */
    this->info_area->have_user_status(src_client_id, status_num);

    /* 获取这个学生闭眼和打哈欠持续的次数 */
    std::tuple<uint64_t, uint64_t> sleep_status;
    if (this->sleep_status_map->contains(src_client_id)) {
        sleep_status = this->sleep_status_map->find(src_client_id).value();
    } 

    /* 判断面部状体是否是闭眼或打哈欠，若是则对应数量加1 */
    switch(status_num) {
        case OCSFS_face_CLOSEEYES : {
            if (std::get<0>(sleep_status) >= OCSFS_CLOSEEYES_keep_count) {
                std::get<0>(sleep_status) = 0;
                status_type = OCSFS_STATUS_TYPE_negative;
            } else {
                std::get<0>(sleep_status)++;
            }
        } break;
        case OCSFS_face_YAWN : {
            if (std::get<1>(sleep_status) >= OCSFS_YAWN_keep_count) {
                std::get<1>(sleep_status) = 0;
                status_type = OCSFS_STATUS_TYPE_negative;
            } else {
                std::get<1>(sleep_status)++;
            }
        } break;
        /* 若都不是则清零 */
        default: {
            std::get<0>(sleep_status) = 0;
            std::get<1>(sleep_status) = 0;
        }
    }

    /* 保存学生打哈欠或闭眼次数 */
    this->sleep_status_map->insert(src_client_id, sleep_status);
    if (-1 == status_type) 
        return ;

    /* 获取学生三种听课状态的持续次数 */
    std::tuple<uint64_t, uint64_t, uint64_t> status;
    if (this->status_map->contains(src_client_id)) {
        status = this->status_map->find(src_client_id).value();
    } 

    uint64_t &active_num = std::get<0>(status);
    uint64_t &neutral_num = std::get<1>(status);
    uint64_t &negative_num = std::get<2>(status);

    /* 给对应听课状态计数器加1 */
    switch(status_type) {
        case OCSFS_STATUS_TYPE_active : {
            active_num++;
            active_num %= 1000000;
        } break;
        case OCSFS_STATUS_TYPE_neutral : {
            neutral_num++;
            neutral_num %= 1000000;
        } break;
        case OCSFS_STATUS_TYPE_negative : {
            negative_num++;
            negative_num %= 1000000;
        } break;
        default: {
            qDebug() << "status_type: " << status_type << ", error";
            return ;
        }
    }

    qDebug() << "active_num: " << active_num << 
        "neutral_num: " << neutral_num << 
        "negative_num: " << negative_num;

    uint64_t total_num = active_num + neutral_num + negative_num;

    /* 保存 */
    this->status_map->insert(src_client_id, status);

    /* 计算百分比并丢给界面去显示 */
    int active_percent = 0; 
    if (total_num != 0)
        active_percent = ((double)active_num / (double)total_num) * 100;

    int neutral_percent = 0; 
    if (total_num != 0)
        neutral_percent = ((double)neutral_num / (double)total_num) * 100;

    int negative_percent = 0; 
    if (total_num != 0)
        negative_percent = 100 - active_percent - neutral_percent;

    // qDebug() << "active_num: " << active_num << 
    //     "neutral_num: " << neutral_num << 
    //     "negative_num: " << negative_num;

    // qDebug() << "active_percent: " << active_percent << 
    //     "neutral_percent: " << neutral_percent << 
    //     "negative_percent: " << negative_percent;

    this->info_area->set_info_percent(active_percent, 
        neutral_percent, negative_percent);

    return ;
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
    std::tuple<uint64_t, uint64_t, uint64_t> status;
    std::get<0>(status) = 0;
    std::get<1>(status) = 0;
    std::get<2>(status) = 0;
    this->status_map->insert(src_client_id, status);
    this->interactive_area->have_user_ready(src_client_id);
}


/* 鼠标悬浮 */
void OCSFS_Widget::slot_mouse_enter(const QString &client_id) {
    // this->float_pie->slot_show_widget();

}

/* 鼠标离开 */
void OCSFS_Widget::slot_mouse_leave(const QString &client_id) {
    // this->float_pie->slot_hide_widget();
}

/* 鼠标点击 */
void OCSFS_Widget::slot_mouse_press(const QString &client_id) {
}

/* 鼠标松开 */
void OCSFS_Widget::slot_mouse_release(const QString &client_id) {
    *this->show_student_id = client_id;
}

void OCSFS_Widget::slot_checkin_click() {
    this->checkin_click();
}

void OCSFS_Widget::slot_rollcall_click() {
    // 生成一个随机数，范围为0到this->status_map的大小减1
    int randomIndex = std::rand() % this->status_map->size();

    // 获取对应的key
    auto it = this->status_map->begin();
    std::advance(it, randomIndex);
    QString randomKey = it.key();
    qDebug() << "抽取到学生: " << randomKey;

    this->rollcall_click(randomKey);
}
