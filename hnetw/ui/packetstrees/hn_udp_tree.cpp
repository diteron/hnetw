#include <stdafx.h>
#include "hn_udp_tree.h"

HnInfoNode* HnUdpTree::buildPacketTree(const HnPacket * packet, HnInfoNode* parent)
{
    if (packet->type() != HnPacket::UDP) return nullptr;

    // Get UDP header from raw data
    int ipHeaderLen = packet->ipv4Header()->header_length;
    udp_hdr* udpHeader = reinterpret_cast<udp_hdr*>(const_cast<uint8_t*>(packet->rawData() + ipHeaderLen));

    HnInfoNode* udpHeaderRoot = new HnInfoNode(udpHeaderFields.header);


    return udpHeaderRoot;
}
