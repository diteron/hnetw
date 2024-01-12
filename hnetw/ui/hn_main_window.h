#pragma once

#include <mutex>

#include <packet/packets/hn_packet.h>
#include <capture/hn_capture_file.h>
#include <capture/hn_packet_capturer.h>
#include <capture/hn_packet_dissector.h>
#include <network/hn_host.h>
#include "hn_central_widget.h"
#include "hn_menubar.h"
#include "hn_packet_list.h"
#include "hn_byte_view.h"
#include "qtmodels/hn_packet_list_model.h"
#include "hn_packet_details.h"
#include "qtmodels/hn_packet_details_model.h"
#include "hn_save_file_dialog.h"

class HnMainWindow : public QMainWindow {

    Q_OBJECT

public:
    HnMainWindow(int startWidth, int startHeight, QWidget* parent = nullptr);
    ~HnMainWindow();

    bool setupNetwork();
    void printErrorMessage(QString errMessage);

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    void setupToolBar();
    void setupStatusBar();
    void setupPacketsViews();
    bool setupHost();
    bool setupCapturer();
    bool setupCaptureInterface();
    void stopCapture();

    int showSaveDialog();

    HnMenuBar* menuBar_ = nullptr;
    HnSaveFileDialog* saveDialog_ = nullptr;
    QToolBar* toolBar_ = nullptr;
    
    QStatusBar* statusBar_ = nullptr;
    QLabel* statusBarIpLabel_ = nullptr;

    HnCaptureFile* captureFile_ = nullptr;
    HnPacketCapturer* packetCapturer_ = nullptr;
    HnPacketDissector* packetDissector_ = nullptr;
    bool captureInProgress_ = false;

    HnHost host_;
    std::vector<std::string> interfacesIpStrings_;
    u_long currentInterfaceIp_ = 0L;
    unsigned short currentPort_ = 0;

    QAction* actionStart_ = nullptr;
    QAction* actionPause_ = nullptr;
    QAction* actionRestart_ = nullptr;

    HnCentralWidget* centralWidget_ = nullptr;
    QSplitter* mainSplitter_ = nullptr;
    QSplitter* treeBytesSplitter_ = nullptr;
    HnPacketList* packetList_ = nullptr;
    HnPacketListModel* packetListModel_ = nullptr;
    HnPacketDetails* packetDetails_ = nullptr;
    HnPacketDetailsModel* packetDetailsModel_ = nullptr;
    HnByteView* packetBytesView_ = nullptr;

private slots:
    void handleOpenFile(QString fname);
    void handleSaveFile(QString fname);
    void handleInterfaceChange(int id);
    void startCapture();
    void pauseCapture();
    void restartCapture();
};
