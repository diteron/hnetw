#include <stdafx.h>

#include <packet/hn_packet_factory.h>
#include <ui/packetstrees/hn_proto_tree_factory.h>
#include "hn_main_window.h"

HnMainWindow::HnMainWindow(int startWidth, int startHeight, QWidget* parent)
    : QMainWindow(parent), interfacesIpStrings_()
{
    this->resize(startWidth, startHeight);
    
    menuBar_ = new HnMenuBar(this->width(), this);
    this->connect(menuBar_, &HnMenuBar::saveFileTriggered, this, &HnMainWindow::handleSaveFile);
    this->connect(menuBar_, &HnMenuBar::interfaceIdChanged, this, &HnMainWindow::handleInterfaceChange);
    this->setMenuBar(menuBar_);

    saveDialog_ = new HnSaveFileDialog(this);
   
    centralWidget_ = new HnCentralWidget(this);
    this->setCentralWidget(centralWidget_);

    setupToolBar();
    setupStatusBar();
    setupPacketsViews();
}

HnMainWindow::~HnMainWindow()
{
    delete captureFile_;
    delete packetCapturer_;
    delete packetDissector_;

    // Delete a packet and protocol tree factories
    delete HnPacketFactory::instance();
    delete HnProtoTreeFactory::instance();
}

bool HnMainWindow::setupNetwork()
{
    return setupCapturer() && setupHost();
}

void HnMainWindow::printErrorMessage(QString errMessage)
{
    QApplication::beep();
    QMessageBox::critical(this, QApplication::applicationName(), errMessage);
}

void HnMainWindow::closeEvent(QCloseEvent* event)
{
    if (captureFile_->size() == 0) {
        event->accept();
        return;
    }

    pauseCapture();
    int execRes = saveDialog_->exec();

    if (execRes == QDialog::Accepted) {
        bool result = packetCapturer_->stopCapturing();
        if (!result) {
            printErrorMessage("Failed to stop capture!");
            event->ignore();
            return;
        }

        QString fileName = QFileDialog::getSaveFileName(this, nullptr, "Untitled.hnw", "Hnetwork File (*.hnw)");
        result = captureFile_->saveFile(fileName.toStdString());
        if (!result) {
            printErrorMessage("Failed to save file!");
            event->ignore();
            return;
        }

        event->accept();
    }
    else if (execRes == QDialog::Rejected && !saveDialog_->isDiscarded()) {
        bool result = packetCapturer_->stopCapturing();
        if (!result) {
            printErrorMessage("Failed to stop capture!");
            event->ignore();
            return;
        }

        event->accept();
    }
    else {
        event->ignore();
    }
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

bool HnMainWindow::setupCapturer()
{
    captureFile_ = new HnCaptureFile();
    if (!captureFile_->createFile()) {
        printErrorMessage("Failed to create temporary capture file!");
        return false;
    }

    packetDissector_ = new HnPacketDissector();
    packetDissector_->setPacketListModel(packetListModel_);
    packetDissector_->setCaptureFile(captureFile_);
    packetCapturer_ = new HnPacketCapturer();
    packetCapturer_->setPacketsDissector(packetDissector_);

    packetList_->setCaptureFile(captureFile_);

    return true;
}

void HnMainWindow::stopCapture()
{
    bool result = packetCapturer_->stopCapturing();
    if (!result) {
        printErrorMessage("Failed to stop capture!");
        return;
    }

    packetList_->setCaptureInProgress(false);
    packetList_->clear();

    actionStart_->setEnabled(true);
    actionPause_->setEnabled(false);
    actionRestart_->setEnabled(false);
}

void HnMainWindow::handleOpenFile(QString fname)
{
    stopCapture();
}

void HnMainWindow::handleSaveFile(QString fname)
{
    if (captureFile_->size() == 0) return;

    pauseCapture();
    bool result = captureFile_->saveFile(fname.toStdString());
    if (!result) {
        printErrorMessage("Failed to save file!");
    }
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
    if (captureFile_->size() == 0) return;
    
    pauseCapture();
    int execRes = saveDialog_->exec();

    if (execRes == QDialog::Accepted) {
        QString fileName = QFileDialog::getSaveFileName(this, nullptr, "Untitled.hnw", "Hnetwork File (*.hnw)");
        bool result = captureFile_->saveFile(fileName.toStdString());
        if (!result) {
            printErrorMessage("Failed to save file!");
            return;
        }

        result = packetCapturer_->stopCapturing();
        if (!result) {
            printErrorMessage("Failed to stop capture!");
            return;
        }
    }
    else if (execRes == QDialog::Rejected && !saveDialog_->isDiscarded()) {
        bool result = packetCapturer_->stopCapturing();
        if (!result) {
            printErrorMessage("Failed to stop capture!");
            return;
        }
    }
    else {
        return;
    }

    packetList_->clear();
    
    if (!captureFile_->recreate()) {
        printErrorMessage("Failed to recreate capture file!");
        return;
    }

    startCapture();
}
