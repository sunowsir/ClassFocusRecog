/*
	* File     : ocsfs_client_handler.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月13日 星期六 20时51分10秒
*/

#ifndef _OCSFS_CLIENT_HANDLER_H
#define _OCSFS_CLIENT_HANDLER_H

#include <QImage>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>

#include "ocsfs_expression_recognition.h"
#include "ocsfs_profileface_recognition.h"

#define OCSFS_CLIENT_TYPE_USER 0
#define OCSFS_CLIENT_TYPE_MGR  1

#define OCSFS_CLIENT_ID_LEN    64

/* 规定，协议头：client_id(string(64)), step(int), data len(int) */
#define OCSFS_PROTO_HEAD_LEN   (OCSFS_CLIENT_ID_LEN + sizeof(int) * 2)

/* 客户端处理类，每来一个客户端，都要实例化一个该类的对象来对接客户端 */
class OCSFS_Client_Handler : public QObject {
private: 
    Q_OBJECT
    
    QString     ip;

    qint16      port;

    QTcpSocket  *socket;

    /* 客户端ID */
    QString     client_id;

    /* 客户端类型 */
    qint8       client_type;

    /* 
     * 0: 确认客户端类型，接收ID
     * 1: 准备确认
     * 2：工作中，传输正式工作数据
     * */
    qint16      step;


    /* 表情识别器 */
    OCSFS_Expression_Recognition            *ser;

    /* 侧脸识别器 */
    OCSFS_Profileface_Recognition           *opr;


private: 
    bool step0_handler(QByteArray &recv_data);
    bool step1_handler(QByteArray &recv_data);
    bool step2_handler(QByteArray &recv_data);

    bool parse_client_type_by_id();
    bool send_data(const QString&);

public: 
    OCSFS_Client_Handler(QTcpSocket *socket);
    ~OCSFS_Client_Handler();

public slots: 

    /* 从套接字中读取数据 */
    void recv_data();  
};

#endif
