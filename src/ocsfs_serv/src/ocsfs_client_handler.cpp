/*
	* File     : ocsfs_client_handler.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月13日 星期六 20时51分13秒
*/

#include "ocsfs_client_handler.h"

OCSFS_Client_Handler::OCSFS_Client_Handler(QTcpSocket *socket) {
    this->socket = socket;
    this->ip = this->socket->peerAddress().toString();
    this->port = this->socket->peerPort();
    this->step = 0;
    this->client_id = QString(OCSFS_CLIENT_ID_LEN, '0');

    qDebug() << "ip: " << this->ip << ", port: " << this->port;

    /* 人脸识别器 */
    this->ser = new OCSFS_Expression_Recognition(QCoreApplication::applicationDirPath() + 
        QString("/SVM_DATA.xml"));

    /* 侧脸识别器 */
    this->opr = new OCSFS_Profileface_Recognition(QCoreApplication::applicationDirPath() + 
        QString("/haarcascade_profileface.xml"));

    QObject::connect(this->socket, SIGNAL(readyRead()), 
        this, SLOT(recv_data()), Qt::AutoConnection);
}

OCSFS_Client_Handler::~OCSFS_Client_Handler() {
    delete this->socket;
}

bool OCSFS_Client_Handler::get_map_key(QString &map_key) {
    map_key = this->ip + QString(static_cast<QChar>(this->port));
    return true;
}

bool OCSFS_Client_Handler::get_client_id(QString &client_id) {
    client_id = this->client_id;
    return true;
}

void OCSFS_Client_Handler::send_recognize_result(const QString& src_client_id, 
    const QString& result) {
    qDebug() << "will send " << src_client_id << " status result: " << result;
    this->send_data(src_client_id, this->client_id, result);
}

void OCSFS_Client_Handler::send_image_to_mgr(const QString &src_client_id, const QImage &image) {
    // 将QImage转换为QByteArray
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPG"); 
    buffer.close();

    // this->send_data_by_byte(src_client_id, this->client_id, byteArray);
}

void OCSFS_Client_Handler::send_have_user_check_in(const QString &src_client_id) {
    this->send_data(src_client_id, this->client_id, QString(OCSFS_CheckIn_ACK));
}

void OCSFS_Client_Handler::send_have_user_roll_call(const QString &src_client_id) {
    this->send_data(src_client_id, this->client_id, QString(OCSFS_RollCall_ACK));
}

void OCSFS_Client_Handler::send_have_user_warning_res(const QString &src_client_id) {
    this->send_data(src_client_id, this->client_id, QString(OCSFS_To_User_Warning_ACK));
}

void OCSFS_Client_Handler::send_have_mgr_check_in(const QString &src_client_id) {
    this->send_data(src_client_id, this->client_id, QString(OCSFS_CheckIn_SYN));
}

void OCSFS_Client_Handler::send_have_mgr_roll_call(const QString &src_client_id) {
    this->send_data(src_client_id, this->client_id, QString(OCSFS_RollCall_SYN));
}

void OCSFS_Client_Handler::send_have_mgr_warning(const QString &src_client_id) {
    this->send_data(src_client_id, this->client_id, QString(OCSFS_To_User_Warning_SYN));
}

void OCSFS_Client_Handler::have_user_ready(const QString &src_client_id) {
    this->send_data(src_client_id, this->client_id, QString(OCSFS_User_Ready_SYN));
}

/* 确认身份 */
bool OCSFS_Client_Handler::step0_handler(QByteArray &recv_data) {
    this->client_id = QString(recv_data);
    qDebug() << this->client_id << " 登陆成功";
    this->step++;
    this->send_data(OCSFS_SERVER_ID, this->client_id, QString("ack"));

    return this->parse_client_type_by_id();
}

/* 准备 */
bool OCSFS_Client_Handler::step1_handler(QByteArray &recv_data) {
    bool ret = true;
    if (this->step != recv_data.toHex().toInt(&ret, 10)) {
        if (!ret)
            qDebug() << "step1 toInt false";
        this->step = 0;
        this->send_data(OCSFS_SERVER_ID, this->client_id, QString("error"));
        return true;
    }

    this->step++;
    this->send_data(OCSFS_SERVER_ID, this->client_id, QString("ack"));
    qDebug() << this->client_id << " 握手成功";

    return true;
}

