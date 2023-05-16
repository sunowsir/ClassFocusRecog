/*
	* File     : ocsfs_server.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月13日 星期六 20时41分30秒
*/

#include "ocsfs_server.h"

OCSFS_Server::OCSFS_Server() {
    this->clients = new QMap<QString, OCSFS_Client_Handler*>();

    this->tcp_server = new QTcpServer(this);
    this->tcp_server->listen(QHostAddress::Any, OCSFS_SERVER_CTL_PORT);

    QObject::connect(this->tcp_server, SIGNAL(QTcpServer::newConnection), 
        this, SLOT(fetchSocket()), Qt::AutoConnection);
}

OCSFS_Server::~OCSFS_Server() {
    delete this->clients;
    delete this->tcp_server;
}

void OCSFS_Server::fetchSocket() {
    QTcpSocket *new_client = this->tcp_server->nextPendingConnection();

    QString ip = new_client->peerAddress().toString();
    qint16 port = new_client->peerPort();

    QString map_key = ip + QString(port);

    OCSFS_Client_Handler *client = new OCSFS_Client_Handler(new_client);
    this->clients->insert(map_key, client);
}
