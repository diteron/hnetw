#pragma once

#include <packet/hn_packet.h>
#include <network/hn_ipv4_socket.h>
#include "hn_capturer_observer.h"

class HnPacketCapturer {
public:
    HnPacketCapturer(std::mutex* sharedMutex);
    ~HnPacketCapturer();

    void connectObserver(IHnCapturerObserver* observer);
    void disconnectObserver(IHnCapturerObserver* observer);
    void notifyObservers(HnPacket* packet) const;

    bool setInterfaceToCapture(u_long intrface, unsigned short port);
    void startCapturing();
    void pauseCapturing();

    const QVector<HnPacket*>* capturedPackets() const { return capturedPackets_; }

private:
    void capturePackets();

    HnIPv4Socket captureSocket_;
    bool socketSetToCapture_ = false;
    bool capturing_ = false;
    const int BuffSize_ = 65536;
    char* buffer_ = nullptr;

    QVector<IHnCapturerObserver*> observers_;
    QVector<HnPacket*>* capturedPackets_ = nullptr;

    std::mutex* sharedMutex_ = nullptr;
};

