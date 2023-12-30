#pragma once

#include <ui/qtmodels/hn_packet_list_model.h>
#include "hn_packet_queue.h"

class HnPacketDissector {
public:
    HnPacketDissector();
    ~HnPacketDissector();

    void setPacketListModel(HnPacketListModel* pListModel);

    void enqueuePacket(raw_packet rawPacket);
    void setCaptureInProgress(bool inProgress);
    void startDissection();
    void stopDissection();
    void dissectPackets();

private:
    HnPacketQueue packetQueue_;
    HnPacketListModel* packetListModel_ = nullptr;

    std::atomic<bool> captureInProgress_ = false;
    int dissectedPacketsCnt_ = 0;
};

