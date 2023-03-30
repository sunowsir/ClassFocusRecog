/*
	* File     : cfr_window.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月05日 星期日 00时48分56秒
*/

#ifndef CFR_WINDOW_H
#define CFR_WINDOW_H

#include <QWidget>
#include <QMainWindow>

#include "cfr_widget.h"

class CFR_Window : public QMainWindow
{
    Q_OBJECT

    CFR_Widget  *ssi_widget;

public:
    CFR_Window(QWidget *parent = nullptr);
    ~CFR_Window();

private:
};

#endif
