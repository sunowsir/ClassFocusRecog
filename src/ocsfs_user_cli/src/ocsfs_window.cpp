/*
	* File     : ocsfs_window.c
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月05日 星期日 00时50分29秒
*/

#include "ocsfs_window.h"

OCSFS_Window::OCSFS_Window(QWidget *parent) 
    : QMainWindow(parent) {

    this->resize(800, 600);
    this->setWindowTitle(tr("课堂状态监测系统演示程序"));

    this->connect_widget = new OCSFS_Connect_Widget(this);
    setCentralWidget(this->connect_widget);

    this->login_widget = nullptr;
    this->ssi_widget = nullptr;

    this->client_socket = new OCSFS_Client(this);

    /* 按下连接按钮 */
    QObject::connect(this->connect_widget, SIGNAL(connect_to_server(const QString&)), 
        this->client_socket, SLOT(connect_to_server(const QString&)), Qt::AutoConnection);

    /* 登陆成功 */
    QObject::connect(this->client_socket, SIGNAL(login_to_server_success()), 
        this, SLOT(login_to_server_success()), Qt::AutoConnection);
}

OCSFS_Window::~OCSFS_Window() {
    if (nullptr != this->connect_widget) {
        this->connect_widget->close();
        delete this->connect_widget;
    }

    if (nullptr != this->login_widget) {
        this->login_widget->close();
        delete this->login_widget;
    }

    if (nullptr != this->ssi_widget) {
        this->ssi_widget->close();
        delete this->ssi_widget;
    }
}

/* 连接成功，关闭连接界面，打开登陆界面 */
void OCSFS_Window::connect_to_server_success() {
    qDebug() << "connect server success!";

    this->connect_widget->close();
    delete this->connect_widget;

    this->login_widget = new OCSFS_Login_Widget(this);
    setCentralWidget(this->login_widget);

    QObject::connect(this->login_widget, SIGNAL(login_to_server(const QString&)), 
        this->client_socket, SLOT(login_to_server(const QString&)), Qt::AutoConnection);
}

/* 登陆成功，关闭登陆界面，打开主界面 */
void OCSFS_Window::login_to_server_success() {
    this->login_widget->close();
    delete this->login_widget;
    
    this->ssi_widget = new OCSFS_Widget(this);
    setCentralWidget(this->ssi_widget);

    QObject::connect(this->ssi_widget, SIGNAL(send_image_to_server(const QImage&)), 
        this->client_socket, SLOT(send_image_to_server(const QImage&)), Qt::AutoConnection);
}
