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

    /* 握手失败 */
    QObject::connect(this->client_socket, SIGNAL(handshake_failed()), 
        this, SLOT(handshake_failed()), Qt::AutoConnection);
}

OCSFS_Window::~OCSFS_Window() {
    if (nullptr != this->connect_widget) {
        this->connect_widget->close();
        delete this->connect_widget;
        this->connect_widget = nullptr;
    }

    if (nullptr != this->login_widget) {
        this->login_widget->close();
        delete this->login_widget;
        this->login_widget = nullptr;
    }

    if (nullptr != this->ssi_widget) {
        this->ssi_widget->close();
        delete this->ssi_widget;
        this->ssi_widget = nullptr;
    }

    if (nullptr != this->client_socket) {
        delete this->client_socket;
        this->client_socket = nullptr;
    }
}

/* 连接成功，关闭连接界面，打开登陆界面 */
void OCSFS_Window::connect_to_server_success() {
    qDebug() << "连接服务器成功!";

    this->connect_widget->close();
    delete this->connect_widget;
    this->connect_widget = nullptr;

    this->login_widget = new OCSFS_Login_Widget(this);
    setCentralWidget(this->login_widget);

    QObject::connect(this->login_widget, SIGNAL(login_to_server(const QString&)), 
        this->client_socket, SLOT(login_to_server(const QString&)), Qt::AutoConnection);
}

/* 断开连接 */
void OCSFS_Window::disconnected_from_server() {
    qDebug() << "与服务器断开连接";

    if (nullptr != this->connect_widget) {
        this->connect_widget->close();
        delete this->connect_widget;
        this->connect_widget = nullptr;
    }

    if (nullptr != this->login_widget) {
        this->login_widget->close();
        delete this->login_widget;
        this->login_widget = nullptr;
    }

    if (nullptr != this->ssi_widget) {
        this->ssi_widget->close();
        delete this->ssi_widget;
        this->ssi_widget = nullptr;
    }

    this->connect_widget = new OCSFS_Connect_Widget(this);
    setCentralWidget(this->connect_widget);


    /* 按下连接按钮 */
    QObject::connect(this->connect_widget, SIGNAL(connect_to_server(const QString&)), 
        this->client_socket, SLOT(connect_to_server(const QString&)), Qt::AutoConnection);
}

/* 登陆成功，关闭登陆界面，打开主界面 */
void OCSFS_Window::login_to_server_success() {
    this->login_widget->close();
    delete this->login_widget;
    this->login_widget = nullptr;
    
    this->ssi_widget = new OCSFS_Widget(this);
    setCentralWidget(this->ssi_widget);

    /* 有学生发来签到响应 */
    QObject::connect(this->client_socket, SIGNAL(have_user_check_in(QString&)), 
        this->ssi_widget, SLOT(have_user_check_in(QString&)), Qt::AutoConnection);

    /* 有学生发来点名响应 */
    QObject::connect(this->client_socket, SIGNAL(have_user_roll_call(QString&)), 
        this->ssi_widget, SLOT(have_user_roll_call(QString&)), Qt::AutoConnection);

    /* 有学生发来状态 */
    QObject::connect(this->client_socket, SIGNAL(have_user_status(int)), 
        this->ssi_widget, SLOT(have_user_status(int)), Qt::AutoConnection);

    /* 有学生发来实时图像 */
    QObject::connect(this->client_socket, SIGNAL(have_user_status_image(QString&, QImage&)), 
        this->ssi_widget, SLOT(have_user_status_image(QString&, QImage&)), Qt::AutoConnection);

    /* 有学生上线 */
    QObject::connect(this->client_socket, SIGNAL(have_user_ready(QString&)), 
        this->ssi_widget, SLOT(have_user_ready(QString&)), Qt::AutoConnection);
}

/* 握手失败 */
void OCSFS_Window::handshake_failed() {
    this->ssi_widget->close();
    delete this->ssi_widget;
    this->ssi_widget = nullptr;

    this->login_widget = new OCSFS_Login_Widget(this);
    setCentralWidget(this->login_widget);

    QObject::connect(this->login_widget, SIGNAL(login_to_server(const QString&)), 
        this->client_socket, SLOT(login_to_server(const QString&)), Qt::AutoConnection);
}
