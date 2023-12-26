#include <stdafx.h>
#include <thread>

#include "hn_packet_capturer.h"

HnPacketCapturer::HnPacketCapturer(std::mutex* sharedMutex)
    : sharedMutex_(sharedMutex)
{
    capturedPackets_ = new QVector<HnPacket*>;
    buffer_ = new char[BuffSize_];
}

HnPacketCapturer::~HnPacketCapturer()
{
    if (capturing_) capturing_ = false;
    if (capturedPackets_ && !capturedPackets_->isEmpty()) {
        qDeleteAll(*capturedPackets_);
        delete capturedPackets_;
        if (buffer_) delete buffer_;
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

bool HnPacketCapturer::setInterfaceToCapture(u_long intrface, unsigned short port)
{
    if (captureSocket_.createRawSocket() != HnIPv4Socket::Success)
        return false;
    if (captureSocket_.bindToInterface(intrface, port) != HnIPv4Socket::Success)
        return false;
    if (captureSocket_.setPacketCaptureMode() != true)
        return false;

    socketSetToCapture_ = true;
    return true;
}

void HnPacketCapturer::startCapturing()
{
    capturing_ = true;
    std::thread capturing(&HnPacketCapturer::capturePackets, this);
    capturing.detach();
}

void HnPacketCapturer::pauseCapturing()
{
    capturing_ = false;
}

const HnPacket* HnPacketCapturer::getCapturedPacket(int packetId) const
{
    if (packetId < capturedPackets_->size())
        return capturedPackets_->at(packetId);
    return nullptr;
}

void HnPacketCapturer::capturePackets()
{
    if (!socketSetToCapture_) return;

    static unsigned int capturedPacketsCnt = 0;
    int bytesRead = 0;
    static std::time_t captureStarted = clock(), currentPacketTime;

    while (capturing_) {
        std::memset(buffer_, 0, BuffSize_);
        bytesRead = recvfrom(captureSocket_.socketHandle(), buffer_, BuffSize_, 0, 0, 0);
        if (bytesRead > 0 && capturing_) {
            currentPacketTime = clock() - captureStarted;
            ipv4_hdr* ipHeader = reinterpret_cast<ipv4_hdr*>(buffer_);
            uint8_t* rawData = new uint8_t[bytesRead];      // Memory management is in the captured packet object
            std::memcpy(rawData, buffer_, bytesRead);

            HnPacket* capturedPacket = HnPacketFactory::instance()->buildPacket(ipHeader->protocol, ++capturedPacketsCnt);
            if (capturedPacket == nullptr) continue;
            capturedPacket->setPacketData(rawData, bytesRead);
            capturedPacket->setArrivalTime(currentPacketTime);

            sharedMutex_->lock();
            capturedPackets_->append(capturedPacket);       // Shared use of QVector in the HnPacketDetails class
            sharedMutex_->unlock();

            notifyObservers(capturedPacket);
        }
    }
}
