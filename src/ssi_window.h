/*
	* File     : ./src/ssi_window.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月05日 星期日 00时48分56秒
*/

#ifndef SSI_WINDOW_H
#define SSI_WINDOW_H

#include <QWidget>
#include <QMainWindow>

class SSI_Window : public QMainWindow
{
    Q_OBJECT

public:
    SSI_Window(QWidget *parent = nullptr);
    ~SSI_Window();

private:
};

#endif
