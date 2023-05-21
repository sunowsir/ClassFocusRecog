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

    this->user_cli = new QMap<QString, std::pair<QString, QDateTime>>();

    
    /* connect signal with slot */


    return ;
}

OCSFS_Widget::~OCSFS_Widget() {
    delete this->settings;
    delete user_cli;
    return ;
}


/* slot function */


/* 从服务器接收识别结果: 
 * 这里的识别结果是服务器发来的初步的识别结果
 * */
void OCSFS_Widget::slots_recv_result(const QString &src_client_id, const QString &result) {
    // todo: 显示识别结果
    /* not exist */
    if (!this->user_cli->contains(src_client_id)) {
        std::pair<QString, QDateTime> value;
        value.first = result;
        value.second = QDateTime::currentDateTime();
        this->user_cli->insert(src_client_id, value);
    /* exist */
    } else {
        // todo: 如何处理服务器发来的初始识别结果，user_cli保存着每个客户端的状态以及该状态首次的时间戳
    }
}
