#include <stdafx.h>
#include "hn_details_tree_builder.h"

#include <packet/packetstrees/hn_proto_tree_factory.h>
#include <packet/packetstrees/hn_ip_tree.h>

HnDetailsTreeBuilder::HnDetailsTreeBuilder()
{}

HnDetailsTreeBuilder::~HnDetailsTreeBuilder()
{}

HnInfoNode* HnDetailsTreeBuilder::buildDetailsTree(const HnPacket* packet)
{
    HnInfoNode* detailsTree = new HnInfoNode("ROOT");

    // Add IP tree
    HnIpTree* ipTree = new HnIpTree(packet);
    detailsTree->addChild(ipTree->rootNode());

    // Add other 3rd layer protocols trees and transport layer protocols trees
    HnProtoTree* protoTree = HnProtoTreeFactory::instance()->buildTree(packet->type(), packet);
    HnInfoNode* protoTreeRoot = protoTree->rootNode();
    detailsTree->addChild(protoTreeRoot);

    delete ipTree;
    delete protoTree;
    return detailsTree;
}
