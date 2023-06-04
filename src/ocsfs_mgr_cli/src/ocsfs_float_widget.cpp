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
    this->resize(OCSFS_PROGRESS_Width, OCSFS_PROGRESS_Height);

    // // 设置窗口大小策略为可变大小
    // this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // /* 无边框 */
    // this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    // this->setAttribute(Qt::WA_TranslucentBackground, true);

    this->active_percent = 30;
    this->neutral_percent = 30;
    this->negative_percent = 40;

    this->active_text = QString(tr("积极:") + QString::number(this->active_percent) + "%");
    this->neutral_text = QString(tr("中性:") + QString::number(this->neutral_percent) + "%");
    this->negative_text = QString(tr("消极:") + QString::number(this->negative_percent) + "%");

    this->active_color  = Qt::green;
    this->neutral_color = Qt::yellow;
    this->negative_color = Qt::red;

    this->active_text_color = Qt::white;
    this->neutral_text_color = Qt::black;
    this->negative_text_color = Qt::black;
}

OCSFS_Float_Widget::~OCSFS_Float_Widget() {
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

    this->active_percent = active_percent;
    this->neutral_percent = neutral_percent;
    this->negative_percent = negative_percent;

    this->active_text = QString(tr("积极:") + QString::number(this->active_percent) + "%");
    this->neutral_text = QString(tr("中性:") + QString::number(this->neutral_percent) + "%");
    this->negative_text = QString(tr("消极:") + QString::number(this->negative_percent) + "%");

    this->update();

    return true;
}

bool OCSFS_Float_Widget::set_color(const QColor &active_color, 
    const QColor &neutral_color, 
    const QColor &negative_color) {

    this->active_color = active_color;
    this->neutral_color = neutral_color;
    this->negative_color = negative_color;

    return true;
}

bool OCSFS_Float_Widget::set_text_color(const QColor &active_text_color, 
    const QColor &neutral_text_color, 
    const QColor &negative_text_color) {

    this->active_text_color = active_text_color;
    this->neutral_text_color = neutral_text_color;
    this->negative_text_color = negative_text_color;

    return true;
}

void OCSFS_Float_Widget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);

    int width = this->width();
    int height = this->height();

    int active_width = (double)width * ((double)this->active_percent / 100.0);
    int neutral_width = (double)width * ((double)this->neutral_percent / 100.0);
    int negative_width = (double)width * ((double)this->negative_percent / 100.0);

    painter.fillRect(0, 0, active_width, height, this->active_color);
    painter.setPen(this->active_text_color);
    painter.drawText(QRectF(0, 
        0, active_width, this->height()), Qt::AlignCenter, this->active_text);

    painter.fillRect(active_width, 0, neutral_width, height, this->neutral_color);
    painter.setPen(this->neutral_text_color);
    painter.drawText(QRectF(active_width, 
        0, neutral_width, this->height()), Qt::AlignCenter, this->neutral_text);

    painter.fillRect(active_width + neutral_width, 0, negative_width, height, this->negative_color);
    painter.setPen(this->negative_text_color);
    painter.drawText(QRectF(active_width + neutral_width, 
        0, negative_width, this->height()), Qt::AlignCenter, this->negative_text);
}

void OCSFS_Float_Widget::slot_show_widget() {
    this->show();
}

void OCSFS_Float_Widget::slot_hide_widget() {
    this->hide();
}

