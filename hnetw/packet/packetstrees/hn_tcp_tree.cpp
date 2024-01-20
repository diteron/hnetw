#include <stdafx.h>
#include "hn_tcp_tree.h"

#include "hn_proto_tree_factory.h"
#include "hn_ip_tree.h"

const bool HnTcpTree::registeredTree = HnProtoTreeFactory::instance()->
                                       registerProtoTree(HnPacket::TCP, treeBuilder<HnTcpTree>);

HnTcpTree::HnTcpTree(const HnPacket* packet, HnInfoNode* parent) 
    : HnProtoTree(packet, parent)
{
    int ipHeaderLen = packet->ipv4Header()->header_length * 4;
    size_t tcpHeaderLen = packet->length() - ipHeaderLen;

    if (tcpHeaderLen >= 20) {
        tcp_hdr* tcpHeader = reinterpret_cast<tcp_hdr*>(const_cast<uint8_t*>(packet->rawData() + ipHeaderLen));

        unsigned int dataOffset = static_cast<unsigned int>(tcpHeader->data_offset);

        QString srcPortValue =    QString::number(ntohs(tcpHeader->src_port));
        QString destProtValue =   QString::number(ntohs(tcpHeader->dest_port));
        QString seqNumValue =     QString::number(ntohl(tcpHeader->seq_num));
        QString ackNumValue =     QString::number(ntohl(tcpHeader->ack_num));
        QString dataOffsValue =   QString::number(dataOffset) + " (" + QString::number(dataOffset * 4) + " bytes)";
        QString cwrFlagValue =    QString::number(static_cast<unsigned int>(tcpHeader->cwr_f));
        QString eceFlagValue =    QString::number(static_cast<unsigned int>(tcpHeader->ecn_echo_f));
        QString urgFlagValue =    QString::number(static_cast<unsigned int>(tcpHeader->urgent_f));
        QString ackFlagValue =    QString::number(static_cast<unsigned int>(tcpHeader->ack_f));
        QString pshFlagValue =    QString::number(static_cast<unsigned int>(tcpHeader->push_f));
        QString rstFlagValue =    QString::number(static_cast<unsigned int>(tcpHeader->reset_f));
        QString synFlagValue =    QString::number(static_cast<unsigned int>(tcpHeader->sync_f));
        QString finFlagValue =    QString::number(static_cast<unsigned int>(tcpHeader->finish_f));
        QString wndSizeValue =    QString::number(ntohs(tcpHeader->window));
        QString checksumValue =   "0x" + QString::number(ntohs(tcpHeader->checksum), 16);
        QString urgPrtValue =     QString::number(ntohs(tcpHeader->urgent_pointer));

        rootNode_ = new HnInfoNode(tcpHeaderFields.header, parent);

        rootNode_->addChild(new HnInfoNode(tcpHeaderFields.srcPort + srcPortValue));
        rootNode_->addChild(new HnInfoNode(tcpHeaderFields.destPort + destProtValue));
        rootNode_->addChild(new HnInfoNode(tcpHeaderFields.seqNumber + seqNumValue));
        rootNode_->addChild(new HnInfoNode(tcpHeaderFields.ackNumber + ackNumValue));
        rootNode_->addChild(new HnInfoNode(tcpHeaderFields.dataOffset + dataOffsValue));
        rootNode_->addChild(new HnInfoNode(tcpHeaderFields.cwr_f + cwrFlagValue));
        rootNode_->addChild(new HnInfoNode(tcpHeaderFields.ece_f + eceFlagValue));
        rootNode_->addChild(new HnInfoNode(tcpHeaderFields.urg_f + urgFlagValue));
        rootNode_->addChild(new HnInfoNode(tcpHeaderFields.ack_f + ackFlagValue));
        rootNode_->addChild(new HnInfoNode(tcpHeaderFields.psh_f + pshFlagValue));
        rootNode_->addChild(new HnInfoNode(tcpHeaderFields.rst_f + rstFlagValue));
        rootNode_->addChild(new HnInfoNode(tcpHeaderFields.syn_f + synFlagValue));
        rootNode_->addChild(new HnInfoNode(tcpHeaderFields.fin_f + finFlagValue));
        rootNode_->addChild(new HnInfoNode(tcpHeaderFields.windowSize + wndSizeValue));
        rootNode_->addChild(new HnInfoNode(tcpHeaderFields.checkSum + checksumValue));
        rootNode_->addChild(new HnInfoNode(tcpHeaderFields.urgPtr + urgPrtValue));

        addTcpOptions(const_cast<uint8_t*>(packet->rawData()), packet->length(), ipHeaderLen, tcpHeader->data_offset * 4);
    }
    else {      // ICMP packets can use headers of less than 20 bytes.
        tcp_hdr_8* tcpHeader = reinterpret_cast<tcp_hdr_8*>(const_cast<uint8_t*>(packet->rawData() + ipHeaderLen));

        QString srcPortValue = QString::number(ntohs(tcpHeader->src_port));
        QString destProtValue = QString::number(ntohs(tcpHeader->dest_port));
        QString seqNumValue = QString::number(ntohl(tcpHeader->seq_num));

        rootNode_ = new HnInfoNode(tcpHeaderFields.header, parent);

        rootNode_->addChild(new HnInfoNode(tcpHeaderFields.srcPort + srcPortValue));
        rootNode_->addChild(new HnInfoNode(tcpHeaderFields.destPort + destProtValue));
        rootNode_->addChild(new HnInfoNode(tcpHeaderFields.seqNumber + seqNumValue));
    }
}

