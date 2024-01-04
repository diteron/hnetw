#pragma once

#include "hn_packet.h"

class HnUdpPacket : public HnPacket {
public:
    HnUdpPacket(int id);
    ~HnUdpPacket();

private:
    static const bool registeredPacket;
};
