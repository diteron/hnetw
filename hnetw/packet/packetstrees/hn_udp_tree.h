#pragma once

#include "hn_proto_tree.h"

class HnUdpTree : public HnProtoTree {
public:
    HnUdpTree(const HnPacket* packet, HnInfoNode* parent = nullptr);
    ~HnUdpTree();

private:
    struct udpHeaderFields_ {
        QString header =        "User Datagram Protocol";
        QString srcPort =       "Source Port: ";
        QString destPort =      "Destination Port: ";
        QString length =        "Length: ";
        QString checksum =      "Checksum: ";
    } udpHeaderFields;

    static const bool registeredTree;
};

