#pragma once
#include "hn_proto_tree.h"

class HnIcmpTree : public HnProtoTree {
public:
    HnIcmpTree(const HnPacket* packet, HnInfoNode* parent = nullptr);
    ~HnIcmpTree();

private:
    void buildDestUnreachableMsg(uint8_t* msgBlock, size_t msgBlockLen);
    void buildTimeExceededMsg(uint8_t* msgBlock, size_t msgBlockLen);
    void buildParamProblemMsg(uint8_t* msgBlock, size_t msgBlockLen);
    void buildRedirectMsg(uint8_t* msgBlock, size_t msgBlockLen);
    void buildEchoReqReplMsg(uint8_t* msgBlock, size_t msgBlockLen);
    void buildTimestampMsg(uint8_t* msgBlock, size_t msgBlockLen);
    void buildUnknownMsg();

    void buildEncapsPacketTree(uint8_t* ipHdrAndDataBlock, size_t ipHdrAndDataBlockLen);

    struct icmpHeaderFields_ {
        QString header =     "Internet Control Message Protocol";
        QString type =       "Type: ";
        QString code =       "Code: ";
        QString checksum =   "Checksum: ";
                             
        QString unused =     "Unused Part: ";
        QString ptr =        "Pointer: ";
        QString gway_addr =  "Gateway Internet Address: ";
        QString id =         "Identifier: ";
        QString seq_num =    "Sequence Number: ";
        QString orig_ts =    "Originate Timestamp: ";
        QString recv_ts =    "Receive Timestamp: ";
        QString transm_ts =  "Transmit Timestamp: ";

        QString unknown =    "Unknown message type or it is deprecated";

    } icmpHeaderFields;

    enum IcmpMessages {
        echoRepl,
        destUnreach = 3,
        redirect = 5,
        echoReq = 8,
        timeExceed = 11,
        paramProbl,
        timestamp,
        timestampRepl,
    };

    const int icmpCommonPartLen = sizeof(icmp_hdr);

    HnPacket* encapsPacket_ = nullptr;
    uint8_t* msgData_ = nullptr;

    static const bool registeredTree;
};

