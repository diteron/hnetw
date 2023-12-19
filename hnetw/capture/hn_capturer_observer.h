#pragma once

#include "../packet/hn_packet.h"

class IHnCapturerObserver {
public:
    IHnCapturerObserver() 
    {}
    ~IHnCapturerObserver()
    {}

    virtual void processPacket(HnPacket* packet) = 0;
};
