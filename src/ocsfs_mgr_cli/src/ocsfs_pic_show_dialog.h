/*
	* File     : ocsfs_pic_show_dialog.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月28日 星期日 13时01分19秒
*/

#ifndef _OCSFS_PIC_SHOW_DIALOG_H
#define _OCSFS_PIC_SHOW_DIALOG_H

#include <QDialog>
#include <QWidget>
#include <QImage>
#include <QLabel>

class OCSFS_Pic_Show_Dialog : public QDialog {
    Q_OBJECT

private: 
    QWidget     *parent;

    QLabel      *show_label;

public: 
    OCSFS_Pic_Show_Dialog(QWidget *parent = nullptr);
    ~OCSFS_Pic_Show_Dialog();
    
    bool recv_show_image(const QImage&);
};

#endif
