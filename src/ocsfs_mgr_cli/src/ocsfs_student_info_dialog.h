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

class OCSFS_Student_Info_Dialog : public QDialog {
    Q_OBJECT

private: 
    QDialog     *parent;

    QLabel      *info_label;

    QString     *client_id;

public: 
    OCSFS_Student_Info_Dialog(QDialog *parent = nullptr);
    ~OCSFS_Student_Info_Dialog();

    bool set_status(const QString&);
    
};

#endif
