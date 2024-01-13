#pragma once

#include <ui/qtmodels/hn_packet_list_model.h>
#include "hn_capture_file.h"

class HnPacketDissector {
public:
    HnPacketDissector();
    ~HnPacketDissector();

    void setPacketListModel(HnPacketListModel* pListModel);
    void setCaptureFile(HnCaptureFile* capFile);

    void writePacket(raw_packet* rawPacket);
    bool startDissection();
    void stopDissection();
    void dissectPackets();

    void reset();

private:
    HnPacketListModel* packetListModel_ = nullptr;
    HnCaptureFile* captureFile_ = nullptr;

    std::atomic<bool> isCapturePermitted_ = false;
    int dissectedPacketsCnt_ = 0;
    std::atomic<long> currentPacketOffset_ = 0;
};

