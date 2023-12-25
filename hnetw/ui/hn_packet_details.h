#pragma once

#include <packet/hn_packet.h>

class HnPacketDetails : public QTreeView {
public:
    HnPacketDetails(QWidget* parent = nullptr);
    ~HnPacketDetails();

    void displayPacket(HnPacket* packet);

private:


};
