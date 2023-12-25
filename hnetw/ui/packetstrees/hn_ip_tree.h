#pragma once

#include <packet/proto_headers.h>
#include "../qtmodels/hn_info_node.h"

class HnIpTree {
public:
    HnIpTree();
    ~HnIpTree();

    HnInfoNode* buildPacketIpTree(HnPacket* packet);

private:
    QString getIpString(uint32_t ip);

    struct ipHeaderFields_ {
        QString header = "Internet Protocol Version 4";
        QString version = "IP Version: ";
        QString headerLength = "Header Length: ";
        QString dscp = "Differentiated Services Code Point: ";
        QString ecn = "Explicit Congestion Notification	";
        QString totalLength = "Total Length: ";
        QString id = "Id: ";
        QString reservedZero_f = "Reserver Zero Flag: ";
        QString dontFragment_f = "Don't Fragment Flag: ";
        QString moreFragments_f = "More Fragments Flag: ";
        QString fragmentOffset = "Fragment Offset: ";
        QString ttl = "Time To Live: ";
        QString protocol = "Protocol: ";
        QString checksum = "Header Checksum: ";
        QString srcIp = "Source IP: ";
        QString destIp = "Destination IP: ";
    } ipHeaderFields;
};
