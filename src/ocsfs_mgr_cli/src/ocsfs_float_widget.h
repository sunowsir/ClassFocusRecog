/*
	* File     : ocsfs_float_widget.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月29日 星期一 20时22分07秒
*/

#ifndef _OCSFS_FLOAT_WIDGET_H
#define _OCSFS_FLOAT_WIDGET_H

#include <QWidget>
#include <QObject>
#include <QDialog>
#include <QtCharts>
#include <QVBoxLayout>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

class OCSFS_Float_Widget : public QDialog {
    Q_OBJECT
private: 
    QWidget     *parent;

    QVBoxLayout *layout;

    /* 用来表示学生的积极状态百分比 */
    QPieSlice   *active;
    /* 用来表示学生的中性状态百分比 */
    QPieSlice   *neutral;
    /* 用来表示学生的消极状态百分比 */
    QPieSlice   *negative;

    /* 将如上三片加入series组中 */
    QPieSeries  *series;

    /* 饼图 */
    QChart       *chart;

    QChartView   *chartview;

public : 
    OCSFS_Float_Widget(QWidget *parent = nullptr);
    ~OCSFS_Float_Widget();

    bool set_percent(const int&, const int&, const int&);

public slots: 
    void slot_show_widget();
    void slot_hide_widget();
};

#endif
