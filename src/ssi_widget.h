/*
	* File     : ssi_widget.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月05日 星期日 01时13分49秒
*/

#ifndef _SSI_WIDGET_H
#define _SSI_WIDGET_H

#include <QString>
#include <QSettings>
#include <QPushButton>
#include <QMainWindow>
#include <QCoreApplication>

class SSI_Widget: public QWidget {
    Q_OBJECT
private: 

    QSettings       *settings;

    QMainWindow     *parent;
    
    /* 打开摄像头的按钮 */
    QPushButton     *open_camera;

public: 
    SSI_Widget(QMainWindow *parent = nullptr);
    ~SSI_Widget();

public slots:
    void slots_open_camera();
};

#endif
