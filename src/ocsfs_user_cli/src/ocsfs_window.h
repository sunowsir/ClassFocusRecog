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
#include <QMainWindow>

#include "ocsfs_widget.h"

class OCSFS_Window : public QMainWindow
{
    Q_OBJECT

    OCSFS_Widget  *ssi_widget;

public:
    OCSFS_Window(QWidget *parent = nullptr);
    ~OCSFS_Window();

private:
};

#endif
