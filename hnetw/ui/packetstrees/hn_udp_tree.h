#pragma once

#include "hn_details_tree.h"

class HnUdpTree : public HnDetailsTree {
public:
    HnInfoNode* buildPacketTree(HnPacket* packet, HnInfoNode* parent = nullptr) override;

private:
    struct udpHeaderFields_ {
        QString header =        "User Datagram Protocol";

    } udpHeaderFields;
};

