/*
	* File     : src/ssi_window.c
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月05日 星期日 00时50分29秒
*/

#include "ssi_window.h"

SSI_Window::SSI_Window(QWidget *parent) 
    : QMainWindow(parent) {

    this->resize(800, 600);
    this->setWindowTitle(tr("课堂状态监测系统演示程序"));

    this->ssi_widget = new SSI_Widget(this);
    setCentralWidget(this->ssi_widget);
    
}

SSI_Window::~SSI_Window() {
    this->ssi_widget->close();
    delete this->ssi_widget;
}
