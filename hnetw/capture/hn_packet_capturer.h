#pragma once

#include <packet/hn_packet.h>
#include <network/hn_ipv4_socket.h>
#include "hn_capturer_observer.h"

class HnPacketCapturer {
public:
    HnPacketCapturer();
    ~HnPacketCapturer();

    void connectObserver(IHnCapturerObserver* observer);
    void disconnectObserver(IHnCapturerObserver* observer);
    void notifyObservers(HnPacket* packet) const;

    const QVector<HnPacket*>* capturedPackets() const { return capturedPackets_; }

private:


    QVector<IHnCapturerObserver*> observers_;
    QVector<HnPacket*>* capturedPackets_ = nullptr;
};

