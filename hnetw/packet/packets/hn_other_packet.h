#pragma once

#include "hn_packet.h"

class HnOtherPacket : public HnPacket {
public:
    HnOtherPacket(int id);
    ~HnOtherPacket();

private:
    static const bool registeredPacket;
};

