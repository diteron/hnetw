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

    setupPacketsViews();
    setupCapturer();
}

HnMainWindow::~HnMainWindow()
{}

void HnMainWindow::setCapturer(HnPacketCapturer* capturer)
{
    packetCapturer_ = capturer;
}

void HnMainWindow::startCapture()
{
    if (!packetCapturer_) return;
    packetCapturer_->startCapturing();
}

void HnMainWindow::setupPacketsViews()
{
    packetListModel_ = new HnPacketListModel(centralWidget_);
    packetList_ = new HnPacketList(packetListModel_, centralWidget_);
    packetDetails_ = new HnPacketDetails(centralWidget_);
    packetList_->setDetailsView(packetDetails_);

    mainSplitter_ = new QSplitter(Qt::Vertical, centralWidget_);
    mainSplitter_->addWidget(packetList_);
    mainSplitter_->addWidget(packetDetails_);
    centralWidget_->addWidget(mainSplitter_);
}

void HnMainWindow::setupCapturer()
{
    host_ = HnHost(0);
    host_.initialize();

    packetCapturer_ = new HnPacketCapturer();
    packetCapturer_->setInterfaceToCapture(host_.interfaceIpAt(1), host_.port());
    packetCapturer_->connectObserver(packetListModel_);
}

