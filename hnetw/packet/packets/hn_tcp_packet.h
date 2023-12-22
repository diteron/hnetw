#pragma once

#include <packet/hn_packet.h>

class HnTcpPacket : public HnPacket {
public:
    HnTcpPacket(int id);
    ~HnTcpPacket();

    std::string typeString() const override;

private:
    static const bool registeredPacket;
};

