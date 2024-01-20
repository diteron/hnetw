#include <stdafx.h>
#include <ui/utils/hn_converter.h>

#include "hn_proto_tree_factory.h"
#include "hn_igmp_tree.h"

const bool HnIgmpTree::registeredTree = HnProtoTreeFactory::instance()->
                                        registerProtoTree(HnPacket::IGMP, treeBuilder<HnIgmpTree>);

HnIgmpTree::HnIgmpTree(const HnPacket* packet, HnInfoNode* parent)
    : HnProtoTree(packet, parent)
{
    int ipHeaderLen = packet->ipv4Header()->header_length * 4;
    // Consider it is IGMPv2 
    igmp_v2_hdr* igmpHeader = reinterpret_cast<igmp_v2_hdr*>(const_cast<uint8_t*>(packet->rawData() + ipHeaderLen));
    // But we need to check IGMP version
    int igmpVersion = getIgmpVersion(packet->length(), ipHeaderLen, igmpHeader);

    switch (igmpVersion) {
        case 1:
            createIgmpTree(igmpHeader, parent, 1);
            break;
        case 2:
            createIgmpTree(igmpHeader, parent);
            break;
        case 3:
            createIgmpV3Tree(ipHeaderLen, packet, parent);
            break;
        default:
            break;
    }
}

HnIgmpTree::~HnIgmpTree()
{}

int HnIgmpTree::getIgmpVersion(size_t packetLen, int ipHeaderLen, struct igmp_v2_hdr* igmpHeader)
{
    const int igmpv2HeaderLen = 8;

    if (igmpHeader->type == membshipRep_v1)             // Type 0x12 can appear only in IGMPv1
        return 1;
    else if (packetLen > ipHeaderLen + igmpv2HeaderLen) // IGMPv1 and v2 header length = 8 bytes, IGMPv3 length always > 8
        return 3;
    else
        return 2;
}

void HnIgmpTree::createIgmpTree(struct igmp_v2_hdr* igmpHeader, HnInfoNode* parent, int version)
{
    rootNode_ = new HnInfoNode(igmpHeaderFields.header + QString::number(version), parent);

    if (version == 1) {
        struct igmp_v1_hdr* igmpV1Hdr = reinterpret_cast<igmp_v1_hdr*>(igmpHeader);

        QString versionVal =       QString::number(igmpV1Hdr->version);
        QString typeV1Val =        "0x" + QString::number(igmpV1Hdr->type, 16);
        QString unusedVal =        QString::number(igmpV1Hdr->unused);

        rootNode_->addChild(new HnInfoNode(igmpHeaderFields.version + versionVal));
        rootNode_->addChild(new HnInfoNode(igmpHeaderFields.msg_type + typeV1Val));
        rootNode_->addChild(new HnInfoNode(igmpHeaderFields.unused + unusedVal));
    }
    else {
        QString msgTypeVal =       "0x" + QString::number(igmpHeader->type, 16);
        double respTimeInSec =     static_cast<double>(igmpHeader->resp_time / 10);
        QString maxRespTimeVal =   QString::number(respTimeInSec, 10, 1) + " sec";

        rootNode_->addChild(new HnInfoNode(igmpHeaderFields.msg_type + msgTypeVal));
        rootNode_->addChild(new HnInfoNode(igmpHeaderFields.max_resp_time + maxRespTimeVal));
    }

    QString checksumVal =       "0x" + QString::number(ntohs(igmpHeader->checksum), 16);
    QString groupAddrVal =      HnConverter::uint32ToIpString(igmpHeader->group_addr);
    rootNode_->addChild(new HnInfoNode(igmpHeaderFields.checksum + checksumVal));
    rootNode_->addChild(new HnInfoNode(igmpHeaderFields.group_addr + groupAddrVal));
}

void HnIgmpTree::createIgmpV3Tree(int ipHeaderLen, const HnPacket* packet, HnInfoNode* parent)
{
    uint8_t igmpV3Type = *(reinterpret_cast<uint8_t*>(const_cast<uint8_t*>(packet->rawData() + ipHeaderLen)));

    if (igmpV3Type == membshipQry) {
        createMembshipQryMsg(ipHeaderLen, packet, parent);
    }
    else if (igmpV3Type == membshipRep_v3) {
        createMembshipRepMsg(ipHeaderLen, packet, parent);
    }
}

