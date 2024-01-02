#pragma once

#include <mutex>

#include <packet/hn_packet.h>
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


class HnMainWindow : public QMainWindow {

    Q_OBJECT

public:
    HnMainWindow(int startWidth, int startHeight, QWidget* parent = nullptr);
    ~HnMainWindow();

    bool setupNetwork();
    void printErrorMessage(QString errMessage);

private:
    void setupToolBar();
    void setupPacketsViews();
    bool setupHost();
    void setupCapturer();
    void stopCapture();

    HnMenuBar* menuBar_ = nullptr;
    QToolBar* toolBar_ = nullptr;

    HnHost host_;
    HnPacketCapturer* packetCapturer_ = nullptr;
    HnPacketDissector* packetDissector_ = nullptr;
    bool captureInProgress_ = false;
    u_long currentInterfaceIp_ = 0;
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
    void handleInterfaceChange(int id);
    void startCapture();
    void pauseCapture();
    void restartCapture();
    bool setupCaptureInterface();
};
