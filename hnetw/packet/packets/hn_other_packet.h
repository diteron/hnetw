#pragma once

#include <packet/hn_packet.h>

class HnOtherPacket : public HnPacket {
public:
    HnOtherPacket(int id);
    ~HnOtherPacket();

    std::string typeString() const override;

private:
    static const bool registeredPacket;
};

