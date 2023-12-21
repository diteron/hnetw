#include <stdafx.h>
#include "hn_packet.h"

using namespace std::chrono;

HnPacket::HnPacket(int id, int type, uint8_t* rawData) 
    : id_(id), type_(type), rawData_(rawData)
{
    arrivalTime_ = system_clock::to_time_t(system_clock::now());
    ipv4Header_ = reinterpret_cast<ipv4_hdr*>(rawData_);
    ipv4HeaderLength_ = ipv4Header_->header_length * 4; // The length in the header is specified in 32-bit words (min 5, so 5 * 4 = 20 bytes)
}

HnPacket::~HnPacket()
{
    if (rawData_) delete rawData_;
}
