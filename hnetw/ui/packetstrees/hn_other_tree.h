#pragma once

#include "hn_proto_tree.h"

class HnOtherTree : public HnProtoTree {
public:
    HnOtherTree(const HnPacket* packet, HnInfoNode* parent = nullptr);
    ~HnOtherTree();
};