/* 正式工作 */
bool OCSFS_Client_Handler::step2_handler(const QString &src_client_id, 
    const QString &dst_client_id, 
    QByteArray &recv_data) {

    /* 有学生响应签到 */
    if (QString(recv_data) == QString(OCSFS_CheckIn_ACK)) {
        this->have_user_check_in(src_client_id);
        return true;
    /* 有学生响应点名 */
    } else if (QString(recv_data) == QString(OCSFS_RollCall_ACK)) {
        this->have_user_roll_call(src_client_id);
        return true;
    /* 有学生响应警告 */
    } else if (QString(recv_data) == QString(OCSFS_To_User_Warning_ACK)) {
        this->have_user_warning_res(src_client_id);
        return true;
    /* 有教师发起签到 */
    } else if (QString(recv_data) == QString(OCSFS_CheckIn_SYN)) {
        this->have_mgr_check_in(src_client_id);
        return true;
    /* 有教师向某个学生发起点名 */
    } else if (QString(recv_data) == QString(OCSFS_RollCall_SYN)) {
        this->have_mgr_roll_call(src_client_id, dst_client_id);
        return true;
    } else if (QString(recv_data) == QString(OCSFS_To_User_Warning_SYN)) {
        this->have_mgr_warning(src_client_id, dst_client_id);
        return true;
    }

    /* 识别学生状态 */

    int face_type = 0;
    QImage recv_image, out_image;
    recv_image.loadFromData(recv_data);

    this->ser->recognize(recv_image, out_image, face_type);
    QString face_type_str = "result: ";
    switch(face_type) {
        case OCSFS_face_COMM: {
            face_type_str += "平静";
        } break;
        case OCSFS_face_HAPPY: {
            face_type_str += "高兴";
        } break;
        case OCSFS_face_HADE: {
            face_type_str += "厌恶";
        } break;
        default: {
            if (this->opr->recognize(recv_image)) {
                face_type = OCSFS_head_PROFILE;
                face_type_str += "侧脸";
            } else 
                face_type_str += "unknow";
        }
    }
    face_type_str += "(" + QString::number(face_type) + ")";
    qDebug() << "服务器识别结果：" << this->client_id << ": " << face_type_str;

    /* 当处理完学生端发来的图片，得到状态结果后，
     * 调用这个信号函数，把结果丢进去 */
    this->image_recognize_over(this->client_id, QString::number(face_type));

    /* 将实时图片也发过去 */
    this->have_send_image_to_mgr(this->client_id, recv_image);

    return true;
}

bool OCSFS_Client_Handler::parse_client_type_by_id() {
    // todo
    /* 未完成：如何根据ID区分学生还是教师 */
    if (this->client_id == QString("0000000001"))
        this->client_type = OCSFS_CLIENT_TYPE_MGR;
    else 
        this->client_type = OCSFS_CLIENT_TYPE_USER;
    
    /* 当确认好身份后，如果是教师端，那么调用这个信号函数
     * 将自己传递过去*/
    if (OCSFS_CLIENT_TYPE_MGR == this->client_type)
        this->mgr_client_ready(this);
    else if (OCSFS_CLIENT_TYPE_USER == this->client_type)
        this->user_client_ready(this);
    else 
        qDebug() << "parse_client_type_by_id error: " << this->client_type;
    return true;
}

bool OCSFS_Client_Handler::send_data(const QString& src_client_id, 
    const QString& dst_client_id, 
    const QString& send_data) {
    return this->send_data_by_byte(src_client_id, dst_client_id, send_data.toUtf8());
}

bool OCSFS_Client_Handler::send_data_by_byte(const QString &src_client_id, 
    const QString& dst_client_id, 
    const QByteArray &send_data) {
    QByteArray all_send_data = "";

    all_send_data.append(src_client_id.toUtf8());

    all_send_data.append(dst_client_id.toUtf8());

    all_send_data.append((char*)&this->step, 1);

    int data_len = send_data.size();

    all_send_data.append((data_len&(0xff000000)) >> 24);
    all_send_data.append((data_len&(0x00ff0000)) >> 16);
    all_send_data.append((data_len&(0x0000ff00)) >> 8);
    all_send_data.append(data_len&(0x000000ff));

    // all_send_data.append((char*)&data_len, sizeof(int));

    all_send_data.append(send_data);

    return this->socket->write(all_send_data);
}

void OCSFS_Client_Handler::recv_data() {
    QByteArray recv_data  = this->socket->readAll();

    if (recv_data.size() < (long long int)OCSFS_PROTO_HEAD_LEN) {
        this->step = 0;
        this->send_data(OCSFS_SERVER_ID, this->client_id, QString("error"));
        qDebug() << "data len error: " << recv_data.size();
    }
    
    QString src_client_id = QString::fromUtf8(recv_data.mid(0, OCSFS_CLIENT_ID_LEN));
    QString dst_client_id = QString::fromUtf8(recv_data.mid(OCSFS_CLIENT_ID_LEN, OCSFS_CLIENT_ID_LEN));

    recv_data.remove(0, OCSFS_PROTO_HEAD_LEN);
    
    switch (this->step) {
        case 0: {
            this->step0_handler(recv_data);
        } break;
        case 1: {
            this->step1_handler(recv_data);
        } break;
        case 2: {
            this->step2_handler(src_client_id, dst_client_id, recv_data);
        } break;
        default: {
            qDebug() << "step error";
        };
    }
}
