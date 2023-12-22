#include <stdafx.h>
#include <packet/hn_packet_factory.h>
#include "hn_other_packet.h"

const bool HnOtherPacket::registeredPacket = HnPacketFactory::instance()->
                                             registerPacket(HnPacket::Undefined, packetBuilder<HnOtherPacket>);

HnOtherPacket::HnOtherPacket(int id) : HnPacket(id)
{
    type_ = HnPacket::Undefined;
    typeString_ = "Other";
}

HnOtherPacket::~HnOtherPacket()
{}

std::string HnOtherPacket::typeString() const
{
    return typeString_;
}
