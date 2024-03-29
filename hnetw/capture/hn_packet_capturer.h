#pragma once

#include <packet/hn_packet_factory.h>
#include <network/hn_ipv4_socket.h>
#include <ui/qtmodels/hn_packet_list_model.h>
#include "hn_packet_dissector.h"

class HnPacketCapturer {
public:
    HnPacketCapturer();
    ~HnPacketCapturer();

    bool setPacketsDissector(HnPacketDissector* dissector);
    bool setInterfaceToCapture(u_long interfaceIp, unsigned short port);

    void startCapturing();
    bool pauseCapturing();
    bool stopCapturing();
    void resetStatistics();

private:
    void capturePackets();

    HnIPv4Socket captureSocket_;
    bool socketSetToCapture_ = false;

    std::atomic<bool> capturePermitted_ = false;

    int capturedPacketsCnt_ = 0;
    std::clock_t captureStarted_ = 0;
    const int BuffSize_ = 65536;

    HnPacketDissector* dissector_ = nullptr;
};
