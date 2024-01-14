#include <stdafx.h>
#include "hn_udp_tree.h"
#include "hn_other_tree.h"

HnOtherTree::HnOtherTree(const HnPacket* packet, HnInfoNode* parent)
    : HnProtoTree(packet, parent)
{
    rootNode_ = new HnInfoNode("Other", parent);
}

HnOtherTree::~HnOtherTree()
{}
