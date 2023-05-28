/*
	* File     : ocsfs_interactive_dialog.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月28日 星期日 13时01分19秒
*/

#ifndef _OCSFS_INTERACTIVE_DIALOG_H
#define _OCSFS_INTERACTIVE_DIALOG_H

#include <QDialog>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class OCSFS_Interactive_Dialog : public QDialog {
    Q_OBJECT

private: 
    QWidget     *parent;

    QVBoxLayout *layout;

    /* 点击要求所有学生签到的按钮 */
    QPushButton *checkin;

    /* 点击随即抽取学生点名的按钮 */
    QPushButton *rollcall;

public: 
    OCSFS_Interactive_Dialog(QWidget *parent = nullptr);
    ~OCSFS_Interactive_Dialog();
    
};

#endif
