/*
	* File     : ocsfs_client.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月20日 星期六 19时28分18秒
*/

#include "ocsfs_client.h"

OCSFS_Client::OCSFS_Client() {
    this->socket = new QTcpSocket(this);
    this->client_id = 0;
    this->step = 0;
    this->serv_ip = "";

    QObject::connect(this->socket, SIGNAL(QTcpServer::readyRead), 
        this, SLOT(recv_data()), Qt::AutoConnection);
}

OCSFS_Client::~OCSFS_Client() {
    delete this->socket;
}

bool OCSFS_Client::send_data(const QString& src_client_id, const QString& send_data) {
    return this->send_data_by_byte(src_client_id, send_data.toUtf8());
}

bool OCSFS_Client::send_data_by_byte(const QString &src_client_id, 
    const QByteArray &send_data) {
    QByteArray all_send_data = "";

    all_send_data.append(src_client_id.toUtf8());

    all_send_data.append(this->client_id.toUtf8());

    all_send_data.append((char*)&this->step, sizeof(int));

    int data_len = send_data.size();
    all_send_data.append((char*)&data_len, sizeof(int));
    all_send_data.append(send_data);

    return this->socket->write(all_send_data);
}

void OCSFS_Client::recv_date() {
    QByteArray recv_data  = this->socket->readAll();

    if (recv_data.size() < OCSFS_PROTO_HEAD_LEN) {
        this->step = 0;
        this->send_data(QString("0"), QString("error"));
    }

    int recv_data_idx = 0;
    QString client_id = QString::fromUtf8(recv_data.mid(recv_data_idx, OCSFS_CLIENT_ID_LEN));
    if (client_id != this->client_id) {
        this->send_data(QString("0"), QString("error"));
        qDebug() << "client_id error: " << client_id << ", this id: " << this->client_id;
    }

    recv_data_idx += OCSFS_CLIENT_ID_LEN;
    QString src_client_id = QString::fromUtf8(recv_data.mid(recv_data_idx, OCSFS_CLIENT_ID_LEN));
    
    recv_data += OCSFS_PROTO_HEAD_LEN;
    
    /* 处理服务器发来的消息 */
    switch (this->step) {
        case 0: {
            this->step0_handler(recv_data);
        } break;
        case 1: {
            this->step1_handler(recv_data);
        } break;
        case 2: {
            this->step2_handler(src_client_id, recv_data);
        } break;
        default: {
            qDebug() << "step error";
        };
    }
}

bool OCSFS_Client::step0_handler(QByteArray &recv_data) {
    if (QString(recv_data) != "ack") {
        this->login_to_server_failed();
        qDebug() << "step 0 failed.";
        return false;
    } 

    /* 登陆成功，step应当为1 */
    this->login_to_server_success();
    this->step++;

    /* 发送step，向服务器握手，进入第一步 */
    this->send_data(this->client_id, QString(this->step));

    return true;
}

bool OCSFS_Client::step1_handler(QByteArray &recv_data) {
    if (QString(recv_data) != "ack") {
        this->step = 0;
        qDebug() << "step 1 failed.";
        return false;
    }

    /* 握手成功，进入第二步 */
    this->step++;

    return true;
}

bool OCSFS_Client::step2_handler(const QString &src_client_id, QByteArray &recv_data) {
    this->recv_result_from_serv(src_client_id, recv_data);
    return true;
}

void OCSFS_Client::connect_to_server(const QString &serv_ip) {
    this->serv_ip = serv_ip;
    this->socket->connectToHost(QHostAddress(this->serv_ip), OCSFS_SERVER_CTL_PORT);
}

void OCSFS_Client::login_to_server(const QString &client_id) {
    this->client_id = client_id;
    this->send_data(this->client_id, this->client_id);
}

