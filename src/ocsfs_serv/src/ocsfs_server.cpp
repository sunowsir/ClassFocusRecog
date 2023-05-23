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

    this->tcp_server = new QTcpServer(this);
    this->tcp_server->listen(QHostAddress::Any, OCSFS_SERVER_CTL_PORT);

    QObject::connect(this->tcp_server, SIGNAL(newConnection()), 
        this, SLOT(fetchSocket()), Qt::AutoConnection);
}

OCSFS_Server::~OCSFS_Server() {
    delete this->mgr_clients;
    delete this->tcp_server;
}

void OCSFS_Server::fetchSocket() {
    qDebug() << "new client connect to server.";
    QTcpSocket *new_client = this->tcp_server->nextPendingConnection();
    OCSFS_Client_Handler *client = new OCSFS_Client_Handler(new_client);

    QObject::connect(client, SIGNAL(mgr_client_ready(OCSFS_Client_Handler*)), 
        this, SLOT(mgr_client_ready(OCSFS_Client_Handler*)), Qt::AutoConnection);

    QObject::connect(client, SIGNAL(image_recognize_over(const QString&, const QString&)), 
        this, SLOT(send_recognize_result(const QString&, const QString&)), Qt::AutoConnection);
}

void OCSFS_Server::mgr_client_ready(OCSFS_Client_Handler *mgr_client) {
    QString mgr_client_map_key;
    mgr_client->get_map_key(mgr_client_map_key);
    this->mgr_clients->insert(mgr_client_map_key, mgr_client);
}

/* 当学生端对象得到识别结果后，触发 image_recognize_over, 
 * 调用如下槽函数，将识别结果以及对应学生端ID发送给教师端 */
void OCSFS_Server::send_recognize_result(const QString &src_client_id, 
    const QString &result) {

    // todo
    qDebug() << "[DEBUG] receive image from " << src_client_id;
    return ;

    for (auto mgr_cli : *(this->mgr_clients)) 
        mgr_cli->send_recognize_result(src_client_id, result);
}
