/*
	* File     : ocsfs_login_widget.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月20日 星期六 17时21分52秒
*/

#include "ocsfs_login_widget.h"

bool OCSFS_Login_Widget::account_check() {
    /* 账号检查 */
    if (this->account->size() != OCSFS_CLIENT_ID_LEN)
        return false;
    return true;
}

/* 构造函数 */
OCSFS_Login_Widget::OCSFS_Login_Widget(QMainWindow *parent) {
    this->parent = parent;

    this->account_label = new QLabel(this);
    this->account_label->setText(tr("ID"));
    this->account_label->move(200, 150);
    this->account_label->resize(100, 30);

    this->account_input = new QLineEdit(this);
    this->account_input->move(260, 145);
    this->account_input->resize(150, 30);

    this->login = new QPushButton(this);
    this->login->setText(tr("登陆"));
    this->login->move(260, 385);

    this->account = new QString();

    QWidget::connect(this->account_input, SIGNAL(textChanged(QString)), 
        this, SLOT(account_input_change(const QString&)), Qt::AutoConnection);

    QWidget::connect(this->login, SIGNAL(released()), 
        this, SLOT(login_button_released()), Qt::AutoConnection);
}

/* 析构函数 */
OCSFS_Login_Widget::~OCSFS_Login_Widget() {
    delete this->account_label;
    delete this->account_input;
    delete this->login;
    delete this->account;
}

/* 账号输入框修改 */
void OCSFS_Login_Widget::account_input_change(const QString &input_account) {
    *this->account = input_account;
}

/* 登陆 */
void OCSFS_Login_Widget::login_button_released() {
    if (this->account_check() == false) {
        QMessageBox::warning(this, tr("警告"), tr("ID 输入错误"));
        return ;
    }

    /* 修改窗口标题 */
    this->parent->setWindowTitle(QString("学生: " + *this->account));

    /* 向服务器发送确认 */
    this->login_to_server(*this->account);
}

void OCSFS_Login_Widget::login_to_server_failed() {
    /* todo: 服务器返回登陆失败 */
}