HnTcpTree::~HnTcpTree()
{}

void HnTcpTree::addTcpOptions(uint8_t* rawData, size_t rawDataSize, int ipHeaderLen, int tcpHeaderLen)
{
    const int minTcpHeaderLen = 20;
    if (tcpHeaderLen <= minTcpHeaderLen) return;             // There are no options in this header

    int optionsOffset = ipHeaderLen + minTcpHeaderLen;
    uint8_t currOption = rawData[optionsOffset];
    uint8_t currOptionLen = 0;

    HnInfoNode* optionsHeader = new HnInfoNode(tcpHeaderFields.options);

    while (optionsOffset < rawDataSize) {
        switch (currOption) {
            case endOfOptList:
            {
                optionsHeader->addChild(new HnInfoNode(tcpHeaderFields.endOfOpt_opt));
                ++optionsOffset;
                break;
            }
            case noOperation:
            {
                optionsHeader->addChild(new HnInfoNode(tcpHeaderFields.noOperation_opt));
                ++optionsOffset;
                break;
            }
            case maxSegSize:
            {
                currOptionLen = rawData[++optionsOffset];

                ++optionsOffset;
                int value = ntohs(*(reinterpret_cast<uint16_t*>(rawData + optionsOffset)));
                QString maxSegSizeVal = QString::number(value);

                optionsHeader->addChild(new HnInfoNode(tcpHeaderFields.maxSegSize_opt + maxSegSizeVal));
                optionsOffset += currOptionLen - 2;
                break;
            }
            case wndScale:
            {
                currOptionLen = rawData[++optionsOffset];

                ++optionsOffset;
                QString wndScaleVal = QString::number(*(reinterpret_cast<uint8_t*>(rawData + optionsOffset)));
                optionsHeader->addChild(new HnInfoNode(tcpHeaderFields.windowScale_opt + wndScaleVal));
                optionsOffset += currOptionLen - 2;
                break;
            }
            case sackPerm:
            {
                currOptionLen = rawData[++optionsOffset];

                ++optionsOffset;
                optionsHeader->addChild(new HnInfoNode(tcpHeaderFields.sackPerm_opt));
                break;
            }
            case sack:
            {
                currOptionLen = rawData[++optionsOffset];

                ++optionsOffset;
                optionsHeader->addChild(createSackNode(rawData, optionsOffset, currOptionLen));
                optionsOffset += currOptionLen - 2;
                break;
            }
            case timestamps:
            {
                currOptionLen = rawData[++optionsOffset];

                ++optionsOffset;
                createTimestampsNode(rawData, optionsOffset, currOptionLen);
                optionsOffset += currOptionLen - 2;
                break;
            }
            default:
                break;
        }

        if (optionsOffset < rawDataSize)
            currOption = rawData[optionsOffset];
        else
            break;
    }

    rootNode_->addChild(optionsHeader);
}

HnInfoNode* HnTcpTree::createSackNode(uint8_t* rawData, int sackOffset, int sackLen)
{
    HnInfoNode* sackHeader = new HnInfoNode(tcpHeaderFields.sack_opt);
    int blocksCount = (sackLen - 2) / 8;    // Each SACK block specified as 32-bit begin/end pointers (max 4 blocks, in this case sackLen = 34)

    // We're already at the first SACK block
    uint32_t value = 0;
    for (int i = 0; i < blocksCount; ++i) {
        value = ntohl(*(reinterpret_cast<uint32_t*>(rawData + sackOffset)));
        QString blockLeftEdge = "Block " + QString::number(i + 1) + tcpHeaderFields.sackLeft_opt +
                                QString::number(value);
        sackOffset += 4;
        value = ntohl(*(reinterpret_cast<uint32_t*>(rawData + sackOffset)));
        QString blockRightEdge = "Block " + QString::number(i + 1) + tcpHeaderFields.sackRight_opt +
                                 QString::number(value);
        sackOffset += 4;
        sackHeader->addChild(new HnInfoNode(blockLeftEdge));
        sackHeader->addChild(new HnInfoNode(blockRightEdge));
    }

    return sackHeader;
}

HnInfoNode* HnTcpTree::createTimestampsNode(uint8_t* rawData, int timestampsOffset, int timestampsLen)
{
    // We're already at the timestamp data block
    timestamp_* timestamps = reinterpret_cast<timestamp_*>(rawData + timestampsOffset);

    HnInfoNode* timestampsHeader = new HnInfoNode(tcpHeaderFields.timestamp_opt);
    QString senderTimestamp = tcpHeaderFields.senderTime_opt +
                              QString::number(static_cast<unsigned int>(timestamps->senderTimestamp));
    QString echoReplyTimestamp = tcpHeaderFields.echoRepTime_opt + 
                                 QString::number(static_cast<unsigned int>(timestamps->echoReplyTimestamp));
    timestampsHeader->addChild(new HnInfoNode(senderTimestamp));
    timestampsHeader->addChild(new HnInfoNode(echoReplyTimestamp));

    return timestampsHeader;
}
