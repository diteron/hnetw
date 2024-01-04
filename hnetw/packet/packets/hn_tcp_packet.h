#pragma once

#include "hn_packet.h"

class HnTcpPacket : public HnPacket {
public:
    HnTcpPacket(int id);
    ~HnTcpPacket();

private:
    static const bool registeredPacket;
};

