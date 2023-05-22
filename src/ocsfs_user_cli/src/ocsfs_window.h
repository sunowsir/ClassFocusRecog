/*
	* File     : ocsfs_window.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月05日 星期日 00时48分56秒
*/

#ifndef OCSFS_WINDOW_H
#define OCSFS_WINDOW_H

#include <QWidget>
#include <QImage>
#include <QTcpSocket>
#include <QMainWindow>

#include "ocsfs_widget.h"
#include "ocsfs_client.h"
#include "ocsfs_login_widget.h"
#include "ocsfs_connect_widget.h"

class OCSFS_Window : public QMainWindow {
    Q_OBJECT

    /* 主界面 */
    OCSFS_Widget  *ssi_widget;

    /* 连接界面 */
    OCSFS_Connect_Widget *connect_widget;

    /* 登陆界面 */
    OCSFS_Login_Widget *login_widget;

    /* 客户端socket类 */
    OCSFS_Client    *client_socket;

public:
    OCSFS_Window(QWidget *parent = nullptr);
    ~OCSFS_Window();

public slots: 
    void connect_to_server_success();
    void login_to_server_success();
};

#endif
