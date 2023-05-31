/*
	* File     : ocsfs_interactive_dialog.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月28日 星期日 13时01分19秒
*/

#ifndef _OCSFS_INTERACTIVE_DIALOG_H
#define _OCSFS_INTERACTIVE_DIALOG_H

#include <QMap>
#include <QDialog>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>

class OCSFS_Interactive_Dialog : public QDialog {
    Q_OBJECT

private: 
    QWidget     *parent;

    QVBoxLayout *layout;

    /* 点击要求所有学生签到的按钮 */
    QPushButton *checkin;

    /* 点击随即抽取学生点名的按钮 */
    QPushButton *rollcall;

    int total_stu_num;

    /* 保存学生是否已经签到 */
    QMap<QString, bool> *is_checkin;


public: 
    OCSFS_Interactive_Dialog(QWidget *parent = nullptr);
    ~OCSFS_Interactive_Dialog();

public slots: 
    void have_user_ready(QString&);
    void have_user_check_in(QString &);
    void have_user_roll_call(QString &);
    
};

#endif
