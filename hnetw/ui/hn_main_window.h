#pragma once

#include <mutex>

#include <packet/hn_packet.h>
#include <capture/hn_packet_capturer.h>
#include <network/hn_host.h>
#include "hn_central_widget.h"
#include "hn_menubar.h"
#include "hn_packetlist.h"
#include "qtmodels/hn_packet_list_model.h"
#include "hn_packet_details.h"
#include "qtmodels/hn_packet_details_model.h"


class HnMainWindow : public QMainWindow {
public:
    HnMainWindow(int startWidth, int startHeight, QWidget* parent = nullptr);
    ~HnMainWindow();

    void setCapturer(HnPacketCapturer* capturer);
    void startCapture();

private:
    HnMenuBar* menuBar_ = nullptr;

    HnHost host_;
    HnPacketCapturer* packetCapturer_;
    std::mutex* sharedMutex_ = nullptr;

    HnCentralWidget* centralWidget_ = nullptr;
    QSplitter* mainSplitter_ = nullptr;
    HnPacketList* packetList_ = nullptr;
    HnPacketListModel* packetListModel_ = nullptr;
    HnPacketDetails* packetDetails_ = nullptr;
    HnPacketDetailsModel* packetDetailsModel_ = nullptr;

public slots:
    void displayPacketDetails(int packetId);
};

