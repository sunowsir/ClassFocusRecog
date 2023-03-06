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
#include <QMessageBox>
#include <QVideoWidget>
#include <QCoreApplication>

#include <QtMultimedia>
#include <QMediaDevices>
#include <QtMultimediaWidgets/QtMultimediaWidgets>

class SSI_Widget: public QWidget {
    Q_OBJECT
private: 

    QSettings       *settings;

    QMainWindow     *parent;

/* 组件 */
    
    /* 打开摄像头的按钮 */
    QPushButton     *open_camera;

    /* 列出摄像头下拉列表 */
    QComboBox       *camera_list;

    /* 摄像头组件 */
    QVideoWidget    *camera_view;


/* 数据 */

    /* 选择的摄像头 */
    QScopedPointer<QCamera>                 selected_camera;

    /* 列出摄像头名称列表 */
    QStringList                             *camera_name_list;

    /* 摄像头列表 */
    QList<std::pair<QString, QCamera*>>     *cameras_list;

    /* 连接器 */
    QMediaCaptureSession                    *captureSession;

    /* 媒体采集器 */
    QImageCapture                           *imageCapture;

    /* 采集到的当前一帧画面, 该属性随刷新率变化而不断变化 */
    QImage                                  *camera_frame;

private: 
    
    void get_camera_list();

public: 
    SSI_Widget(QMainWindow *parent = nullptr);
    ~SSI_Widget();

public slots:
    void slots_open_camera();
    void slots_select_camera(const QString&);
    void slots_capture_camera_frame(int, const QImage&);
};

#endif
