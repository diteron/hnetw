#pragma once

#include "hn_packet.h"

class HnIgmpPacket : public HnPacket {
public:
    HnIgmpPacket(int id);
    ~HnIgmpPacket();

private:
    static const bool registeredPacket;
};

