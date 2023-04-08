/*
	* File     : cfr_window.c
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月05日 星期日 00时50分29秒
*/

#include "cfr_window.h"

CFR_Window::CFR_Window(QWidget *parent) 
    : QMainWindow(parent) {

    this->resize(800, 600);
    this->setWindowTitle(tr("课堂状态监测系统演示程序"));

    this->ssi_widget = new CFR_Widget(this);
    setCentralWidget(this->ssi_widget);
    
}

CFR_Window::~CFR_Window() {
    this->ssi_widget->close();
    delete this->ssi_widget;
}
