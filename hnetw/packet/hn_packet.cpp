#include <stdafx.h>
#include "hn_packet.h"

using namespace std::chrono;

HnPacket::HnPacket(int id, int type, uint8_t* rawData, int rawDataLen, std::time_t arrivalTime) 
    : id_(id),
      type_(type),
      rawData_(rawData),
      rawDataLen_(rawDataLen),
      arrivalTime_(arrivalTime)
{
    arrivalTime_ = system_clock::to_time_t(system_clock::now());
    ipv4Header_ = reinterpret_cast<ipv4_hdr*>(rawData_);
    ipv4HeaderLength_ = ipv4Header_->header_length * 4; // The length in the header is specified in 32-bit words (min 5, so 5 * 4 = 20 bytes)
}

HnPacket::~HnPacket()
{
    if (rawData_) delete rawData_;
}

const int HnPacket::id() const
{
    return id_;
}

const int HnPacket::type() const
{
    return type_;
}

const std::time_t HnPacket::arrivalTime() const
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
