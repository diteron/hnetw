#include <stdafx.h>
#include "hn_proto_tree.h"

HnProtoTree::HnProtoTree(const HnPacket* packet, HnInfoNode* parent) 
    : packet_(packet), parent_(parent)
{}

HnProtoTree::~HnProtoTree()
{}

HnInfoNode* HnProtoTree::rootNode() const
{
    return rootNode_;
}
