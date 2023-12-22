#include <stdafx.h>

#include "packets/hn_other_packet.h"
#include "hn_packet_factory.h"

HnPacketFactory* HnPacketFactory::instance_ = nullptr;

HnPacketFactory* HnPacketFactory::instance()
{
    if (instance_ == nullptr) {
        instance_ = new HnPacketFactory();
    }
    return instance_;
}

bool HnPacketFactory::registerPacket(const int packetType, PacketBuilder const& builder)
{
    return registry_.insert(std::make_pair(packetType, builder)).second;
}

HnPacket* HnPacketFactory::buildPacket(int packetType, int id)
{
    auto it = registry_.find(packetType);
    if (it == registry_.end()) return new HnOtherPacket(id);
    return (it->second)(id);
}

std::vector<int> HnPacketFactory::getRegisteredPackets()
{
    std::vector<int> packets;
    for (const std::pair packet : registry_) {
        packets.push_back(packet.first);
    }
    return packets;
}
