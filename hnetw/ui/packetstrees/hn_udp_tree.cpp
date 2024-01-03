#include <stdafx.h>

#include "hn_proto_tree_factory.h"
#include "hn_udp_tree.h"

const bool HnUdpTree::registeredTree = HnProtoTreeFactory::instance()->
                                       registerProtoTree(HnPacket::UDP, treeBuilder<HnUdpTree>);

HnUdpTree::HnUdpTree(const HnPacket* packet, HnInfoNode* parent)
    : HnProtoTree(packet, parent)
{
    // Get UDP header from raw data
    int ipHeaderLen = packet->ipv4Header()->header_length * 4;
    udp_hdr* udpHeader = reinterpret_cast<udp_hdr*>(const_cast<uint8_t*>(packet->rawData() + ipHeaderLen));

    rootNode_ = new HnInfoNode(udpHeaderFields.header, parent);
    QString srcPortVal = QString::number(ntohs(udpHeader->src_port));
    QString destPortVal = QString::number(ntohs(udpHeader->dest_port));
    QString lengthVal = QString::number(ntohs(udpHeader->udp_packet_length));
    QString checksumVal = "0x" + QString::number(ntohs(udpHeader->checksum), 16);

    rootNode_ = new HnInfoNode(udpHeaderFields.header, parent);

    rootNode_->addChild(new HnInfoNode(udpHeaderFields.srcPort + srcPortVal));
    rootNode_->addChild(new HnInfoNode(udpHeaderFields.destPort + destPortVal));
    rootNode_->addChild(new HnInfoNode(udpHeaderFields.length + lengthVal));
    rootNode_->addChild(new HnInfoNode(udpHeaderFields.checksum + checksumVal));
}

HnUdpTree::~HnUdpTree()
{}
