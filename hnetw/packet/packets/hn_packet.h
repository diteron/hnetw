#pragma once

#include <packet/proto_headers.h>

class HnPacket {
public:
    HnPacket();
    HnPacket(int id = -1);
    virtual ~HnPacket();

    enum PacketTypes {
        Undefined = -1,
        ICMP = 1,
        IGMP = 2,
        TCP = 6,
        UDP = 17,
    };
    
    void setPacketData(uint8_t* rawData, size_t rawDataLen);
    void setArrivalTime(std::clock_t arrivalTime);

    int id() const;
    std::string typeString() const;
    int type() const;
    std::clock_t arrivalTime() const;
    const ipv4_hdr* ipv4Header() const;
    const uint8_t* rawData() const;
    size_t length() const;

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
    size_t rawDataLen_ = 0;
};
