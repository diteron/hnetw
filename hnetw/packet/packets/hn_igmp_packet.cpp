#include <stdafx.h>
#include <packet/hn_packet_factory.h>
#include "hn_igmp_packet.h"

const bool HnIgmpPacket::registeredPacket = HnPacketFactory::instance()->
                                            registerPacket(HnPacket::IGMP, packetBuilder<HnIgmpPacket>);

HnIgmpPacket::HnIgmpPacket(int id) : HnPacket(id)
{
    type_ = HnPacket::IGMP;
    typeString_ = "IGMP";
}

HnIgmpPacket::~HnIgmpPacket()
{}
