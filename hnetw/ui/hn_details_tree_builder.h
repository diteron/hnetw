#pragma once

#include <packet/hn_packet.h>
#include <packet/proto_headers.h>
#include "qtmodels/hn_info_node.h"

class HnDetailsTreeBuilder {
public:
    HnDetailsTreeBuilder();
    ~HnDetailsTreeBuilder();

    HnInfoNode* buildDetailsTree(HnPacket* packet);
};