void HnIgmpTree::createMembshipQryMsg(int ipHeaderLen, const HnPacket* packet, HnInfoNode* parent)
{
    struct igmp_v3_membs_qry* membshipQryHdr = reinterpret_cast<igmp_v3_membs_qry*>(const_cast<uint8_t*>(packet->rawData() + ipHeaderLen));

    rootNode_ = new HnInfoNode(igmpV3MembshipQryFields.header, parent);

    QString msgTypeVal =      "0x" + QString::number(membshipQryHdr->common_v2_part.type, 16);
    double respTimeInSec =    static_cast<double>(membshipQryHdr->common_v2_part.resp_time / 10);
    QString maxRespTimeVal =  QString::number(respTimeInSec, 10, 1) + " sec";
    QString checksumVal =     "0x" + QString::number(ntohs(membshipQryHdr->common_v2_part.checksum), 16);
    QString groupAddrVal =    HnConverter::uint32ToIpString(membshipQryHdr->common_v2_part.group_addr);
    QString qrvVal =          QString::number(membshipQryHdr->qrv);
    QString sFlgVal =         QString::number(membshipQryHdr->s_flg);
    QString resvVal =         QString::number(membshipQryHdr->resv);
    QString qqicVal =         QString::number(membshipQryHdr->qqic);
    QString srcNumVal =       QString::number(ntohs(membshipQryHdr->src_num));      

    rootNode_->addChild(new HnInfoNode(igmpV3MembshipQryFields.msg_type + msgTypeVal));
    rootNode_->addChild(new HnInfoNode(igmpV3MembshipQryFields.max_resp_time + maxRespTimeVal));
    rootNode_->addChild(new HnInfoNode(igmpV3MembshipQryFields.checksum + checksumVal));
    rootNode_->addChild(new HnInfoNode(igmpV3MembshipQryFields.group_addr + groupAddrVal));
    rootNode_->addChild(new HnInfoNode(igmpV3MembshipQryFields.qrv + qrvVal));
    rootNode_->addChild(new HnInfoNode(igmpV3MembshipQryFields.s_flg + sFlgVal));
    rootNode_->addChild(new HnInfoNode(igmpV3MembshipQryFields.resv + resvVal));
    rootNode_->addChild(new HnInfoNode(igmpV3MembshipQryFields.qqic + qqicVal));
    rootNode_->addChild(new HnInfoNode(igmpV3MembshipQryFields.src_num + srcNumVal));

    if (membshipQryHdr->src_num > 3) {
        rootNode_->addChild(createSourcesTree(packet, membshipQryHdr->src_num, ipHeaderLen));
    }
}

HnInfoNode* HnIgmpTree::createSourcesTree(const HnPacket* packet, int srcNum, int ipHeaderLen)
{
    const int igmpV3WithoutSrcsLen = 12;
    
    int sourcesOffset = ipHeaderLen + igmpV3WithoutSrcsLen;
    uint8_t* sourcesBlock = const_cast<uint8_t*>(packet->rawData() + sourcesOffset);

    HnInfoNode* sourcesHdr = new HnInfoNode("Sources");

    uint32_t srcAddrValue = 0;
    int currentSrcOffset = 0;
    for (int i = 0; i < srcNum; ++i) {
        srcAddrValue = ntohl(*(reinterpret_cast<uint32_t*>(sourcesBlock + currentSrcOffset)));
        QString srcAddrIpValue = HnConverter::uint32ToIpString(srcAddrValue);
        sourcesHdr->addChild(new HnInfoNode(igmpV3MembshipQryFields.src_addr +
                                            QString::number(i + 1) + ": " + srcAddrIpValue));
        currentSrcOffset += 4;
    }

    return sourcesHdr;
}

