#include <stdafx.h>
#include <ui/utils/hn_converter.h>
#include "hn_ip_tree.h"

HnIpTree::HnIpTree(const HnPacket* packet, HnInfoNode* parent) 
    : packet_(packet), parent_(parent)
{
     // Calculate 13-bit fragment offset field value
    unsigned int fragOffsetCombined = (packet->ipv4Header()->fragment_offset_5 << 8) | packet->ipv4Header()->fragment_offset_8;
    unsigned int headerLen = static_cast<unsigned int>(packet->ipv4Header()->header_length);

    rootNode_ = new HnInfoNode(ipHeaderFields.header, parent);
    QString ipVersionValue =    QString::number(static_cast<unsigned int>(packet->ipv4Header()->version));
    QString headerLenValue =    QString::number(headerLen) + " (" + QString::number(headerLen * 4) + " bytes)";
    QString dscpValue =         QString::number(static_cast<unsigned int>(packet->ipv4Header()->dscp));
    QString ecnValue =          QString::number(static_cast<unsigned int>(packet->ipv4Header()->ecn));
    QString totalLenValue =     QString::number(ntohs(packet->ipv4Header()->total_length));
    QString idValue =           QString::number(ntohs(packet->ipv4Header()->id));
    QString resZeroFlgValue =   QString::number(static_cast<unsigned int>(packet->ipv4Header()->reserved_zero_f));
    QString dFragFlgValue =     QString::number(static_cast<unsigned int>(packet->ipv4Header()->dont_fragment_f));
    QString mFragFlgValue =     QString::number(static_cast<unsigned int>(packet->ipv4Header()->more_fragment_f));
    QString fragOffsetValue =   QString::number(fragOffsetCombined);
    QString ttlValue =          QString::number(static_cast<unsigned int>(packet->ipv4Header()->time_to_live));
    QString protocolValue =     packet->typeString().c_str();
    QString checkSumValue =     "0x" + QString::number(ntohs(packet->ipv4Header()->checksum), 16);
    QString srcIpValue =        HnConverter::uint32ToIpString(packet->ipv4Header()->src_address);
    QString destIpValue =       HnConverter::uint32ToIpString(packet->ipv4Header()->dest_address);

    rootNode_->addChild(new HnInfoNode(ipHeaderFields.version + ipVersionValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.headerLength + headerLenValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.dscp + dscpValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.ecn + ecnValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.totalLength + totalLenValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.id + idValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.reservedZero_f + resZeroFlgValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.dontFragment_f + dFragFlgValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.moreFragments_f + mFragFlgValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.fragmentOffset + fragOffsetValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.ttl + ttlValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.protocol + protocolValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.checksum + checkSumValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.srcIp + srcIpValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.destIp + destIpValue));
}

HnIpTree::~HnIpTree()
{}

HnInfoNode* HnIpTree::rootNode() const
{
    return rootNode_;
}

HnInfoNode* HnIpTree::buildTree(const HnPacket* packet, HnInfoNode* parent)
{
    // Calculate 13-bit fragment offset field value
    unsigned int fragOffsetCombined = (packet->ipv4Header()->fragment_offset_5 << 8) | packet->ipv4Header()->fragment_offset_8;
    unsigned int headerLen = static_cast<unsigned int>(packet->ipv4Header()->header_length);

    rootNode_ = new HnInfoNode(ipHeaderFields.header, parent);
    QString ipVersionValue =    QString::number(static_cast<unsigned int>(packet->ipv4Header()->version));
    QString headerLenValue =    QString::number(headerLen) + " (" + QString::number(headerLen * 4) + " bytes)";
    QString dscpValue =         QString::number(static_cast<unsigned int>(packet->ipv4Header()->dscp));
    QString ecnValue =          QString::number(static_cast<unsigned int>(packet->ipv4Header()->ecn));
    QString totalLenValue =     QString::number(ntohs(packet->ipv4Header()->total_length));
    QString idValue =           QString::number(ntohs(packet->ipv4Header()->id));
    QString resZeroFlgValue =   QString::number(static_cast<unsigned int>(packet->ipv4Header()->reserved_zero_f));
    QString dFragFlgValue =     QString::number(static_cast<unsigned int>(packet->ipv4Header()->dont_fragment_f));
    QString mFragFlgValue =     QString::number(static_cast<unsigned int>(packet->ipv4Header()->more_fragment_f));
    QString fragOffsetValue =   QString::number(fragOffsetCombined);
    QString ttlValue =          QString::number(static_cast<unsigned int>(packet->ipv4Header()->time_to_live));
    QString protocolValue =     packet->typeString().c_str();
    QString checkSumValue =     "0x" + QString::number(ntohs(packet->ipv4Header()->checksum), 16);
    QString srcIpValue =        HnConverter::uint32ToIpString(packet->ipv4Header()->src_address);
    QString destIpValue =       HnConverter::uint32ToIpString(packet->ipv4Header()->dest_address);

    rootNode_->addChild(new HnInfoNode(ipHeaderFields.version + ipVersionValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.headerLength + headerLenValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.dscp + dscpValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.ecn + ecnValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.totalLength + totalLenValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.id + idValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.reservedZero_f + resZeroFlgValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.dontFragment_f + dFragFlgValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.moreFragments_f + mFragFlgValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.fragmentOffset + fragOffsetValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.ttl + ttlValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.protocol + protocolValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.checksum + checkSumValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.srcIp + srcIpValue));
    rootNode_->addChild(new HnInfoNode(ipHeaderFields.destIp + destIpValue));

    return rootNode_;
}
