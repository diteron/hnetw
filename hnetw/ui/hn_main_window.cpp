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


    packetListModel_ = new HnPacketListModel(centralWidget_);
    packetList_ = new HnPacketList(packetListModel_, centralWidget_);
    packetDetails_ = new HnPacketDetails(centralWidget_);
    packetDetailsModel_ = new HnPacketDetailsModel(centralWidget_);
    packetDetails_->setModel(packetDetailsModel_);

    mainSplitter_ = new QSplitter(Qt::Vertical);
    mainSplitter_->addWidget(packetList_);
    mainSplitter_->addWidget(packetDetails_);
    centralWidget_->addWidget(mainSplitter_);

    // Network initialization for packet capture
    host_ = HnHost(0);
    host_.initialize();
    std::mutex* sharedMutex = new std::mutex();
    packetCapturer_ = new HnPacketCapturer(sharedMutex);
    packetCapturer_->setInterfaceToCapture(host_.interfaceAt(1), host_.port());
    packetCapturer_->connectObserver(packetListModel_);
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

