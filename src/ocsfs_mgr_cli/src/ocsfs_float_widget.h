/*
	* File     : ocsfs_float_widget.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月29日 星期一 20时22分07秒
*/

#ifndef _OCSFS_FLOAT_WIDGET_H
#define _OCSFS_FLOAT_WIDGET_H

#include <QColor>
#include <QWidget>
#include <QObject>
#include <QPainter>
#include <QString>
#include <QLabel>

#include <sstream>


#include "ocsfs_define.h"

class OCSFS_Float_Widget : public QLabel {
    Q_OBJECT
private: 
    QWidget     *parent;

    int      active_percent;
    int      neutral_percent;
    int      negative_percent;

    QString     active_text;
    QString     neutral_text;
    QString     negative_text;

    QColor      active_color;
    QColor      neutral_color;
    QColor      negative_color;

    QColor      active_text_color;
    QColor      neutral_text_color;
    QColor      negative_text_color;

public : 
    OCSFS_Float_Widget(QWidget *parent = nullptr);
    ~OCSFS_Float_Widget();

    bool set_percent(const int&, const int&, const int&);
    bool set_color(const QColor&, const QColor&, const QColor&);
    bool set_text_color(const QColor&, const QColor&, const QColor&);

protected:
    void paintEvent(QPaintEvent *event) override;

public slots: 
    void slot_show_widget();
    void slot_hide_widget();
};

#endif
