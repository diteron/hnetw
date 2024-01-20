#pragma once

#include <packet/packets/hn_packet.h>

class HnPacketListRow {
public:
    HnPacketListRow();
    HnPacketListRow(HnPacket* packet, size_t packetOffset);

    ~HnPacketListRow();

    enum ColumnsNames {
        id,
        time,
        source,
        destination,
        protocol,
        length
    };

    const QString columnString(int column);
    size_t packetOffset() const;
    size_t packetLength() const;

private:
    void setId(int id);
    void setTime(std::clock_t time);
    void setSourceIp(uint32_t srcIp);
    void setDestinationIp(uint32_t destIp);
    void setProtocolType(QString type);
    void setLength(size_t length);
    void setCaptureFileOffset(size_t packetOffset, size_t packetLen);

    size_t packetOffset_ = 0;
    size_t packetLen_ = 0;
    const static size_t columnsNum_ = 6;
    std::array<QString, columnsNum_> rowColumns_ = {};
};

