#pragma once

#include "hn_info_node.h"

class HnProtoTree {
public:
    HnProtoTree(const HnPacket* packet, HnInfoNode* parent = nullptr);
    virtual ~HnProtoTree();

    HnInfoNode* rootNode() const;

protected:
    template <typename derivedTree>
    static HnProtoTree* treeBuilder(const HnPacket* packet, HnInfoNode* parent = nullptr) {
        return new derivedTree(packet, parent);
    }

    const HnPacket* packet_ = nullptr;
    HnInfoNode* parent_ = nullptr;

    HnInfoNode* rootNode_ = nullptr;
};
