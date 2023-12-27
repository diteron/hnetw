#pragma once

#include <packet/hn_packet_factory.h>
#include <network/hn_ipv4_socket.h>
#include "hn_capturer_observer.h"

class HnPacketCapturer : public QObject {

    Q_OBJECT

public:
    HnPacketCapturer();
    ~HnPacketCapturer();

    void connectObserver(IHnCapturerObserver* observer);
    void disconnectObserver(IHnCapturerObserver* observer);
    void notifyObservers(HnPacket* packet) const;

    bool setInterfaceToCapture(u_long interfaceIp, unsigned short port);
    void startCapturing();
    void pauseCapturing();

private:
    void capturePackets();

    HnIPv4Socket captureSocket_;
    bool socketSetToCapture_ = false;
    bool capturing_ = false;
    const int BuffSize_ = 65536;
    char* buffer_ = nullptr;

    QVector<IHnCapturerObserver*> observers_;
};

