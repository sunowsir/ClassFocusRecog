/*
	* File     : ocsfs_connect_widget.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月21日 星期日 19时59分23秒
*/

#include "ocsfs_connect_widget.h"

bool OCSFS_Connect_Widget::serv_address_check() {
    /* todo */
    /* ip检查 */
    return true;
}

OCSFS_Connect_Widget::OCSFS_Connect_Widget(QMainWindow *parent) {
    this->parent = parent;

    this->connect_label = new QLabel(this);
    this->connect_label->setText(tr("服务器IP"));
    this->connect_label->move(200, 150);
    this->connect_label->resize(100, 30);

    this->connect_input = new QLineEdit(this);
    this->connect_input->move(260, 145);
    this->connect_input->resize(150, 30);

    this->connect = new QPushButton(this);
    this->connect->setText(tr("连接"));
    this->connect->move(260, 385);

    this->serv_ip = new QString();

    QWidget::connect(this->connect_input, SIGNAL(textChanged(QString)), 
        this, SLOT(connect_input_change(const QString&)), Qt::AutoConnection);
    QWidget::connect(this->connect, SIGNAL(released()), 
        this, SLOT(connect_button_released()), Qt::AutoConnection);

}

OCSFS_Connect_Widget::~OCSFS_Connect_Widget() {
    delete this->connect_label;
    delete this->connect_input;
    delete this->connect;
    delete this->serv_ip;
}

/* IP输入框修改 */
void OCSFS_Connect_Widget::connect_input_change(const QString &input_account) {
    *this->serv_ip = input_account;
}

/* 连接 */
void OCSFS_Connect_Widget::connect_button_released() {
    if (this->serv_address_check() == false) {
        QMessageBox::warning(this, tr("警告"), tr("IP 输入错误"));
        return ;
    }
    
    /* 连接服务器 */
    this->connect_to_server(*this->serv_ip);
}
