#include <stdafx.h>
#include "hn_packet.h"

using namespace std::chrono;

HnPacket::HnPacket()
{}

HnPacket::HnPacket(int id)
    : id_(id)
{}

HnPacket::~HnPacket()
{
    delete[] rawData_;
}

void HnPacket::setPacketData(uint8_t* rawData, size_t rawDataLen)
{
    rawData_ = rawData;
    rawDataLen_ = rawDataLen;
    ipv4Header_ = reinterpret_cast<ipv4_hdr*>(rawData_);
    ipv4HeaderLength_ = ipv4Header_->header_length * 4; // The length in the header is specified in 32-bit words (min 5, so 5 * 4 = 20 bytes)
}

void HnPacket::setArrivalTime(std::clock_t arrivalTime)
{
    arrivalTime_ = arrivalTime;
}

int HnPacket::id() const
{
    return id_;
}

std::string HnPacket::typeString() const
{
    return typeString_;
}

int HnPacket::type() const
{
    return type_;
}

std::clock_t HnPacket::arrivalTime() const
{
    return arrivalTime_;
}

const ipv4_hdr* HnPacket::ipv4Header() const
{
    return ipv4Header_;
}

const uint8_t* HnPacket::rawData() const
{
    return rawData_;
}

size_t HnPacket::length() const
{
    return rawDataLen_;
}
