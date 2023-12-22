#pragma once

#include <map>
#include <unordered_map>
#include "hn_packet.h"

class HnPacketFactory {
public:
    typedef std::function<HnPacket* (int id)> PacketBuilder;

    static HnPacketFactory* instance();
    bool registerPacket(const int packetType, PacketBuilder const& builder);
    HnPacket* buildPacket(int packetType, int id);
    std::vector<int> getRegisteredPackets();

private:
    static HnPacketFactory* instance_;
    std::unordered_map<int, PacketBuilder> registry_;
};
