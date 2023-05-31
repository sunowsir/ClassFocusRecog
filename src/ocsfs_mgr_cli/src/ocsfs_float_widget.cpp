/*
	* File     : ocsfs_float_widget.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月29日 星期一 20时23分22秒
*/

#include "ocsfs_float_widget.h"

OCSFS_Float_Widget::OCSFS_Float_Widget(QWidget *parent) : 
    parent(parent) {

    this->active = new QPieSlice(this);
    this->active->setLabel(QString("积极"));
    this->active->setValue(40);
    this->active->setBrush(Qt::green);
    this->active->setLabelVisible(true);

    this->neutral = new QPieSlice(this);
    this->neutral->setLabel(QString("中性"));
    this->neutral->setValue(30);
    this->neutral->setBrush(Qt::yellow);
    this->neutral->setLabelVisible(true);

    this->negative = new QPieSlice(this);
    this->neutral->setLabel(QString("消极"));
    this->neutral->setValue(30);
    this->negative->setBrush(Qt::red);
    this->negative->setLabelVisible(true);

    this->series = new QPieSeries(this);
    /* 添加三个片区到组里 */
    this->series->append(this->active);
    this->series->append(this->neutral);
    this->series->append(this->negative);

    this->chart = new QChart();
    /* 添加组 */
    this->chart->addSeries(this->series);
    /* 显示实时动画效果 */
    this->chart->setAnimationOptions(QChart::AllAnimations);

    this->chartview = new QChartView(this);
    this->chartview->setChart(this->chart);
    this->chartview->hide();

    /* 无边框 */
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);

    this->hide();
}

OCSFS_Float_Widget::~OCSFS_Float_Widget() {
    delete this->active;
    delete this->neutral;
    delete this->negative;
    delete this->series;
    delete this->chart;
    delete this->chartview;
    // delete this->layout;
}


bool OCSFS_Float_Widget::set_percent(const int &active_percent, 
    const int &neutral_percent, 
    const int &negative_percent) {
    int total_percent = active_percent + neutral_percent + negative_percent;
    if (100 != total_percent) {
        qDebug() << "percent error: active_percent: " << active_percent 
            << "neutral_percent: " << neutral_percent
            << "negative_percent: " << negative_percent;
        return false;
    }

    this->series->slices().at(0)->setValue(active_percent);
    this->series->slices().at(1)->setValue(neutral_percent);
    this->series->slices().at(2)->setValue(negative_percent);

    /* 先删掉series */
    this->chartview->chart()->removeAllSeries();
    /* 再重新添加 */
    this->chartview->chart()->addSeries(this->series);

    return true;
}

void OCSFS_Float_Widget::slot_show_widget() {
    /* 先删掉series */
    // this->chartview->chart()->removeAllSeries();
    /* 再重新添加 */
    // this->chartview->chart()->addSeries(this->series);

    this->chartview->show();
    this->show();
}

void OCSFS_Float_Widget::slot_hide_widget() {
    /* 先删掉series */
    // this->chartview->chart()->removeAllSeries();
    /* 再重新添加 */
    // this->chartview->chart()->addSeries(this->series);

    this->chartview->hide();
    this->hide();
}

