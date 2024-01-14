#pragma once

#include <map>
#include <unordered_map>
#include "hn_proto_tree.h"

class HnProtoTreeFactory {
public:
    typedef std::function<HnProtoTree* (const HnPacket* packet, HnInfoNode* parent)> ProtoTreeBuilder;

    static HnProtoTreeFactory* instance();
    bool registerProtoTree(const int packetType, ProtoTreeBuilder const& builder);
    HnProtoTree* buildTree(int proto, const HnPacket* packet, HnInfoNode* parent = nullptr);
    std::vector<int> getRegisteredTrees();

private:
    static HnProtoTreeFactory* instance_;
    std::unordered_map<int, ProtoTreeBuilder> registry_;
};
