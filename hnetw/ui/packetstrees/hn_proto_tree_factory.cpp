#include <stdafx.h>

#include "hn_other_tree.h"
#include "hn_proto_tree_factory.h"

HnProtoTreeFactory* HnProtoTreeFactory::instance_ = nullptr;

HnProtoTreeFactory* HnProtoTreeFactory::instance()
{
    if (instance_ == nullptr) {
        instance_ = new HnProtoTreeFactory();
    }
    return instance_;
}

bool HnProtoTreeFactory::registerProtoTree(const int packetType, ProtoTreeBuilder const& builder)
{
    return registry_.insert(std::make_pair(packetType, builder)).second;
}

HnProtoTree* HnProtoTreeFactory::buildTree(int proto, const HnPacket* packet, HnInfoNode* parent)
{
    auto it = registry_.find(proto);
    if (it == registry_.end()) return new HnOtherTree(packet);
    return (it->second)(packet, parent);
}

std::vector<int> HnProtoTreeFactory::getRegisteredTrees()
{
    std::vector<int> packets;
    for (const std::pair packet : registry_) {
        packets.push_back(packet.first);
    }
    return packets;
}
