/*
	* File     : ocsfs_login_widget.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月20日 星期六 17时21分47秒
*/

#ifndef _OCSFS_LOGIN_WIDGET_H
#define _OCSFS_LOGIN_WIDGET_H

#include <QLabel>
#include <QWidget>
#include <QObject>
#include <QLineEdit>
#include <QPushButton>
#include <QMainWindow>
#include <QMessageBox>

/* 登陆界面 */
class OCSFS_Login_Widget : public QWidget {
private: 
    Q_OBJECT

    QMainWindow *parent;

    /* 标签以及输入框 */
    QLabel      *account_label;
    QLineEdit   *account_input;

    /* 登陆按钮 */
    QPushButton *login;

private: 
    /* 账号ID */
    QString     *account;

private: 
    bool account_check();

public: 
    OCSFS_Login_Widget(QMainWindow *parent = nullptr);
    ~OCSFS_Login_Widget();

signals: 
    void connect_to_server(const QString&);
    void login_to_server(const QString&);

public slots: 
    void account_input_change(const QString &);
    void login_button_released();
    void login_to_server_failed();
};

#endif
