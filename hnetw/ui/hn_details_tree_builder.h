#pragma once

#include <packet/packets/hn_packet.h>
#include <packet/proto_headers.h>
#include <packet/packetstrees/hn_info_node.h>

class HnDetailsTreeBuilder {
public:
    HnDetailsTreeBuilder();
    ~HnDetailsTreeBuilder();

    HnInfoNode* buildDetailsTree(const HnPacket* packet);
};
