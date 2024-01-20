#pragma once

#include "hn_proto_tree.h"

class HnIgmpTree : public HnProtoTree {
public:
    HnIgmpTree(const HnPacket* packet, HnInfoNode* parent = nullptr);
    ~HnIgmpTree();

private:
    int getIgmpVersion(size_t packetLen, int ipHeaderLen, struct igmp_v2_hdr* igmpHeader);
    void createIgmpTree(struct igmp_v2_hdr* igmpHeader, HnInfoNode* parent, int version = 2);
    void createIgmpV3Tree(int ipHeaderLen, const HnPacket* packet, HnInfoNode* parent);
    
    void createMembshipQryMsg(int ipHeaderLen, const HnPacket* packet, HnInfoNode* parent);
    HnInfoNode* createSourcesTree(const HnPacket* packet, int srcNum, int ipHeaderLen);
    
    void createMembshipRepMsg(int ipHeaderLen, const HnPacket* packet, HnInfoNode* parent);
    HnInfoNode* createGroupRecordsTree(uint8_t* groupRecordsBlock, int groupRecordsNum);
    int createGroupRecord(uint8_t* recordBlock, HnInfoNode* recordsRoot);
    HnInfoNode* addSourcesAddresses(uint8_t* sourcesBlock, int sourcesNum);

    enum MsgTypes {
        membshipQry = 0x11,
        membshipRep_v1,
        membshipRep_v2 = 0x16,
        leaveGroup,
        membshipRep_v3 = 0x22,
    };

    struct igmpHeaderFields_ {
        QString header =           "Internet Group Management Protocol Version ";

        // Only for IGMPv1
        QString version =          "Version: ";
        QString unused =           "Unused: ";
        
        QString msg_type =         "Message Type: ";
        QString max_resp_time =    "Max Resp Time: ";
        QString checksum =         "Checksum: ";
        QString group_addr =       "Group Address: ";
    } igmpHeaderFields;

    struct igmpV3MembshipQryFields_ {
        QString header =           "Internet Group Management Protocol Version 3";
        QString msg_type =         "Message Type: ";
        QString max_resp_time =    "Max Resp Time: ";
        QString checksum =         "Checksum: ";
        QString group_addr =       "Multicast Address: ";
        QString qrv =              "Querier's Robustness Variable: ";
        QString s_flg =            "Suppress Router-side Processing: ";
        QString resv =             "Reserved: ";
        QString qqic =             "Querier's Query Interval Code: ";
        QString src_num =          "Number of source addresses: ";
        QString src_addr =         "Source address ";
    } igmpV3MembshipQryFields;

    struct igmpV3MembshipRepFields_ {
        QString header =           "Internet Group Management Protocol Version 3";
        QString msg_type =         "Message Type: ";
        QString reserved8 =        "Reserved: ";
        QString checksum =         "Checksum: ";
        QString reserved16 =       "Reserved: ";
        QString group_rec_num =    "Number of Group Records: ";
    } igmpV3MembshipRepFields;

    struct groupRecordFields_ {
        QString header =           "Group Record ";
        QString rec_type =         "Record Type: ";
        QString aux_data_len =     "Aux Data Len: ";
        QString src_num =          "Number of Sources: ";
        QString mcast_addr =       "Multicast Address: ";
        QString src_addr =         "Source Address ";
    } groupRecordFields;

    static const bool registeredTree;
};

