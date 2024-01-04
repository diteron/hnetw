#pragma once

#include "hn_proto_tree.h"

class HnTcpTree : public HnProtoTree {
public:
    HnTcpTree(const HnPacket* packet, HnInfoNode* parent = nullptr);
    ~HnTcpTree();

private:
    void addTcpOptions(uint8_t* rawData, int rawDataSize, int ipHeaderLen, int tcpHeaderLen);
    HnInfoNode* createSackNode(uint8_t* rawData, int sackOffset, int sackLen);
    HnInfoNode* createTimestampsNode(uint8_t* rawData, int timestampsOffset, int timestampsLen);

    enum TcpOptions {
        endOfOptList,
        noOperation,
        maxSegSize,
        wndScale,
        sackPerm,
        sack,
        timestamps = 8
    };

    struct timestamp_ {
        uint8_t echoReplyTimestamp : 4;     // TODO: Check bits order
        uint8_t senderTimestamp : 4;
    };

    struct tcpHeaderFields_ {
        QString header =          "Transmission Control Protocol";
        QString srcPort =         "Source Port: ";
        QString destPort =        "Destination Port: ";
        QString seqNumber =       "Sequence Number: ";
        QString ackNumber =       "Acknowledgment Number: ";
        QString dataOffset =      "Data Offset: ";
        QString reserved =        "Reserved: ";
        QString cwr_f =           "Congestion Window Reduced Flag: ";
        QString ece_f =           "ECN-Echo Flag: ";
        QString urg_f =           "Urgent Flag: ";
        QString ack_f =           "Acknowledgment Flag: ";
        QString psh_f =           "Push Flag: ";
        QString rst_f =           "Reset Flag: ";
        QString syn_f =           "Syn Flag: ";
        QString fin_f =           "Fin Flag: ";
        QString windowSize =      "Window Size: ";
        QString checkSum =        "Checksum: ";
        QString urgPtr =          "Urgent Pointer: ";
        QString options =         "Options";
        QString endOfOpt_opt =    "End Of Options List";
        QString noOperation_opt = "No Operation";
        QString maxSegSize_opt =  "Maximum Segment Size: ";
        QString windowScale_opt = "Window Scale: ";
        QString sackPerm_opt =    "Selective Acknowledgment Permitted";
        QString sack_opt =        "Selective Acknowledgment";
        QString sackLeft_opt =    " Left Edge: ";   // Block number is displayed first
        QString sackRight_opt =   " Right Edge: ";  // same
        QString timestamp_opt =   "Timestamp and echo of previous timestamp";
        QString senderTime_opt =  "Sender Timestamp: ";
        QString echoRepTime_opt = "Echo Reply Timestamp: ";
    } tcpHeaderFields;

    static const bool registeredTree;
};

