#pragma once

#include "hn_details_tree.h"

class HnTcpTree : public HnDetailsTree {
public:
    HnTcpTree();
    ~HnTcpTree();

    HnInfoNode* buildPacketTree(HnPacket* packet, HnInfoNode* parent = nullptr) override;

private:
    void addTcpOptions(uint8_t* rawData, int rawDataSize, int ipHeaderLen, int tcpHeaderLen);
    HnInfoNode* createSackNode(uint8_t* rawData, int sackOffset);
    HnInfoNode* createTimestampsNode(uint8_t* rawData, int timestampsOffset);

    enum tcpOptions {
        endOfOptList,
        noOperation,
        maxSegSize,
        wndScale,
        sackPerm,
        sack,
        timestamps = 8
    };

    struct timestamp_ {
        uint8_t senderTimestamp : 4;
        uint8_t echoReplyTimestamp : 4;
    };

    struct tcpHeaderFields_ {
        QString header =          "Transmission Control Protocol";
        QString srcPort =         "Source Port: ";
        QString destPort =        "Destination Port: ";
        QString seqNumber =       "Sequence number: ";
        QString ackNumber =       "Acknowledgment number: ";
        QString dataOffset =      "Data Offset: ";
        QString reserved =        "Reserved: ";
        QString cwr_f =           "Congestion window reduced flag: ";
        QString ece_f =           "ECN-Echo flag: ";
        QString urg_f =           "Urgent flag: ";
        QString ack_f =           "Acknowledgment flag: ";
        QString psh_f =           "Push flag: ";
        QString rst_f =           "Reset flag: ";
        QString syn_f =           "SYN flag: ";
        QString fin_f =           "FIN flag: ";
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
};

