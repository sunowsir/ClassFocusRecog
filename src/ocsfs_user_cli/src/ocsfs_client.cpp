/*
	* File     : ocsfs_client.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月20日 星期六 19时28分18秒
*/

#include "ocsfs_client.h"

OCSFS_Client::OCSFS_Client(QMainWindow *mainwindow) {
    this->mainwindow = mainwindow;
    this->socket = new QTcpSocket(this);
    this->client_id = QString(OCSFS_CLIENT_ID_LEN, '0');
    this->step = 0;
    this->serv_ip = "";


    /* 连接成功 */
    QObject::connect(this->socket, SIGNAL(connected()), 
        this->mainwindow, SLOT(connect_to_server_success()), Qt::AutoConnection);

    QObject::connect(this->socket, SIGNAL(readyRead()), 
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
    qDebug() << "head len: " << all_send_data.size();
    all_send_data.append(send_data);

    qDebug() << "will send data len: " << all_send_data.size();
    return this->socket->write(all_send_data);
}

void OCSFS_Client::recv_data() {
    QByteArray recv_data  = this->socket->readAll();

    if (recv_data.size() < OCSFS_PROTO_HEAD_LEN) {
        this->step = 0;
        this->send_data(QString(OCSFS_CLIENT_ID_LEN, '0'), QString("error"));
    }
    
    recv_data.remove(0, OCSFS_PROTO_HEAD_LEN);
    
    /* 处理服务器发来的消息 */
    switch (this->step) {
        case 0: {
            this->step0_handler(recv_data);
        } break;
        case 1: {
            this->step1_handler(recv_data);
        } break;
        default: {
            qDebug() << "step error";
        };
    }
}

bool OCSFS_Client::step0_handler(QByteArray &recv_data) {
    if (QString(recv_data) != "ack") {
        this->login_to_server_failed();
        qDebug() << "step 0 failed: " << recv_data << ".";
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

void OCSFS_Client::connect_to_server(const QString &serv_ip) {
    qDebug() << "connect to " << serv_ip;
    this->serv_ip = serv_ip;
    this->socket->connectToHost(QHostAddress(this->serv_ip), OCSFS_SERVER_CTL_PORT);
}

void OCSFS_Client::login_to_server(const QString &client_id) {
    qDebug() << "login to server, client_id: " << client_id;
    this->client_id = client_id;
    this->send_data(this->client_id, this->client_id);
}

void OCSFS_Client::send_image_to_server(const QImage &image) {
    // 将QImage转换为QByteArray
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPG"); // 您可以使用其他格式
    buffer.close();

    this->send_data_by_byte(this->client_id, byteArray);
}
