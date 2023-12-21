#pragma once

class HnPacketListRow {
public:
    HnPacketListRow();
    ~HnPacketListRow();

    const QString columnString(int column) const;

private:
    QStringList rowColumns_;
};

