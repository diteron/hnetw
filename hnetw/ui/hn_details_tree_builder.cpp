#include <stdafx.h>
#include "hn_details_tree_builder.h"

#include "packetstrees/hn_ip_tree.h"
#include "packetstrees/hn_tcp_tree.h"
#include "packetstrees/hn_udp_tree.h"

HnDetailsTreeBuilder::HnDetailsTreeBuilder()
{}

HnDetailsTreeBuilder::~HnDetailsTreeBuilder()
{}

HnInfoNode* HnDetailsTreeBuilder::buildDetailsTree(const HnPacket* packet)
{
    HnInfoNode* detailsTree = new HnInfoNode("ROOT");
    detailsTree->addChild(HnIpTree().buildPacketTree(packet));

    switch (packet->type()) {
        case HnPacket::TCP: 
        {
            detailsTree->addChild(HnTcpTree().buildPacketTree(packet));
            break;
        }
        case HnPacket::UDP:
        {
            detailsTree->addChild(HnUdpTree().buildPacketTree(packet));
            break;
        }
        default:
            break;
    }

    return detailsTree;
}
