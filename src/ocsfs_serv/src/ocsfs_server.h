/*
	* File     : ocsfs_server.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月13日 星期六 20时41分25秒
*/

#ifndef _OCSFS_SERVER_H
#define _OCSFS_SERVER_H

#include <QMap>
#include <QTcpServer>
#include <QTcpSocket>

#include "ocsfs_client_handler.h"

#define OCSFS_SERVER_CTL_PORT 7890

/* 服务器主类 */
class OCSFS_Server : public QObject {
private: 
    Q_OBJECT

    /* 每个在线的客户端服务器会给其分配一个单独的处理器 */
    /* 保存在线的教师端处理器 */
    QMap<QString, OCSFS_Client_Handler*>   *mgr_clients;

    QTcpServer                    *tcp_server;
    
public: 
    OCSFS_Server();
    ~OCSFS_Server();

public slots: 
    /* 获取通信套接字 */
    void fetchSocket();   
    void mgr_client_ready(OCSFS_Client_Handler*);

    /* 当学生端对象得到识别结果后，触发 image_recognize_over, 
     * 调用如下槽函数，将识别结果以及对应学生端ID发送给教师端 */
    void send_recognize_result(const QString&/* 客户端ID（得告诉教师端，这是谁的识别结果） */, 
        const QString&/* 识别结果 */);
};

#endif
