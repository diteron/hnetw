#include <stdafx.h>

#include <packet/hn_packet_factory.h>
#include <ui/packetstrees/hn_proto_tree_factory.h>
#include "hn_main_window.h"

HnMainWindow::HnMainWindow(int startWidth, int startHeight, QWidget* parent)
    : QMainWindow(parent), interfacesIpStrings_()
{
    this->resize(startWidth, startHeight);
    
    menuBar_ = new HnMenuBar(this->width(), this);
    this->connect(menuBar_, &HnMenuBar::interfaceIdChanged, this, &HnMainWindow::handleInterfaceChange);
    this->setMenuBar(menuBar_);
   
    centralWidget_ = new HnCentralWidget(this);
    this->setCentralWidget(centralWidget_);

    setupToolBar();
    setupStatusBar();
    setupPacketsViews();
}

HnMainWindow::~HnMainWindow()
{
    delete packetDissector_;
    // Delete a packet and protocol tree factories
    delete HnPacketFactory::instance();
    delete HnProtoTreeFactory::instance();
}

bool HnMainWindow::setupNetwork()
{
    setupCapturer();
    return setupHost();
}

void HnMainWindow::printErrorMessage(QString errMessage)
{
    QApplication::beep();
    QMessageBox::critical(this, QApplication::applicationName(), errMessage);
}

void HnMainWindow::setupToolBar()
{
    toolBar_ = new QToolBar(this);
    toolBar_->setMovable(false);

    actionStart_ = new QAction("Start", toolBar_);
    actionPause_ = new QAction("Pause", toolBar_);
    actionPause_->setEnabled(false);
    actionRestart_ = new QAction("Restart", toolBar_);
    actionRestart_->setEnabled(false);

    connect(actionStart_, &QAction::triggered, this, &HnMainWindow::startCapture);
    connect(actionPause_, &QAction::triggered, this, &HnMainWindow::pauseCapture);
    connect(actionRestart_, &QAction::triggered, this, &HnMainWindow::restartCapture);

    toolBar_->addAction(actionStart_);
    toolBar_->addAction(actionPause_);
    toolBar_->addAction(actionRestart_);

    this->addToolBar(toolBar_);
}

void HnMainWindow::setupStatusBar()
{
    statusBar_ = new QStatusBar(this);
    this->setStatusBar(statusBar_);
    statusBarIpLabel_ = new QLabel("Interface IP is not selected", statusBar_);
    statusBar_->insertPermanentWidget(0, statusBarIpLabel_, 1);     // stretch > 0 moves single widget in the status bar to the left side
}

void HnMainWindow::setupPacketsViews()
{
    packetListModel_ = new HnPacketListModel(centralWidget_);
    packetList_ = new HnPacketList(packetListModel_, centralWidget_);
    packetDetails_ = new HnPacketDetails(centralWidget_);
    packetBytesView_ = new HnByteView(centralWidget_);
    packetList_->setDetailsView(packetDetails_);
    packetList_->setBytesView(packetBytesView_);

    treeBytesSplitter_ = new QSplitter(Qt::Horizontal, centralWidget_);
    treeBytesSplitter_->addWidget(packetDetails_);
    treeBytesSplitter_->addWidget(packetBytesView_);

    mainSplitter_ = new QSplitter(Qt::Vertical, centralWidget_);
    mainSplitter_->addWidget(packetList_);
    mainSplitter_->addWidget(treeBytesSplitter_);
    centralWidget_->addWidget(mainSplitter_);
}

bool HnMainWindow::setupHost()
{
    host_ = HnHost(0);
    bool result = host_.initialize();
    if (!result) {
        printErrorMessage("Failed to initialize host!");
        return false;
    }

    currentPort_ = host_.port();
    interfacesIpStrings_ = host_.interfacesIpStrings();

    menuBar_->setInterfacesIp(host_.interfacesIpStrings());
    menuBar_->showChangeInterfaceDialog();

    return true;
}

void HnMainWindow::setupCapturer()
{
    packetDissector_ = new HnPacketDissector();
    packetDissector_->setPacketListModel(packetListModel_);
    packetCapturer_ = new HnPacketCapturer(this);
    packetCapturer_->setPacketsDissector(packetDissector_);
}

void HnMainWindow::stopCapture()
{
    bool result = packetCapturer_->stopCapturing();
    if (!result) {
        printErrorMessage("Failed to restart capture!");
        return;
    }

    packetList_->setCaptureInProgress(false);
    packetList_->clear();

    actionStart_->setEnabled(true);
    actionPause_->setEnabled(false);
    actionRestart_->setEnabled(false);
}

void HnMainWindow::handleInterfaceChange(int id) 
{
    if (id < 0) {
        printErrorMessage("Incorrect interface id!");
        return;
    }

    u_long newInterfaceIp = host_.interfaceIpAt(id);
    currentInterfaceIp_ = newInterfaceIp;

    if (captureInProgress_) {
        stopCapture();
    }
    else {
        packetCapturer_->resetStatistics();
        packetList_->clear();
    }

    QString ipString = interfacesIpStrings_[id].c_str();
    statusBarIpLabel_->setText("Capturing interface IP: " + ipString);
}

void HnMainWindow::startCapture()
{
    if (!setupCaptureInterface()) {
        return;
    }

    packetCapturer_->startCapturing();
    packetList_->setCaptureInProgress(true);
    captureInProgress_ = true;
    
    actionStart_->setEnabled(false);
    actionPause_->setEnabled(true);
    actionRestart_->setEnabled(true);
}

void HnMainWindow::pauseCapture()
{
    bool result = packetCapturer_->pauseCapturing();
    if (!result) {
        printErrorMessage("Failed to pause capture!");
        return;
    }

    captureInProgress_ = false;
    actionStart_->setEnabled(true);
    actionPause_->setEnabled(false);
    actionRestart_->setEnabled(false);

    packetList_->setCaptureInProgress(false);
}

void HnMainWindow::restartCapture()
{
    bool result = packetCapturer_->stopCapturing();
    if (!result) {
        printErrorMessage("Failed to restart capture!");
        return;
    }

    packetList_->setCaptureInProgress(false);
    packetList_->clear();

    actionStart_->setEnabled(true);
    actionPause_->setEnabled(false);
    actionRestart_->setEnabled(false);

    startCapture();
}

bool HnMainWindow::setupCaptureInterface()
{
    if (currentInterfaceIp_ == 0) {
        printErrorMessage("Interface for capture is not set!");
        return false;
    }

    bool result = packetCapturer_->setInterfaceToCapture(currentInterfaceIp_, currentPort_);
    if (!result) {
        printErrorMessage("Failed to set interface for capture!\nTry running as Administrator.");
        return false;
    }

    return true;
}
