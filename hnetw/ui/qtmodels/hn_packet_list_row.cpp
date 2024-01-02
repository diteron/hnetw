#include <stdafx.h>

#include <string>
#include <sstream>

#include "hn_packet_list_row.h"

HnPacketListRow::HnPacketListRow()
{}

HnPacketListRow::HnPacketListRow(HnPacket* packet) : packet_(packet)
{
    setId(packet_->id());
    setTime(packet_->arrivalTime());
    setSourceIp(packet_->ipv4Header()->src_address);
    setDestinationIp(packet_->ipv4Header()->dest_address);
    setProtocolType(static_cast<QString>(packet_->typeString().c_str()));
    setLength(packet_->length());
}

HnPacketListRow::HnPacketListRow(const HnPacketListRow& other)
{
    packet_ = new HnPacket();
    *packet_ = *(other.packet_);
    this->rowColumns_ = other.rowColumns_;
}

HnPacketListRow::~HnPacketListRow()
{
    delete packet_;
}

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
    rowColumns_[HnPacketListRow::time] = time_tToString(time);
}

void HnPacketListRow::setSourceIp(uint32_t srcIp)
{
    rowColumns_[HnPacketListRow::source] = ipToString(static_cast<u_long>(srcIp));
}

const HnPacket* HnPacketListRow::packet() const
{
    return packet_;
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

QString HnPacketListRow::time_tToString(std::clock_t time)       // Possible errors?
{
    double timeInSeconds = static_cast<double>(time) / CLOCKS_PER_SEC;
    return QString::number(timeInSeconds, 10, 3);
}

QString HnPacketListRow::ipToString(u_long ip)
{
    char strIpBuffer[16] = "";
    in_addr addr;
    addr.s_addr = ip;
    inet_ntop(AF_INET, &addr, strIpBuffer, 16);
    return QString(strIpBuffer);
}
