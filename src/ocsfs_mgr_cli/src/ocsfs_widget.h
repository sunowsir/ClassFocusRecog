/* File     : ocsfs_widget.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月05日 星期日 01时13分49秒
*/

#ifndef _OCSFS_WIDGET_H
#define _OCSFS_WIDGET_H

#include <QMap>
#include <QTimer>
#include <QString>
#include <QDateTime>
#include <QSettings>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QMainWindow>
#include <QMessageBox>
#include <QVideoWidget>
#include <QCoreApplication>

#include <QtMultimedia>
#include <QMediaDevices>

class OCSFS_Widget: public QWidget {
    Q_OBJECT
private: 

    QSettings       *settings;

/* 组件 */
    


/* 数据 */
    QMap<QString, std::pair<QString, QDateTime>> *user_cli;

private: 

public: 
    OCSFS_Widget(QMainWindow *parent = nullptr);
    ~OCSFS_Widget();


public slots:

    /* 从服务器接收识别结果 */
    void slots_recv_result(const QString&, const QString&);
};

#endif