void HnIgmpTree::createMembshipRepMsg(int ipHeaderLen, const HnPacket* packet, HnInfoNode* parent)
{
    // Size of the membership report message header without records in bytes 
    const int membshipRepHeaderLen = 8;

    struct igmp_v3_membs_rep* membshipRepHdr = reinterpret_cast<igmp_v3_membs_rep*>(const_cast<uint8_t*>(packet->rawData() +
                                                                                                         ipHeaderLen));
    rootNode_ = new HnInfoNode(igmpV3MembshipRepFields.header, parent);

    QString msgTypeVal =       "0x" + QString::number(membshipRepHdr->type, 16);
    QString reserved8Val =     QString::number(membshipRepHdr->type, 16);
    QString checksumVal =      "0x" + QString::number(ntohs(membshipRepHdr->checksum), 16);
    QString reserved16Val =    QString::number(ntohs(membshipRepHdr->reserved16));
    QString groupRecNumVal =   QString::number(ntohs(membshipRepHdr->group_rec_num));

    rootNode_->addChild(new HnInfoNode(igmpV3MembshipRepFields.msg_type + msgTypeVal));
    rootNode_->addChild(new HnInfoNode(igmpV3MembshipRepFields.reserved8 + reserved8Val));
    rootNode_->addChild(new HnInfoNode(igmpV3MembshipRepFields.checksum + checksumVal));
    rootNode_->addChild(new HnInfoNode(igmpV3MembshipRepFields.reserved16 + reserved16Val));
    rootNode_->addChild(new HnInfoNode(igmpV3MembshipRepFields.group_rec_num + groupRecNumVal));

    uint8_t* rawData = const_cast<uint8_t*>(packet->rawData());
    HnInfoNode* groupRecords = createGroupRecordsTree(rawData + ipHeaderLen + membshipRepHeaderLen,
                                                      ntohs(membshipRepHdr->group_rec_num));
    rootNode_->addChild(groupRecords);
}

HnInfoNode* HnIgmpTree::createGroupRecordsTree(uint8_t* groupRecordsBlock, int groupRecordsNum)
{
    HnInfoNode* groupRecordsRoot = new HnInfoNode("Group Records");

    int readBlockSize = 0;
    for (int i = 0; i < groupRecordsNum; ++i) {
        readBlockSize = createGroupRecord(groupRecordsBlock + readBlockSize, groupRecordsRoot);
    }

    return groupRecordsRoot;
}

int HnIgmpTree::createGroupRecord(uint8_t* recordBlock, HnInfoNode* recordsRoot)
{
    const int recordHeaderLen = 8; // Size of the record without sources and aux data in bytes 
    struct igmp_v3_group_rec* groupRecord = reinterpret_cast<igmp_v3_group_rec*>(recordBlock);

    QString recTypeVal =       QString::number(groupRecord->rec_type);
    QString auxDataLenVal =    QString::number(groupRecord->aux_data_len);
    QString srcNumVal =        QString::number(ntohs(groupRecord->src_num));
    QString mcastAddrVal =     HnConverter::uint32ToIpString(groupRecord->mcast_addr);
    
    HnInfoNode* recordRoot = new HnInfoNode("Group Record: " + mcastAddrVal);

    recordRoot->addChild(new HnInfoNode(groupRecordFields.rec_type + recTypeVal));
    recordRoot->addChild(new HnInfoNode(groupRecordFields.aux_data_len + auxDataLenVal));
    recordRoot->addChild(new HnInfoNode(groupRecordFields.src_num + srcNumVal));
    recordRoot->addChild(new HnInfoNode(groupRecordFields.mcast_addr + mcastAddrVal));

    if (groupRecord->src_num > 0) {
        HnInfoNode* sources = addSourcesAddresses(recordBlock + recordHeaderLen,
                                                  ntohs(groupRecord->src_num));
        recordRoot->addChild(sources);
    }

    recordsRoot->addChild(recordRoot);

    int recordSize = recordHeaderLen + groupRecord->src_num * 4 + groupRecord->aux_data_len * 4;
    return recordSize;
}

HnInfoNode* HnIgmpTree::addSourcesAddresses(uint8_t* sourcesBlock, int sourcesNum)
{
    HnInfoNode* sourcesRoot = new HnInfoNode("Sources");

    int sourceOffset = 0;
    uint32_t currentSourceIp = 0;
    for (int i = 0; i < sourcesNum; ++i) {
        currentSourceIp = ntohl(*(reinterpret_cast<uint32_t*>(sourcesBlock + sourceOffset)));
        QString currenSrcIp = HnConverter::uint32ToIpString(currentSourceIp);
        sourcesRoot->addChild(new HnInfoNode("Source: " + currenSrcIp));
        sourceOffset += 4;
    }

    return sourcesRoot;
}
