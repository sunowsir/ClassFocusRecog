/*
	* File     : ocsfs_processthree_widget.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年06月02日 星期五 02时47分17秒
*/

#include "ocsfs_processthree_widget.h"

void ProgressThree::paintEvent(QPaintEvent *)
{
    //绘制准备工作,启用反锯齿,平移坐标轴中心,等比例缩放
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    if (autoRadius) {
        radius = this->height() / 2;
    }

    //绘制背景
    drawBg(&painter);
    //绘制值1
    drawValue1(&painter);
    //绘制值2
    drawValue2(&painter);
    //绘制值3
    drawValue3(&painter);
    //最后绘制边框盖上去
    drawBorder(&painter);
}

void ProgressThree::drawBg(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(bgColor);
    painter->drawRoundedRect(this->rect(), radius, radius);
    painter->restore();
}

void ProgressThree::drawValue1(QPainter *painter)
{
    painter->save();

    //计算百分比以及对应的宽度
    int sum = value1 + value2 + value3;
    double percent = (double)value1 / sum;
    width1 = this->width() * percent;

    painter->setPen(Qt::NoPen);
    painter->setBrush(color1);

    //计算绘制的区域,需要裁剪圆角部分
    QPainterPath clipPath;
    clipPath.addRoundedRect(this->rect(), radius, radius);
    painter->setClipPath(clipPath);
    QRect rect(0, 0, width1, this->height());
    painter->drawRect(rect);

    //绘制文字
    if (showValue) {
        //设置文字字体+颜色
        if (autoFont) {
            QFont font;
            font.setPixelSize(this->height() * 0.9);
            painter->setFont(font);
        }

        QString text = QString::number(value1);
        if (showPercent) {
            text = QString("%1%").arg(QString::number(percent * 100, 'f', 0));
        }

        painter->setPen(textColor);
        painter->drawText(rect, Qt::AlignCenter, text);
    }

    painter->restore();
}

void ProgressThree::drawValue2(QPainter *painter)
{
    painter->save();

    //计算百分比以及对应的宽度
    int sum = value1 + value2 + value3;
    double percent = (double)value2 / sum;    
    width2 = this->width() * percent;

    painter->setPen(Qt::NoPen);
    painter->setBrush(color2);

    //计算绘制的区域,需要裁剪圆角部分
    QPainterPath clipPath;
    clipPath.addRoundedRect(this->rect(), radius, radius);
    painter->setClipPath(clipPath);
    QRect rect(width1, 0, width2, this->height());
    painter->drawRect(rect);

    //绘制文字
    if (showValue) {
        //设置文字字体+颜色
        if (autoFont) {
            QFont font;
            font.setPixelSize(this->height() * 0.9);
            painter->setFont(font);
        }

        QString text = QString::number(value2);
        if (showPercent) {
            text = QString("%1%").arg(QString::number(percent * 100, 'f', 0));
        }

        painter->setPen(textColor);
        painter->drawText(rect, Qt::AlignCenter, text);
    }

    painter->restore();
}

void ProgressThree::drawValue3(QPainter *painter)
{
    painter->save();

    //宽度减去其他两个就是
    int sum = value1 + value2 + value3;
    double percent = (double)value3 / sum;
    width3 = this->width() - width1 - width2;

    painter->setPen(Qt::NoPen);
    painter->setBrush(color3);

    //计算绘制的区域,需要裁剪圆角部分
    QPainterPath clipPath;
    clipPath.addRoundedRect(this->rect(), radius, radius);
    painter->setClipPath(clipPath);
    QRect rect(width1 + width2, 0, width3, this->height());
    painter->drawRect(rect);

    //绘制文字
    if (showValue) {
        //设置文字字体+颜色
        if (autoFont) {
            QFont font;
            font.setPixelSize(this->height() * 0.9);
            painter->setFont(font);
        }

        QString text = QString::number(value3);
        if (showPercent) {
            text = QString("%1%").arg(QString::number(percent * 100, 'f', 0));
        }

        painter->setPen(textColor);
        painter->drawText(rect, Qt::AlignCenter, text);
    }

    painter->restore();
}

void ProgressThree::drawBorder(QPainter *painter)
{
    painter->save();

    QPen pen;
    pen.setWidthF(borderWidth);
    pen.setColor(borderColor);
    painter->setPen(borderWidth > 0 ? pen : Qt::NoPen);
    painter->setBrush(Qt::NoBrush);

    int radius = this->radius;
    if (autoRadius) {
        radius = this->height() / 2;
    }

    //绘制圆角矩形
    painter->drawRoundedRect(this->rect(), radius, radius);

    painter->restore();
}

