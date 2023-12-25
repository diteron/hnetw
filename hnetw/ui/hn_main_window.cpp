#include <stdafx.h>

#include "hn_main_window.h"

HnMainWindow::HnMainWindow(int startWidth, int startHeight, QWidget* parent)
    : QMainWindow(parent)
{
    this->resize(startWidth, startHeight);
    menuBar_ = new HnMenuBar(this->width(), this);
    this->setMenuBar(menuBar_);
    
    centralWidget_ = new HnCentralWidget(this);
    this->setCentralWidget(centralWidget_);

    mainSplitter_ = new QSplitter(Qt::Vertical);
    packetList_ = new HnPacketList(centralWidget_);
    packetListModel_ = new HnPacketListModel(centralWidget_);
    packetList_->setModel(packetListModel_);
    packetDetails_ = new HnPacketDetails(centralWidget_);
    mainSplitter_->addWidget(packetList_);
    mainSplitter_->addWidget(packetDetails_);
    centralWidget_->addWidget(mainSplitter_);
}

HnMainWindow::~HnMainWindow()
{}

