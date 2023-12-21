#pragma once

#include "proto_headers.h"

class HnPacket {
public:
    HnPacket(int id, int type, uint8_t* rawData, int rawDataLen, std::time_t arrivalTime);
    ~HnPacket();

    enum packetTypes {
        Undefined = -1,
        ICMP = 1,
        IGMP = 2,
        TCP = 6,
        UDP = 17,
    };
    
    const int id() const;
    const int type() const;
    const std::time_t arrivalTime() const;
    const ipv4_hdr* ipv4Header() const;
    const uint8_t* rawData() const;

private:
    int id_;
    int type_;
    std::time_t arrivalTime_;
    ipv4_hdr* ipv4Header_ = nullptr;
    int ipv4HeaderLength_;
    uint8_t* rawData_ = nullptr;
    int rawDataLen_ = 0;
};
