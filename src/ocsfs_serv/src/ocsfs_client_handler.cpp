/*
	* File     : ocsfs_client_handler.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月13日 星期六 20时51分13秒
*/

#include "ocsfs_client_handler.h"

OCSFS_Client_Handler::OCSFS_Client_Handler(QTcpSocket *socket) {
    this->socket = new QTcpSocket(socket);
    this->ip = this->socket->peerAddress().toString();
    this->port = this->socket->peerPort();

    /* 人脸识别器 */
    this->ser = new OCSFS_Expression_Recognition(QCoreApplication::applicationDirPath() + 
        QString("/SVM_DATA.xml"));

    /* 侧脸识别器 */
    this->opr = new OCSFS_Profileface_Recognition(QCoreApplication::applicationDirPath() + 
        QString("/haarcascade_profileface.xml"));

    QObject::connect(this->socket, SIGNAL(QTcpServer::readyRead), 
        this, SLOT(recv_data()), Qt::AutoConnection);
}

OCSFS_Client_Handler::~OCSFS_Client_Handler() {
    delete this->socket;
}

bool OCSFS_Client_Handler::get_map_key(QString &map_key) {
    map_key = this->ip + QString(this->port);
    return true;
}

void OCSFS_Client_Handler::send_recognize_result(const QString& src_client_id, 
    const QString& result) {
    this->send_data(src_client_id, result);
}

/* 确认身份 */
bool OCSFS_Client_Handler::step0_handler(QByteArray &recv_data) {
    if (recv_data.size() < OCSFS_PROTO_HEAD_LEN) 
        this->send_data(QString("0"), QString("error"));

    this->client_id = recv_data;
    this->step++;
    this->send_data(QString("0"), QString("ack"));

    return this->parse_client_type_by_id();
}

/* 准备 */
bool OCSFS_Client_Handler::step1_handler(QByteArray &recv_data) {
    if (this->step != recv_data.toInt()) {
        this->step = 0;
        this->send_data(QString("0"), QString("error"));
        return true;
    }

    this->step++;
    this->send_data(QString("0"), QString("ack"));

    return true;
}

/* 正式工作 */
bool OCSFS_Client_Handler::step2_handler(QByteArray &recv_data) {
    int face_type = 0;
    QImage recv_image, out_image;
    recv_image.loadFromData(recv_data);

    this->ser->recognize(recv_image, out_image, face_type);
    QString face_type_str = "result: ";
    switch(face_type) {
        case OCSFS_face_COMM: {
            face_type_str += "calm";
        } break;
        case OCSFS_face_HAPPY: {
            face_type_str += "happy";
        } break;
        case OCSFS_face_HADE: {
            face_type_str += "hade";
        } break;
        default: {
            if (this->opr->recognize(recv_image))
                face_type_str += "profile";
            else 
                face_type_str += "unknow";
        }
    }
    face_type_str += "(" + QString::number(face_type) + ")";
    std::cout << this->client_id.toStdString() << ": " << face_type_str.toStdString() << std::endl;

    /* 当处理完学生端发来的图片，得到状态结果后，
     * 调用这个信号函数，把结果丢进去 */
    this->image_recognize_over(this->client_id, face_type_str);

    return true;
}

bool OCSFS_Client_Handler::parse_client_type_by_id() {
    // todo
    /* 未完成：如何根据ID区分学生还是教师 */
    
    /* 当确认好身份后，如果是教师端，那么调用这个信号函数
     * 将自己传递过去*/
    if (OCSFS_CLIENT_TYPE_MGR == this->client_type)
        this->mgr_client_ready(this);
    return true;
}

bool OCSFS_Client_Handler::send_data(const QString& src_client_id, const QString& send_data) {
    QByteArray all_send_data = "";

    all_send_data.append(src_client_id.toUtf8());

    all_send_data.append(this->client_id.toUtf8());

    all_send_data.append((char*)&this->step, sizeof(int));

    int data_len = send_data.size();
    all_send_data.append((char*)&data_len, sizeof(int));
    all_send_data.append(send_data.toUtf8());

    return this->socket->write(all_send_data);
}

void OCSFS_Client_Handler::recv_data() {
    QByteArray recv_data  = this->socket->readAll();

    if (recv_data.size() < OCSFS_PROTO_HEAD_LEN) {
        this->step = 0;
        this->send_data(QString("0"), QString("error"));
    }
    
    recv_data += OCSFS_PROTO_HEAD_LEN;
    

    switch (this->step) {
        case 0: {
            this->step0_handler(recv_data);
        } break;
        case 1: {
            this->step1_handler(recv_data);
        } break;
        case 2: {
            this->step2_handler(recv_data);
        } break;
        default: {
            qDebug() << "step error";
        };
    }
}
