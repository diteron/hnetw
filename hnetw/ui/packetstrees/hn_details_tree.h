#pragma once

#include "../qtmodels/hn_info_node.h"

class HnDetailsTree {
public:
    HnDetailsTree();
    virtual ~HnDetailsTree();

    virtual HnInfoNode* buildPacketTree(HnPacket* packet, HnInfoNode* parent = nullptr) = 0;
    const HnInfoNode* rootNode() const;

protected:
    HnInfoNode* rootNode_ = nullptr;
};
