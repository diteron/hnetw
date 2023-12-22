#include <stdafx.h>

#include <string>
#include <sstream>

#include "hn_packet_list_row.h"

HnPacketListRow::HnPacketListRow(HnPacket* packet)
{
    setId(packet->id());
    setTime(packet->arrivalTime());
    setSourceIp(packet->ipv4Header()->src_address);
    setDestinationIp(packet->ipv4Header()->dest_address);
    setProtocolType(static_cast<QString>(packet->typeString().c_str()));
    setLength(packet->length());
}

HnPacketListRow::~HnPacketListRow()
{}

const QString HnPacketListRow::columnString(int column) const
{
    if (column >= columnsNum_) return QString();

    return rowColumns_.at(column);
}

void HnPacketListRow::setId(int id)
{
    rowColumns_[HnPacketListRow::id] = QString::number(id);
}

void HnPacketListRow::setTime(std::time_t time)
{
    rowColumns_[HnPacketListRow::time] = time_tToString(time);
}

void HnPacketListRow::setSourceIp(uint32_t srcIp)
{
    rowColumns_[HnPacketListRow::source] = ipToString(static_cast<u_long>(srcIp));
}

void HnPacketListRow::setDestinationIp(uint32_t destIp)
{
    rowColumns_[HnPacketListRow::destination] = ipToString(static_cast<u_long>(destIp));
}

void HnPacketListRow::setProtocolType(QString type)
{
    rowColumns_[HnPacketListRow::protocol] = type;
}

void HnPacketListRow::setLength(int length)
{
    rowColumns_[HnPacketListRow::length] = QString::number(length);
}

QString HnPacketListRow::time_tToString(std::time_t time)       // Possible errors?
{
    std::stringstream strStream;
    std::time(&time);
    strStream << time;
    std::string str = strStream.str();
    QString qstr(str.c_str());
    return qstr;
}

QString HnPacketListRow::ipToString(u_long ip)
{
    char strIpBuffer[16];
    in_addr addr;
    addr.s_addr = ip;
    inet_ntop(AF_INET, &addr, strIpBuffer, 16);
    return QString(strIpBuffer);
}
