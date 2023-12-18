#include "stdafx.h"

#include "hn_main_window.h"

HnMainWindow::HnMainWindow(int startWidth, int startHeight, QWidget* parent)
    : QMainWindow(parent)
{
    this->resize(startWidth, startHeight);
    centralWidget_ = new HnCentralWidget(this);
    this->setCentralWidget(centralWidget_);
    menuBar_ = new HnMenuBar(this->width(), this);
    this->setMenuBar(menuBar_);
}

HnMainWindow::~HnMainWindow()
{}

