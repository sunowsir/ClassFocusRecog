/*
	* File     : ocsfs_barchar_label.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月28日 星期日 19时43分06秒
*/

#ifndef _OCSFS_BARCHAR_LABEL_H
#define _OCSFS_BARCHAR_LABEL_H

#include <QEvent>
#include <QLabel>
#include <QWidget>
#include <QString>
#include <QMouseEvent>


class OCSFS_BarChartLabel : public QLabel {
    Q_OBJECT

    QString                  *client_id;

public:
    OCSFS_BarChartLabel(const QString&, QWidget *parent = nullptr); 
    bool get_client_id(QString&);
    bool set_client_id(const QString&);

protected:
    bool event(QEvent *event) override;

signals: 
    void signal_mouse_enter(const QString&);
    void signal_mouse_leave(const QString&);
    void signal_mouse_press(const QString&);
    void signal_mouse_release(const QString&);
    
};

#endif
