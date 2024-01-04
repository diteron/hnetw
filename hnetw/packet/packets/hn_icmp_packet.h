#pragma once
#include "hn_packet.h"

class HnIcmpPacket : public HnPacket {
public:
    HnIcmpPacket(int id);
    ~HnIcmpPacket();

private:
    static const bool registeredPacket;
};

