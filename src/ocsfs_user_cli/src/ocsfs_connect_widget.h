/*
	* File     : ocsfs_connect_widget.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月21日 星期日 19时53分42秒
*/

#ifndef _OCSFS_CONNECT_WIDGET_H
#define _OCSFS_CONNECT_WIDGET_H

#include <QLabel>
#include <QWidget>
#include <QObject>
#include <QLineEdit>
#include <QPushButton>
#include <QMainWindow>
#include <QMessageBox>

/* 连接界面 */
class OCSFS_Connect_Widget : public QWidget {
    Q_OBJECT

    QMainWindow *parent;

    /* 标签以及输入框 */
    QLabel      *connect_label;
    QLineEdit   *connect_input;

    /* 连接服务器按钮 */
    QPushButton *connect;

private:
    /* 服务器IP */
    QString     *serv_ip;

private:
    bool serv_address_check();

public:
    OCSFS_Connect_Widget(QMainWindow *parent = nullptr);
    ~OCSFS_Connect_Widget();

signals: 
    void connect_to_server(const QString&);
    
public slots: 
    void connect_input_change(const QString &);
    void connect_button_released();
};

#endif
