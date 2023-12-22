#pragma once

#include <packet/hn_packet.h>

class HnUdpPacket : public HnPacket {
public:
    HnUdpPacket(int id);
    ~HnUdpPacket();

    std::string typeString() const override;

private:
    static const bool registeredPacket;
};
