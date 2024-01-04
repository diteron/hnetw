#include <stdafx.h>
#include <packet/hn_packet_factory.h>

#include "hn_icmp_packet.h"


const bool HnIcmpPacket::registeredPacket = HnPacketFactory::instance()->
                                            registerPacket(HnPacket::ICMP, packetBuilder<HnIcmpPacket>);

HnIcmpPacket::HnIcmpPacket(int id) : HnPacket(id)
{
    type_ = HnPacket::ICMP;
    typeString_ = "ICMP";
}

HnIcmpPacket::~HnIcmpPacket()
{}
