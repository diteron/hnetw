#pragma once

#include "proto_headers.h"

class HnPacket {
public:
    HnPacket(int id);
    ~HnPacket();

    enum packetTypes {
        IPv4,
        UDP,
        TCP,
    };



private:
    int id_;
    int type_;
    std::time_t time_;
    struct ipv4Header ipv4Header_;
    uint8_t* rawData_ = nullptr;
};

