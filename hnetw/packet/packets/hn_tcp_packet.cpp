#include <stdafx.h>
#include <packet/hn_packet_factory.h>
#include "hn_tcp_packet.h"

const bool HnTcpPacket::registeredPacket = HnPacketFactory::instance()->
                                           registerPacket(HnPacket::TCP, packetBuilder<HnTcpPacket>);

HnTcpPacket::HnTcpPacket(int id) : HnPacket(id)
{
    type_ = HnPacket::TCP;
    typeString_ = "TCP";
}

HnTcpPacket::~HnTcpPacket()
{}

std::string HnTcpPacket::typeString() const
{
    return typeString_;
}
