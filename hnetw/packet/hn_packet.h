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
    
    const int id() const { return id_; }
    const int type() const { return type_; }
    const std::time_t arrivalTime() const { return arrivalTime_; }
    const ipv4_hdr* ipv4Header() const { return ipv4Header_; }
    const uint8_t* rawData() const { return rawData_; }

private:
    int id_;
    int type_;
    std::time_t arrivalTime_;
    ipv4_hdr* ipv4Header_ = nullptr;
    int ipv4HeaderLength_;
    uint8_t* rawData_ = nullptr;
    int rawDataLen_ = 0;
};
