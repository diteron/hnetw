#pragma once

#include <ui/qtmodels/hn_packet_list_model.h>
#include "hn_capture_file.h"

class HnPacketDissector {
public:
    HnPacketDissector();
    ~HnPacketDissector();

    void setPacketListModel(HnPacketListModel* pListModel);
    void setCaptureFile(HnCaptureFile* capFile);

    void enqueuePacket(raw_packet* rawPacket);
    void startDissection();
    void stopDissection();
    void dissectPackets();

    void reset();

private:
    HnPacketListModel* packetListModel_ = nullptr;
    HnCaptureFile* captureFile_ = nullptr;

    std::atomic<bool> isCapturePermitted_ = false;
    int dissectedPacketsCnt_ = 0;
    long currentPacketOffset_ = 0;
};

