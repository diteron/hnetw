#include <stdafx.h>
#include <packet/hn_packet_factory.h>
#include <packet/packetstrees/hn_proto_tree_factory.h>
#include <packet/packetstrees/hn_ip_tree.h>
#include <ui/utils/hn_converter.h>

#include "hn_icmp_tree.h"

const bool HnIcmpTree::registeredTree = HnProtoTreeFactory::instance()->
                       registerProtoTree(HnPacket::ICMP, treeBuilder<HnIcmpTree>);

HnIcmpTree::HnIcmpTree(const HnPacket* packet, HnInfoNode* parent) 
    : HnProtoTree(packet, parent)
{
    int ipHeaderLen = packet->ipv4Header()->header_length * 4; 
    icmp_hdr* icmpHeader = reinterpret_cast<icmp_hdr*>(const_cast<uint8_t*>(packet->rawData() + ipHeaderLen));
    
    // Icmp common part
    QString typeVal =       QString::number(icmpHeader->type);
    QString codeVal =       QString::number(icmpHeader->code);
    QString checksumVal =   "0x" + QString::number(ntohs(icmpHeader->checksum), 16);

    rootNode_ = new HnInfoNode(icmpHeaderFields.header, parent);

    rootNode_->addChild(new HnInfoNode(icmpHeaderFields.type + typeVal));
    rootNode_->addChild(new HnInfoNode(icmpHeaderFields.code + codeVal));
    rootNode_->addChild(new HnInfoNode(icmpHeaderFields.checksum + checksumVal));

    const uint8_t* packetRawData = packet->rawData();
    int msgBlockOffset = ipHeaderLen + icmpCommonPartLen;
    size_t msgLen = packet->length() - ipHeaderLen - icmpCommonPartLen;

    msgData_ = new uint8_t[msgLen];
    std::memcpy(msgData_, packetRawData + msgBlockOffset, msgLen);

    int icmpMsgType = static_cast<int>(icmpHeader->type);

    switch (icmpMsgType) {
        case echoRepl:
            buildEchoReqReplMsg(msgData_, msgLen);
            break;
        case destUnreach:
            buildDestUnreachableMsg(msgData_, msgLen);
            break;
        case redirect:
            buildRedirectMsg(msgData_, msgLen);
            break;
        case echoReq:
            buildEchoReqReplMsg(msgData_, msgLen);
            break;
        case timeExceed:
            buildTimeExceededMsg(msgData_, msgLen);
            break;
        case paramProbl:
            buildParamProblemMsg(msgData_, msgLen);
            break;
        case timestamp:
            buildTimestampMsg(msgData_, msgLen);
            break;
        case timestampRepl:
            buildTimestampMsg(msgData_, msgLen);
            break;
        default:
            buildUnknownMsg();
            break;
    }
}

HnIcmpTree::~HnIcmpTree()
{
    delete[] msgData_;
    delete encapsPacket_;
}

void HnIcmpTree::buildDestUnreachableMsg(uint8_t* msgBlock, size_t msgBlockLen)
{
    icmp_dest_unreach* destUnreachHdr = reinterpret_cast<icmp_dest_unreach*>(msgBlock);

    QString unusedVal = QString::number(ntohs(destUnreachHdr->unused));
    rootNode_->addChild(new HnInfoNode(icmpHeaderFields.unused + unusedVal));

    size_t ipHdrAndDataBlockLen = msgBlockLen - sizeof(destUnreachHdr->unused);
    buildEncapsPacketTree(msgBlock + sizeof(destUnreachHdr->unused),
                          ipHdrAndDataBlockLen);
}

void HnIcmpTree::buildTimeExceededMsg(uint8_t* msgBlock, size_t msgBlockLen)
{
    icmp_time_exceed* timeExceedHdr = reinterpret_cast<icmp_time_exceed*>(msgBlock);

    QString unusedVal = QString::number(ntohs(timeExceedHdr->unused));
    rootNode_->addChild(new HnInfoNode(icmpHeaderFields.unused + unusedVal));

    size_t ipHdrAndDataBlockLen = msgBlockLen - sizeof(timeExceedHdr->unused);
    buildEncapsPacketTree(msgBlock + sizeof(timeExceedHdr->unused),
                          ipHdrAndDataBlockLen);
}

