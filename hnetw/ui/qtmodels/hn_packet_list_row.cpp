#include <stdafx.h>

#include <string>
#include <sstream>
#include <ui/utils/hn_converter.h>

#include "hn_packet_list_row.h"

HnPacketListRow::HnPacketListRow()
{}

HnPacketListRow::HnPacketListRow(HnPacket* packet, size_t packetOffset)
{
    setId(packet->id());
    setTime(packet->arrivalTime());
    setSourceIp(packet->ipv4Header()->src_address);
    setDestinationIp(packet->ipv4Header()->dest_address);
    setProtocolType(static_cast<QString>(packet->typeString().c_str()));
    setLength(packet->length());
    setCaptureFileOffset(packetOffset, packet->length());
}

HnPacketListRow::~HnPacketListRow()
{}

const QString HnPacketListRow::columnString(int column)
{
    if (column >= columnsNum_ || column < 0) return QString();
    return rowColumns_.at(column);
}

void HnPacketListRow::setId(int id)
{
    rowColumns_[HnPacketListRow::id] = QString::number(id);
}

void HnPacketListRow::setTime(std::clock_t time)
{
    rowColumns_[HnPacketListRow::time] = HnConverter::time_tToString(time);
}

void HnPacketListRow::setSourceIp(uint32_t srcIp)
{
    rowColumns_[HnPacketListRow::source] = HnConverter::uint32ToIpString(srcIp);
}

void HnPacketListRow::setDestinationIp(uint32_t destIp)
{
    rowColumns_[HnPacketListRow::destination] = HnConverter::uint32ToIpString(destIp);
}


size_t HnPacketListRow::packetOffset() const
{
    return packetOffset_;
}

size_t HnPacketListRow::packetLength() const
{
    return packetLen_;
}

void HnPacketListRow::setProtocolType(QString type)
{
    rowColumns_[HnPacketListRow::protocol] = type;
}

void HnPacketListRow::setLength(size_t length)
{
    rowColumns_[HnPacketListRow::length] = QString::number(length);
}

void HnPacketListRow::setCaptureFileOffset(size_t packetOffset, size_t packetLen)
{
    packetOffset_ = packetOffset;
    packetLen_ = packetLen;
}
