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
    this->setWindowTitle(tr("面向线上教学的课堂状态反馈系统学生端"));

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
    this->setWindowTitle(tr("面向线上教学的课堂状态反馈系统学生端"));

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

    /* 有教师端 发来签到请求 */
    QObject::connect(this->client_socket, SIGNAL(have_mgr_check_in(const QString &)), 
        this->ssi_widget, SLOT(have_mgr_check_in(const QString &)), Qt::AutoConnection);

    /* 有教师端 发来点名请求 */
    QObject::connect(this->client_socket, SIGNAL(have_mgr_roll_call(const QString &)), 
        this->ssi_widget, SLOT(have_mgr_roll_call(const QString &)), Qt::AutoConnection);

    /* 有教师端发来警告 */
    QObject::connect(this->client_socket, SIGNAL(have_mgr_warning(const QString &)), 
        this->ssi_widget, SLOT(have_mgr_warning(const QString &)), Qt::AutoConnection);

    /* 发送摄像头图片到服务器 */
    QObject::connect(this->ssi_widget, SIGNAL(send_image_to_server(const QImage&)), 
        this->client_socket, SLOT(send_image_to_server(const QImage&)), Qt::AutoConnection);

    /* 回应签到 */
    QObject::connect(this->ssi_widget, SIGNAL(response_mgr_check_in()), 
        this->client_socket, SLOT(response_mgr_check_in()), Qt::AutoConnection);

    /* 回应点名 */
    QObject::connect(this->ssi_widget, SIGNAL(response_mgr_roll_call()), 
        this->client_socket, SLOT(response_mgr_roll_call()), Qt::AutoConnection);

    /* 回应警告 */
    QObject::connect(this->ssi_widget, SIGNAL(response_mgr_warning()), 
        this->client_socket, SLOT(response_mgr_warning()), Qt::AutoConnection);
}

/* 握手失败 */
void OCSFS_Window::handshake_failed() {
    this->setWindowTitle(tr("面向线上教学的课堂状态反馈系统学生端"));

    this->ssi_widget->close();
    delete this->ssi_widget;
    this->ssi_widget = nullptr;

    this->login_widget = new OCSFS_Login_Widget(this);
    setCentralWidget(this->login_widget);

    QObject::connect(this->login_widget, SIGNAL(login_to_server(const QString&)), 
        this->client_socket, SLOT(login_to_server(const QString&)), Qt::AutoConnection);
}

