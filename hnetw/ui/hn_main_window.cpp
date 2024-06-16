#include <stdafx.h>

#include <packet/hn_packet_factory.h>
#include <packet/packetstrees/hn_proto_tree_factory.h>
#include "hn_main_window.h"

HnMainWindow::HnMainWindow(int startWidth, int startHeight, QWidget* parent)
    : QMainWindow(parent), interfacesIpStrings_()
{
    this->resize(startWidth, startHeight);
    
    menuBar_ = new HnMenuBar(this->width(), this);
    this->connect(menuBar_, &HnMenuBar::saveFileTriggered, this, &HnMainWindow::handleSaveFile);
    this->connect(menuBar_, &HnMenuBar::openFileTriggered, this, &HnMainWindow::handleOpenFile);
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
    QMessageBox::critical(this, QApplication::applicationName(), errMessage);
}

void HnMainWindow::closeEvent(QCloseEvent* event)
{   
    if (captureFile_->size() > 0 && captureFile_->isLiveCapture()) {
        if (captureInProgress_) {
            pauseCapture();
        }
        
        int execRes = saveDialog_->exec();

        if (execRes == QDialog::Accepted) {
            QString fileName = QFileDialog::getSaveFileName(this, nullptr, "Untitled.hnw", "Hnetwork File (*.hnw)");
            bool result = captureFile_->saveFile(fileName.toStdString());
            if (!result) {
                printErrorMessage("Failed to save file!");
                event->ignore();
                return;
            }
        }
        else if (execRes == QDialog::Rejected && saveDialog_->isDiscarded()) {
            event->ignore();
            startCapture();
            return;
        }
    }

    bool result = packetCapturer_->stopCapturing();
    if (!result) {
        printErrorMessage("Failed to stop capture!");
        event->ignore();
        return;
    }

    event->accept();
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
    if (!captureFile_->create()) {
        printErrorMessage("Failed to create temporary capture file!");
        return false;
    }

    packetDissector_ = new HnPacketDissector();
    packetDissector_->setPacketListModel(packetListModel_);
    packetDissector_->setCaptureFile(captureFile_);
    packetCapturer_ = new HnPacketCapturer();
    
    if (!packetCapturer_->setPacketsDissector(packetDissector_)) {
        printErrorMessage("Failed to set packet dissector!");
        return false;
    }

    packetList_->setCaptureFile(captureFile_);

    return true;
}

bool HnMainWindow::setupCaptureInterface()
{
    if (currentInterfaceIp_ == 0L) {
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

void HnMainWindow::stopCapture()
{
    bool result = packetCapturer_->stopCapturing();
    if (!result) {
        printErrorMessage("Failed to stop capture!");
        return;
    }

    packetList_->setCaptureInProgress(false);
    packetList_->clear();

    if (!captureFile_->recreate()) {
        printErrorMessage("Failed to recreate capture file!");
        return;
    }

    actionStart_->setEnabled(true);
    actionPause_->setEnabled(false);
    actionRestart_->setEnabled(false);
}

int HnMainWindow::showSaveDialog()
{
    if (captureFile_->size() > 0 && captureFile_->isLiveCapture()) {
        if (captureInProgress_) {
            pauseCapture();
        }
        int execRes = saveDialog_->exec();

        if (execRes == QDialog::Accepted) {
            QString fileName = QFileDialog::getSaveFileName(this, nullptr, "Untitled.hnw", "Hnetwork File (*.hnw)");
            bool result = captureFile_->saveFile(fileName.toStdString());
            if (!result) {
                printErrorMessage("Failed to save file!");
                return HnSaveFileDialog::Accepted;
            }
        }
        else if (execRes == QDialog::Rejected && saveDialog_->isDiscarded()) {
            startCapture();
            return HnSaveFileDialog::Discarded;
        }
    }

    return HnSaveFileDialog::Rejected;
}

void HnMainWindow::handleOpenFile()
{
    if (showSaveDialog() == HnSaveFileDialog::Discarded) {
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(this, nullptr, nullptr, "Hnetwork File (*.hnw)");
    if (fileName.isEmpty()) {
        return;
    }

    stopCapture();
    if (captureFile_->open(fileName.toStdString())) {
        packetListModel_->addRowsFromCapFile(captureFile_);
    }
    else {
        printErrorMessage("Failed to open file!");
        return;
    }

    // Reset capture interface IP, in case we start writing captured packets to a saved file
    currentInterfaceIp_ = 0L;
    menuBar_->deselectInterfaceIp();
    statusBarIpLabel_->setText("Viewing capture file: " + fileName);
}

void HnMainWindow::handleSaveFile(QString fname)
{
    if (fname.isEmpty()) return;

    if (captureInProgress_) {
        pauseCapture();
    }

    if (captureFile_->size() == 0) {
        startCapture();
        return;
    }
    bool result = captureFile_->saveFile(fname.toStdString());
    if (!result) {
        printErrorMessage("Failed to save file!");
    }
}

void HnMainWindow::handleInterfaceChange(int id)
{
    if (id < 0) {
        printErrorMessage("Incorrect interface id!");
        statusBarIpLabel_->setText("Interface IP is not selected");
        return;
    }

    if (showSaveDialog() == HnSaveFileDialog::Discarded) return;

    u_long newInterfaceIp = host_.interfaceIpAt(id);
    currentInterfaceIp_ = newInterfaceIp;

    stopCapture();

    QString ipString = interfacesIpStrings_[id].c_str();
    statusBarIpLabel_->setText("Capturing interface IP: " + ipString);
}

void HnMainWindow::startCapture()
{
    if (!setupCaptureInterface()) {
        return;
    }

    // If we are viewing a saved file, close it and create a temporary file
    if (!captureFile_->isLiveCapture()) {
        packetList_->clear();
        captureFile_->recreate();
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
    if (captureFile_->size() == 0) return;
    
    if (showSaveDialog() == HnSaveFileDialog::Discarded) return;

    bool result = packetCapturer_->stopCapturing();
    if (!result) {
        printErrorMessage("Failed to restart capture!");
        return;
    }

    packetList_->clear();
    
    if (!captureFile_->recreate()) {
        printErrorMessage("Failed to recreate capture file!");
        return;
    }

    startCapture();
}
