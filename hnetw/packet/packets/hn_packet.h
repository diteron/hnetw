#pragma once

#include <packet/proto_headers.h>

class HnPacket {
public:
    HnPacket();
    HnPacket(int id);
    virtual ~HnPacket();

    enum PacketTypes {
        Undefined = -1,
        ICMP = 1,
        IGMP = 2,
        TCP = 6,
        UDP = 17,
    };
    
    void setPacketData(uint8_t* rawData, int rawDataLen);
    void setArrivalTime(std::clock_t arrivalTime);

    const int id() const;
    std::string typeString() const;
    const int type() const;
    const std::clock_t arrivalTime() const;
    const ipv4_hdr* ipv4Header() const;
    const uint8_t* rawData() const;
    const int length() const;

protected:
    template <typename derivedPacket>
    static HnPacket* packetBuilder(int id) { return new derivedPacket(id); }

    int id_ = 0;
    int type_ = Undefined;
    std::string typeString_ = "Undefined";
    std::clock_t arrivalTime_ = 0;
    ipv4_hdr* ipv4Header_ = nullptr;
    int ipv4HeaderLength_ = 0;
    uint8_t* rawData_ = nullptr;
    int rawDataLen_ = 0;
};