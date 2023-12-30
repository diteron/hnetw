#pragma once

#include <packet/hn_packet.h>
#include "qtmodels/hn_packet_details_model.h"

class HnPacketDetails : public QTreeView {
public:
    HnPacketDetails(QWidget* parent = nullptr);
    ~HnPacketDetails();

    void displayPacket(const HnPacket* packet);
    void clear();

private:
    HnPacketDetailsModel* detailsViewModel_ = nullptr;
};
