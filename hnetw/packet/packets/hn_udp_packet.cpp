#include <stdafx.h>
#include <packet/hn_packet_factory.h>
#include "hn_udp_packet.h"

const bool HnUdpPacket::registeredPacket = HnPacketFactory::instance()->
                                           registerPacket(HnPacket::UDP, packetBuilder<HnUdpPacket>);

HnUdpPacket::HnUdpPacket(int id) : HnPacket(id)
{
    HnPacket::UDP;
    typeString_ = "UDP";
}

HnUdpPacket::~HnUdpPacket()
{}
