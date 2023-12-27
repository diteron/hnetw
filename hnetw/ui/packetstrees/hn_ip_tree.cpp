#include <stdafx.h>
#include "hn_ip_tree.h"

HnIpTree::HnIpTree()
{}

HnIpTree::~HnIpTree()
{}

HnInfoNode* HnIpTree::buildPacketIpTree(const HnPacket* packet)
{
    // Calculate 13-bit fragment offset field value
    unsigned int fragOffsetCombined = (packet->ipv4Header()->fragment_offset_5 << 8) | packet->ipv4Header()->fragment_offset_8;
    unsigned int headerLen = static_cast<unsigned int>(packet->ipv4Header()->header_length);

    HnInfoNode* ipHeader = new HnInfoNode(ipHeaderFields.header);
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
    QString srcIpValue =        getIpString(packet->ipv4Header()->src_address);
    QString destIpValue =       getIpString(packet->ipv4Header()->dest_address);

    ipHeader->addChild(new HnInfoNode(ipHeaderFields.version + ipVersionValue, ipHeader));
    ipHeader->addChild(new HnInfoNode(ipHeaderFields.headerLength + headerLenValue, ipHeader));
    ipHeader->addChild(new HnInfoNode(ipHeaderFields.dscp + dscpValue, ipHeader));
    ipHeader->addChild(new HnInfoNode(ipHeaderFields.ecn + ecnValue, ipHeader));
    ipHeader->addChild(new HnInfoNode(ipHeaderFields.totalLength + totalLenValue, ipHeader));
    ipHeader->addChild(new HnInfoNode(ipHeaderFields.id + idValue, ipHeader));
    ipHeader->addChild(new HnInfoNode(ipHeaderFields.reservedZero_f + resZeroFlgValue, ipHeader));
    ipHeader->addChild(new HnInfoNode(ipHeaderFields.dontFragment_f + dFragFlgValue, ipHeader));
    ipHeader->addChild(new HnInfoNode(ipHeaderFields.moreFragments_f + mFragFlgValue, ipHeader));
    ipHeader->addChild(new HnInfoNode(ipHeaderFields.fragmentOffset + fragOffsetValue, ipHeader));
    ipHeader->addChild(new HnInfoNode(ipHeaderFields.ttl + ttlValue, ipHeader));
    ipHeader->addChild(new HnInfoNode(ipHeaderFields.protocol + protocolValue, ipHeader));
    ipHeader->addChild(new HnInfoNode(ipHeaderFields.checksum + checkSumValue, ipHeader));
    ipHeader->addChild(new HnInfoNode(ipHeaderFields.srcIp + srcIpValue, ipHeader));
    ipHeader->addChild(new HnInfoNode(ipHeaderFields.destIp + destIpValue, ipHeader));

    return ipHeader;
}

QString HnIpTree::getIpString(uint32_t ip)
{
    char strIpBuffer[16];
    in_addr addr;
    addr.s_addr = ip;
    inet_ntop(AF_INET, &addr, strIpBuffer, 16);
    return QString(strIpBuffer);
}
