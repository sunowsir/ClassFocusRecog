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
#include <QComboBox>
#include <QPushButton>
#include <QMainWindow>
#include <QCoreApplication>

#include <QtMultimedia>
#include <QMediaDevices>
#include <QtMultimediaWidgets/QtMultimediaWidgets>

class SSI_Widget: public QWidget {
    Q_OBJECT
private: 

    QSettings       *settings;

    QMainWindow     *parent;
    
    /* 打开摄像头的按钮 */
    QPushButton     *open_camera;

    /* 列出摄像头下拉列表 */
    QComboBox       *camera_list;

    /* 列出摄像头名称列表 */
    QStringList     *camera_name_list;

    /* 摄像头列表 */
    QList<std::pair<QString, QCamera*>>    *cameras_list;

private: 
    
    void get_camera_list();

public: 
    SSI_Widget(QMainWindow *parent = nullptr);
    ~SSI_Widget();

public slots:
    void slots_open_camera();
};

#endif
