/*
	* File     : ocsfs_student_info_dialog.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月28日 星期日 13时01分19秒
*/

#ifndef _OCSFS_STUDENT_INFO_DIALOG_H
#define _OCSFS_STUDENT_INFO_DIALOG_H

#include <QLabel>
#include <QDialog>
#include <QWidget>
#include <QString>
#include <QVBoxLayout>

#include "ocsfs_float_widget.h"

class OCSFS_Student_Info_Dialog : public QWidget {
    Q_OBJECT

private: 
    QWidget     *parent;

    QLabel      *info_label;

    OCSFS_Float_Widget *percent_info;

    QString     *client_id;

    QVBoxLayout *layout;

public: 
    OCSFS_Student_Info_Dialog(QWidget *parent = nullptr);
    ~OCSFS_Student_Info_Dialog();

    bool get_client_id(QString&);
    bool set_client_id(const QString&);

    void setText(const QString&);
};

#endif
