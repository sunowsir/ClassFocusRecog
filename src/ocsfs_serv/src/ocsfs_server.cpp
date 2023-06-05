/*
	* File     : ocsfs_server.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月13日 星期六 20时41分30秒
*/

#include "ocsfs_server.h"

OCSFS_Server::OCSFS_Server() {
    this->mgr_clients = new QMap<QString, OCSFS_Client_Handler*>();
    this->user_clients = new QMap<QString, OCSFS_Client_Handler*>();

    this->tcp_server = new QTcpServer(this);
    this->tcp_server->listen(QHostAddress::Any, OCSFS_SERVER_CTL_PORT);

    QObject::connect(this->tcp_server, SIGNAL(newConnection()), 
        this, SLOT(fetchSocket()), Qt::AutoConnection);
}

OCSFS_Server::~OCSFS_Server() {
    delete this->mgr_clients;
    delete this->user_clients;
    delete this->tcp_server;
}

void OCSFS_Server::fetchSocket() {
    qDebug() << "new client connect to server.";
    QTcpSocket *new_client = this->tcp_server->nextPendingConnection();
    OCSFS_Client_Handler *client = new OCSFS_Client_Handler(new_client);

    QObject::connect(client, SIGNAL(mgr_client_ready(OCSFS_Client_Handler*)), 
        this, SLOT(mgr_client_ready(OCSFS_Client_Handler*)), Qt::AutoConnection);

    QObject::connect(client, SIGNAL(user_client_ready(OCSFS_Client_Handler*)), 
        this, SLOT(user_client_ready(OCSFS_Client_Handler*)), Qt::AutoConnection);

    QObject::connect(client, SIGNAL(image_recognize_over(const QString&, const QString&)), 
        this, SLOT(send_recognize_result(const QString&, const QString&)), Qt::AutoConnection);

    QObject::connect(client, SIGNAL(have_send_image_to_mgr(const QString&, const QImage&)), 
        this, SLOT(have_send_image_to_mgr(const QString&, const QImage&)), Qt::AutoConnection);

    QObject::connect(client, SIGNAL(have_user_check_in(const QString &)), 
        this, SLOT(have_user_check_in(const QString &)), Qt::AutoConnection);

    QObject::connect(client, SIGNAL(have_user_roll_call(const QString &)), 
        this, SLOT(have_user_roll_call(const QString &)), Qt::AutoConnection);

    QObject::connect(client, SIGNAL(have_user_warning_res(const QString &)), 
        this, SLOT(have_user_warning_res(const QString &)), Qt::AutoConnection);

    QObject::connect(client, SIGNAL(have_mgr_check_in(const QString &)), 
        this, SLOT(have_mgr_check_in(const QString &)), Qt::AutoConnection);

    QObject::connect(client, SIGNAL(have_mgr_roll_call(const QString &, const QString &)), 
        this, SLOT(have_mgr_roll_call(const QString &, const QString &)), Qt::AutoConnection);

    QObject::connect(client, SIGNAL(have_mgr_warning(const QString &, const QString &)), 
        this, SLOT(have_mgr_warning(const QString &, const QString &)), Qt::AutoConnection);
}

void OCSFS_Server::mgr_client_ready(OCSFS_Client_Handler *mgr_client) {
    QString mgr_client_map_key;
    mgr_client->get_map_key(mgr_client_map_key);
    this->mgr_clients->insert(mgr_client_map_key, mgr_client);
}

/* 有学生上线：1. 添加到map里，2. 向教师端报告 */
void OCSFS_Server::user_client_ready(OCSFS_Client_Handler *user_client) {
    QString user_client_map_key;
    user_client->get_map_key(user_client_map_key);
    this->user_clients->insert(user_client_map_key, user_client);

    QString user_client_id;
    user_client->get_client_id(user_client_id);

    for (auto mgr_cli : *(this->mgr_clients)) 
        mgr_cli->have_user_ready(user_client_id);
}

/* 当学生端对象得到识别结果后，触发 image_recognize_over, 
 * 调用如下槽函数，将识别结果以及对应学生端ID发送给教师端 */
void OCSFS_Server::send_recognize_result(const QString &src_client_id, 
    const QString &result) {

    for (auto mgr_cli : *(this->mgr_clients)) 
        mgr_cli->send_recognize_result(src_client_id, result);
}

void OCSFS_Server::have_send_image_to_mgr(const QString &src_client_id, 
    const QImage &image) {

    for (auto mgr_cli : *(this->mgr_clients)) 
        mgr_cli->send_image_to_mgr(src_client_id, image);
}

/* 有学生响应签到 */
void OCSFS_Server::have_user_check_in(const QString &src_client_id) {
    for (auto mgr_cli : *(this->mgr_clients)) 
        mgr_cli->send_have_user_check_in(src_client_id);
}

/* 有学生响应点名 */
void OCSFS_Server::have_user_roll_call(const QString &src_client_id) {
    for (auto mgr_cli : *(this->mgr_clients)) 
        mgr_cli->send_have_user_roll_call(src_client_id);
}

/* 有学生响应警告 */
void OCSFS_Server::have_user_warning_res(const QString &src_client_id) {
    qDebug() << "student recv warning: " << src_client_id;
    for (auto mgr_cli : *(this->mgr_clients)) 
        mgr_cli->send_have_user_warning_res(src_client_id);
}

/* 有教师发起签到: 教师发起签到应当发给所有学生 */
void OCSFS_Server::have_mgr_check_in(const QString &src_client_id) {
    for (auto user_cli : *(this->user_clients)) 
        user_cli->send_have_mgr_check_in(src_client_id);
}

/* 有教师发起点名: 教师发起点名应当发给目的学生端 */
void OCSFS_Server::have_mgr_roll_call(const QString &src_client_id, 
    const QString &dst_client_id) {
    // for (auto user_cli : *(this->user_clients)) {
    //     QString client_id;
    //     user_cli->get_client_id(client_id);
    //     if (client_id != dst_client_id)
    //         continue;
    //     user_cli->send_have_mgr_roll_call(src_client_id);
    // }
    if (this->user_clients->contains(dst_client_id)) {
        auto user_cli = (*this->user_clients)[dst_client_id];
        user_cli->send_have_mgr_roll_call(src_client_id);
    }
}

void OCSFS_Server::have_mgr_warning(const QString &src_client_id, 
    const QString &dst_client_id) {
    // for (auto user_cli : *(this->user_clients)) {
    //     QString client_id;
    //     user_cli->get_client_id(client_id);
    //     if (client_id != dst_client_id)
    //         continue;
    //     user_cli->send_have_mgr_warning(src_client_id);
    // }
    if (this->user_clients->contains(dst_client_id)) {
        auto user_cli = (*this->user_clients)[dst_client_id];
        user_cli->send_have_mgr_warning(src_client_id);
    }
}
