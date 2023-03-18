/*
	* File     : ssi_widget.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月05日 星期日 01时13分49秒
*/

#ifndef _SSI_WIDGET_H
#define _SSI_WIDGET_H

#include <QTimer>
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

#include "ssi_module_trainer.h"
#include "ssi_expression_recognition.h"

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
    QCamera                                 *selected_camera;

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

    /* 定时器，用于定时采集摄像头图像 */
    QTimer                                  *capture_timer;

    /* 表情识别器 */
    SSI_Expression_Recognition              *ser;

private: 
    
    void get_camera_list();

public: 
    SSI_Widget(QMainWindow *parent = nullptr);
    ~SSI_Widget();

public slots:
    void slots_open_camera();
    void slots_select_camera(const QString&);
    void slots_capture_camera_frame(int, const QImage&);
    void slots_timer_out();
};

#endif
