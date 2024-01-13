#include <stdafx.h>
#include <thread>

#include "hn_packet_capturer.h"

HnPacketCapturer::HnPacketCapturer()
{}

HnPacketCapturer::~HnPacketCapturer()
{
    stopCapturing();
}

bool HnPacketCapturer::setPacketsDissector(HnPacketDissector* dissector)
{
    dissector_ = dissector;
    if (!dissector_->startDissection())
        return false;

    return true;
}

bool HnPacketCapturer::setInterfaceToCapture(u_long interfaceIp, unsigned short port)
{
    if (captureSocket_.createRawSocket() != HnIPv4Socket::Success)
        return false;

    if (captureSocket_.bindToInterface(interfaceIp, port) != HnIPv4Socket::Success) {
        captureSocket_.close();
        return false;
    }
    if (captureSocket_.setPacketCaptureMode() != true) {
        captureSocket_.close();
        return false;
    }

    socketSetToCapture_ = true;
    return true;
}

void HnPacketCapturer::startCapturing()
{
    capturePermitted_ = true;
    
    if (capturedPacketsCnt_ == 0)
        captureStarted_ = clock();      // Set start time when it is a first start or after restarting capture

    std::thread capturing(&HnPacketCapturer::capturePackets, this);
    capturing.detach();
}

bool HnPacketCapturer::pauseCapturing()
{
    capturePermitted_ = false;

    if (captureSocket_.isValid()) {
        if (captureSocket_.close() != HnIPv4Socket::Success) {
            return false;
        }
    }

    return true;
}

bool HnPacketCapturer::stopCapturing()
{
    if (!pauseCapturing()) return false;
    resetStatistics();
    
    return true;
}

void HnPacketCapturer::resetStatistics()
{
    capturedPacketsCnt_ = 0;
    dissector_->reset();
}

void HnPacketCapturer::capturePackets()
{
    if (!socketSetToCapture_) return;

    int bytesRead = 0;
    std::clock_t currentPacketTime = 0;

    char* buffer = nullptr;
    uint8_t* rawData = nullptr;
    raw_packet* rawPacket = nullptr;

    while (capturePermitted_.load()) {
        buffer = new char[BuffSize_];

        bytesRead = recv(captureSocket_.socketHandle(), buffer, BuffSize_, 0);

        if (bytesRead > 0 && capturePermitted_.load()) {
            rawData = new uint8_t[bytesRead];
            std::memcpy(rawData, buffer, bytesRead);

            currentPacketTime = clock() - captureStarted_;
            rawPacket = new raw_packet { ++capturedPacketsCnt_, currentPacketTime, rawData, bytesRead };
            dissector_->writePacket(rawPacket);
        }

        delete[] buffer;
    }
}
