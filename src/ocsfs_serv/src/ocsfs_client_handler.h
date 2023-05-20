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

#include "ocsfs_define.h"
#include "ocsfs_expression_recognition.h"
#include "ocsfs_profileface_recognition.h"

/* 客户端处理类，每来一个客户端，都要实例化一个该类的对象来对接客户端
 * 每个客户端对应一个这个类实例化的对象 */
class OCSFS_Client_Handler : public QObject {
private: 
    Q_OBJECT
    
    QString      ip;

    qint16       port;

    QTcpSocket   *socket;

    /* 客户端ID */
    QString      client_id;

    /* 客户端类型 */
    qint8        client_type;

    /* 
     * 0: 确认客户端类型，接收ID
     * 1: 准备确认
     * 2：工作中，传输正式工作数据
     * */
    qint16       step;


    /* 表情识别器 */
    OCSFS_Expression_Recognition            *ser;

    /* 侧脸识别器 */
    OCSFS_Profileface_Recognition           *opr;

private: 
    bool step0_handler(QByteArray &recv_data);
    bool step1_handler(QByteArray &recv_data);
    bool step2_handler(QByteArray &recv_data);

    bool parse_client_type_by_id();
    bool send_data(const QString&, const QString&);

public: 
    OCSFS_Client_Handler(QTcpSocket *socket);
    ~OCSFS_Client_Handler();

    bool get_map_key(QString&);

    void send_recognize_result(const QString&/* 客户端ID（得告诉教师端，这是谁的识别结果） */, 
        const QString&/* 识别结果 */);

/* 自定义信号: 除了qt提供的各种信号，如果不满足需求，也可以自己定义信号
 * 信号不需要实现，只需要在类中声明，然后使用connect与槽函数绑定一起，
 * 当我们在某个地方手动调用我们自定义的信号函数，就自动会调用槽函数，
 * 可以理解为调用信号函数，就相当与调用了槽函数
 * */
signals: 
    /* 自定义一个信号函数，我希望，当处理完学生端发来的图片，得到状态结果后，
     * 调用这个信号函数，把结果丢进去 */
    void image_recognize_over(const QString&/* 客户端ID（得告诉教师端，这是谁的识别结果） */, 
        const QString&/* 识别结果 */);

    /* 自定义一个信号函数，当确认好身份后，如果是教师端，那么调用这个信号函数
     * 将自己传递过去*/
    void mgr_client_ready(OCSFS_Client_Handler*);

/* 自定义的槽函数，关于信号函数和槽函数见如上 */
public slots: 

    /* 从套接字中读取数据 */
    void recv_data();  

};

#endif
