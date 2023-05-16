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
    QMap<QString, OCSFS_Client_Handler*>   *clients;

    QTcpServer                    *tcp_server;
    
public: 
    OCSFS_Server();
    ~OCSFS_Server();
public slots: 
    /* 获取通信套接字 */
    void fetchSocket();   
};

#endif