void HnIcmpTree::buildParamProblemMsg(uint8_t* msgBlock, size_t msgBlockLen)
{
    icmp_param_probl* paramProblHdr = reinterpret_cast<icmp_param_probl*>(msgBlock);

    QString ptrVal = QString::number(paramProblHdr->ptr);
    int unused8 = static_cast<int>(paramProblHdr->unused8);
    int unused16 = static_cast<int>(ntohs(paramProblHdr->unused16));
    int unusedComb = unused8 << 16 | unused16;
    QString unusedVal = QString::number(unusedComb);
    
    rootNode_->addChild(new HnInfoNode(icmpHeaderFields.ptr + ptrVal));
    rootNode_->addChild(new HnInfoNode(icmpHeaderFields.unused + unusedVal));

    int encapsPacketOffset = sizeof(paramProblHdr->ptr) + sizeof(paramProblHdr->unused8) +
                             sizeof(paramProblHdr->unused16);
    size_t ipHdrAndDataBlockLen = msgBlockLen - encapsPacketOffset;
    buildEncapsPacketTree(msgBlock + encapsPacketOffset, ipHdrAndDataBlockLen);

}

void HnIcmpTree::buildRedirectMsg(uint8_t* msgBlock, size_t msgBlockLen)
{
    icmp_redirect* redirectHdr = reinterpret_cast<icmp_redirect*>(msgBlock);

    QString gwayAddrVal = HnConverter::uint32ToIpString(ntohl(redirectHdr->ip));
    rootNode_->addChild(new HnInfoNode(icmpHeaderFields.gway_addr + gwayAddrVal));

    size_t ipHdrAndDataBlockLen = msgBlockLen - sizeof(redirectHdr->ip);
    buildEncapsPacketTree(msgBlock + sizeof(redirectHdr->ip),
                          ipHdrAndDataBlockLen);
}

void HnIcmpTree::buildEchoReqReplMsg(uint8_t* msgBlock, size_t msgBlockLen)
{
    icmp_echo_req_rep* echoHdr = reinterpret_cast<icmp_echo_req_rep*>(msgBlock);

    QString idVal =        QString::number(ntohs(echoHdr->id));
    QString seqNumVal =    QString::number(ntohs(echoHdr->seq_num));
    
    rootNode_->addChild(new HnInfoNode(icmpHeaderFields.id + idVal));
    rootNode_->addChild(new HnInfoNode(icmpHeaderFields.seq_num + seqNumVal));
}

void HnIcmpTree::buildTimestampMsg(uint8_t* msgBlock, size_t msgBlockLen)
{
    icmp_timestamp* timestampHdr = reinterpret_cast<icmp_timestamp*>(msgBlock);

    QString idVal =       QString::number(ntohs(timestampHdr->id));
    QString seqNumVal =   QString::number(ntohs(timestampHdr->seq_num));
    QString origVal =     HnConverter::msSinceMidntUtcToTimeString(ntohl(timestampHdr->origin_ts));
    QString recvVal =     HnConverter::msSinceMidntUtcToTimeString(ntohl(timestampHdr->recv_ts));
    QString transmVal =   HnConverter::msSinceMidntUtcToTimeString(ntohl(timestampHdr->transm_ts));

    rootNode_->addChild(new HnInfoNode(icmpHeaderFields.id + idVal));
    rootNode_->addChild(new HnInfoNode(icmpHeaderFields.seq_num + seqNumVal));
    rootNode_->addChild(new HnInfoNode(icmpHeaderFields.orig_ts + origVal));
    rootNode_->addChild(new HnInfoNode(icmpHeaderFields.recv_ts + recvVal));
    rootNode_->addChild(new HnInfoNode(icmpHeaderFields.transm_ts + transmVal));
}

void HnIcmpTree::buildUnknownMsg()
{
    rootNode_->addChild(new HnInfoNode(icmpHeaderFields.unknown));
}

void HnIcmpTree::buildEncapsPacketTree(uint8_t* ipHdrAndDataBlock, size_t ipHdrAndDataBlockLen)
{
    // Raw data for a pseudo-packet encapsulated in an ICMP packet
    // (IP header and at least 8 bytes of original datagram data) 
    uint8_t* encapsPacketData = new uint8_t[ipHdrAndDataBlockLen];
    std::memcpy(encapsPacketData, ipHdrAndDataBlock, ipHdrAndDataBlockLen);

    // Build encapsulated packet
    ipv4_hdr* ipHeader = reinterpret_cast<ipv4_hdr*>(ipHdrAndDataBlock);
    HnPacket* encapsPacket = HnPacketFactory::instance()->buildPacket(ipHeader->protocol, -1);
    encapsPacket->setPacketData(encapsPacketData, ipHdrAndDataBlockLen);

    HnIpTree* ipTree = new HnIpTree(encapsPacket);
    HnProtoTree* encapsPacketTree = HnProtoTreeFactory::instance()->buildTree(ipHeader->protocol,
                                                                              encapsPacket);
    rootNode_->addChild(ipTree->rootNode());
    rootNode_->addChild(encapsPacketTree->rootNode());
}
