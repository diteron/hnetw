#pragma once

#include <packet/packets/hn_packet.h>

class HnPacketListRow {
public:
    HnPacketListRow();
    HnPacketListRow(HnPacket* packet, long packetOffset);

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
    long packetOffset() const;
    int packetLength() const;

private:
    void setId(int id);
    void setTime(std::clock_t time);
    void setSourceIp(uint32_t srcIp);
    void setDestinationIp(uint32_t destIp);
    void setProtocolType(QString type);
    void setLength(int length);
    void setCaptureFileOffset(long packetOffset, int packetLen);

    long packetOffset_ = 0;
    int packetLen_ = 0;
    const static size_t columnsNum_ = 6;
    std::array<QString, columnsNum_> rowColumns_ = {};
};

