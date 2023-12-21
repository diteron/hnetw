#include <stdafx.h>

#include "hn_packet_capturer.h"

HnPacketCapturer::HnPacketCapturer()
{
    capturedPackets_ = new QVector<HnPacket*>;
}

HnPacketCapturer::~HnPacketCapturer()
{
    if (capturedPackets_ && !capturedPackets_->isEmpty()) {
        qDeleteAll(*capturedPackets_);
        delete capturedPackets_;
    }
}

void HnPacketCapturer::connectObserver(IHnCapturerObserver* observer)
{
    observers_.append(observer);
}

void HnPacketCapturer::disconnectObserver(IHnCapturerObserver* observer)
{
    observers_.removeOne(observer);
}

void HnPacketCapturer::notifyObservers(HnPacket* packet) const
{
    for (IHnCapturerObserver* observer : observers_)
        observer->processPacket(packet);
}
