#include "stdafx.h"
#include "hn_packet.h"

HnPacket::HnPacket(int id) : id_(id)
{}

HnPacket::~HnPacket()
{
    if (rawData_) delete rawData_;
}
