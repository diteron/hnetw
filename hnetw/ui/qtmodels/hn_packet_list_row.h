#pragma once

#include <packet/hn_packet.h>

class HnPacketListRow {
public:
    HnPacketListRow();
    HnPacketListRow(HnPacket* packet);
    HnPacketListRow(const HnPacketListRow& other);
    ~HnPacketListRow();

    enum columnsNames {
        id,
        time,
        source,
        destination,
        protocol,
        length
    };

    const QString columnString(int column);
    const HnPacket* packet() const;

private:
    void setId(int id);
    void setTime(std::time_t time);
    void setSourceIp(uint32_t srcIp);
    void setDestinationIp(uint32_t destIp);
    void setProtocolType(QString type);
    void setLength(int length);

    QString time_tToString(std::time_t time);
    QString ipToString(u_long ip);

    HnPacket* packet_ = nullptr;
    const static size_t columnsNum_ = 6;
    std::array<QString, columnsNum_> rowColumns_ = {};
};

