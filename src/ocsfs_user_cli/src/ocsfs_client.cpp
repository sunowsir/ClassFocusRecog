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

    /* 断开连接 */
    QObject::connect(this->socket, SIGNAL(disconnected()), 
        this->mainwindow, SLOT(disconnected_from_server()), Qt::AutoConnection);

    /* 接收到数据 */
    QObject::connect(this->socket, SIGNAL(readyRead()), 
        this, SLOT(recv_data()), Qt::AutoConnection);
}

OCSFS_Client::~OCSFS_Client() {
    delete this->socket;
}

bool OCSFS_Client::send_data(const QString& src_client_id, 
    const QString& dst_client_id, 
    const QString& send_data) {
    return this->send_data_by_byte(src_client_id, dst_client_id, send_data.toUtf8());
}

bool OCSFS_Client::send_data_by_byte(const QString &src_client_id, 
    const QString& dst_client_id, 
    const QByteArray &send_data) {
    QByteArray all_send_data = "";

    all_send_data.append(src_client_id.toUtf8());

    all_send_data.append(dst_client_id.toUtf8());

    all_send_data.append((char*)&this->step, 1);

    int data_len = send_data.size();
    // all_send_data.append((char*)&data_len, sizeof(int));

    all_send_data.append((data_len&(0xff000000)) >> 24);
    all_send_data.append((data_len&(0x00ff0000)) >> 16);
    all_send_data.append((data_len&(0x0000ff00)) >> 8);
    all_send_data.append(data_len&(0x000000ff));

    all_send_data.append(send_data);

    return this->socket->write(all_send_data);
}

void OCSFS_Client::recv_data() {
    QByteArray recv_data  = this->socket->readAll();

    if (recv_data.size() < (long long int)OCSFS_PROTO_HEAD_LEN) {
        this->step = 0;
        this->send_data(this->client_id, OCSFS_SERVER_ID, QString("error"));
        return ;
    }

    int recv_data_idx = 0;
    while ((recv_data_idx <= recv_data.size()) && 
        (recv_data.size() - recv_data_idx + 1) >= (long long int)OCSFS_PROTO_HEAD_LEN) {
        QString src_client_id;
        QString dst_client_id;
        int data_len = 0;
        QByteArray data = "";
        this->parse_data(src_client_id, dst_client_id, 
            recv_data + recv_data_idx, 
            data, data_len);

        recv_data_idx += OCSFS_PROTO_HEAD_LEN + data_len;

        /* 处理服务器发来的消息 */
        switch (this->step) {
            case 0: {
                this->step0_handler(data);
            } break;
            case 1: {
                this->step1_handler(data);
            } break;
            case 2: {
                this->step2_handler(src_client_id, data);
            } break;
            default: {
                qDebug() << "step error";
            };
        }

    }
}

bool OCSFS_Client::parse_data(QString &src_client_id, 
    QString &dst_client_id, 
    const QByteArray &recv_data, 
    QByteArray &data, 
    int &data_len) {

    src_client_id = QString::fromUtf8(recv_data.mid(0, OCSFS_CLIENT_ID_LEN));
    dst_client_id = QString::fromUtf8(recv_data.mid(OCSFS_CLIENT_ID_LEN, OCSFS_CLIENT_ID_LEN));
    int step = recv_data.mid(OCSFS_CLIENT_ID_LEN * 2, 1).toHex().toInt(nullptr, 10);

    int network_data_len;
    std::memcpy(&network_data_len, 
        recv_data.mid(OCSFS_CLIENT_ID_LEN * 2 + 1, sizeof(int)).constData(), 
        sizeof(int));
    data_len = qFromBigEndian(network_data_len);

    if (data_len <= 0) 
        return true;

    data = recv_data.mid(OCSFS_PROTO_HEAD_LEN, data_len);
    
    return true;
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

    qDebug() << "登陆成功, step: " << this->step;

    /* 发送step，向服务器握手，进入第一步 */
    this->send_data(this->client_id, OCSFS_SERVER_ID, QString(static_cast<QChar>(this->step)));

    return true;
}

bool OCSFS_Client::step1_handler(QByteArray &recv_data) {
    if (QString(recv_data) != "ack") {
        this->step = 0;
        qDebug() << "握手失败，需重新登陆";
        this->handshake_failed();
        return false;
    }

    /* 握手成功，进入第二步 */
    this->step++;

    qDebug() << "握手成功，step: " << this->step;

    return true;
}

bool OCSFS_Client::step2_handler(const QString &src_client_id, QByteArray &recv_data) {
    /* 有教师发起签到 */
    if (QString(recv_data) == QString(OCSFS_CheckIn_SYN)) {
        this->have_mgr_check_in(src_client_id);
    /* 有教师向某个学生发起点名 */
    } else if (QString(recv_data) == QString(OCSFS_RollCall_SYN)) {
        this->have_mgr_roll_call(src_client_id);
    /* 有教师端向某个学生发起警告 */
    } else if (QString(recv_data) == QString(OCSFS_To_User_Warning_SYN)) {
        this->have_mgr_warning(src_client_id);
    } else {
        qDebug() << "unknow info, from: " << src_client_id << ": " << recv_data;
    }
    return true;
}

void OCSFS_Client::connect_to_server(const QString &serv_ip) {
    qDebug() << "连接服务器： " << serv_ip;
    this->serv_ip = serv_ip;
    this->socket->connectToHost(QHostAddress(this->serv_ip), OCSFS_SERVER_CTL_PORT);
}

void OCSFS_Client::login_to_server(const QString &client_id) {
    qDebug() << "准备登陆, 用户ID: " << client_id;
    this->client_id = client_id;
    this->send_data(this->client_id, OCSFS_SERVER_ID, this->client_id);
}

void OCSFS_Client::send_image_to_server(const QImage &image) {
    // 将QImage转换为QByteArray
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPG"); // 您可以使用其他格式
    buffer.close();

    this->send_data_by_byte(this->client_id, OCSFS_SERVER_ID, byteArray);
}

/* 回应签到 */
void OCSFS_Client::response_mgr_check_in() {
    this->send_data(this->client_id, OCSFS_SERVER_ID, QString(OCSFS_CheckIn_ACK));
}

/* 回应点名 */
void OCSFS_Client::response_mgr_roll_call() {
    this->send_data(this->client_id, OCSFS_SERVER_ID, QString(OCSFS_RollCall_ACK));
}

/* 回应警告 */
void OCSFS_Client::response_mgr_warning() {
    this->send_data(this->client_id, OCSFS_SERVER_ID, QString(OCSFS_To_User_Warning_ACK));
}
