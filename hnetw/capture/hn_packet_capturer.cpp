#include <stdafx.h>
#include <thread>

#include "hn_packet_capturer.h"

HnPacketCapturer::HnPacketCapturer()
{}

HnPacketCapturer::~HnPacketCapturer()
{
    stopCapturing();
}

void HnPacketCapturer::setPacketsDissector(HnPacketDissector* dissector)
{
    dissector_ = dissector;
    dissector_->startDissection();
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
    capturedPacketsCnt_ = 0;
    dissector_->reset();

    return true;
}

void HnPacketCapturer::resetStatistics()
{
    capturedPacketsCnt_ = 0;
    captureStarted_ = clock();
}

void HnPacketCapturer::capturePackets()
{
    if (!socketSetToCapture_) return;

    int bytesRead = 0;
    std::clock_t currentPacketTime = 0;

    while (capturePermitted_.load()) {
        char* buffer = new char[BuffSize_];

        bytesRead = recv(captureSocket_.socketHandle(), buffer, BuffSize_, 0);

        if (bytesRead > 0 && capturePermitted_.load()) {
            currentPacketTime = clock() - captureStarted_;
            raw_packet rawPacket = { ++capturedPacketsCnt_, currentPacketTime, buffer, bytesRead };
            dissector_->enqueuePacket(rawPacket);
        }
        else {
            delete[] buffer;
        }
    }

    //dissector_->stopDissection();
}
