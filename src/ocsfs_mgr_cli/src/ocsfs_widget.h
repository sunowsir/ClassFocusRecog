/* File     : ocsfs_widget.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月05日 星期日 01时13分49秒
*/

#ifndef _OCSFS_WIDGET_H
#define _OCSFS_WIDGET_H

#include <QPoint>
#include <QTimer>
#include <QString>
#include <QImage>
#include <QSettings>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QMainWindow>
#include <QMessageBox>
#include <QGridLayout>
#include <QVideoWidget>
#include <QCoreApplication>

#include <QScrollArea>

#include <QtMultimedia>
#include <QMediaDevices>

#include "ocsfs_info_dialog.h"
#include "ocsfs_pic_show_dialog.h"
#include "ocsfs_interactive_dialog.h"

class OCSFS_Widget: public QWidget {
    Q_OBJECT
private: 

    QSettings                *settings;
    QMainWindow              *parent;

    QGridLayout               *layout;

    /* 右侧信息栏 */
    OCSFS_Info_Dialog        *info_area;
    QScrollArea              *info_scrollarea;

    /* 左上画面区 */
    OCSFS_Pic_Show_Dialog    *picture_area;

    /* 左下互动区 */
    OCSFS_Interactive_Dialog *interactive_area;

    /* 鼠标点了哪个学生 */
    QString                  *show_student_id;

public: 
    OCSFS_Widget(QMainWindow *parent = nullptr);
    ~OCSFS_Widget();

signals: 

public slots:
    void have_user_check_in(QString &);
    void have_user_roll_call(QString &);
    void have_user_warning_res(QString &);
    void have_user_status(int);
    void have_user_status_image(QString&, QImage&);
    void have_user_ready(QString&);

    void slot_mouse_enter(const QString&);
    void slot_mouse_leave(const QString&);
    void slot_mouse_press(const QString&);
    void slot_mouse_release(const QString&);
};

#endif
