/*
	* File     : ocsfs_info_dialog.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月28日 星期日 13时01分19秒
*/

#ifndef _OCSFS_INFO_DIALOG_H
#define _OCSFS_INFO_DIALOG_H

#include <QMap>
#include <QLabel>
#include <QDialog>
#include <QWidget>
#include <QVBoxLayout>
#include <QMouseEvent>

#include "ocsfs_student_info_dialog.h"
#include "ocsfs_define.h"
#include "ocsfs_barchar_label.h"

class OCSFS_Info_Dialog : public QDialog {
    Q_OBJECT

private: 
    QWidget     *parent;

    // QList<OCSFS_Student_Info_Dialog*>   *stu_dialog_list;
    QMap<QString, OCSFS_BarChartLabel*>   *stu_dialog_list;

    QVBoxLayout *layout;
private: 
    bool student_status_get(int&, QString&);

public: 
    OCSFS_Info_Dialog(QWidget *parent = nullptr);
    ~OCSFS_Info_Dialog();

    void have_user_status(QString&, int&);

signals:
    void signal_have_user_status_image(QString&, QImage&);

public slots: 
    bool add_student_label(const QString &);
    void have_user_status_image(QString&, QImage&);
};

#endif
