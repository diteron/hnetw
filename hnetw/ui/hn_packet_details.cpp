#include <stdafx.h>
#include "hn_packet_details.h"

HnPacketDetails::HnPacketDetails(QWidget* parent) : QTreeView(parent)
{
    setHeaderHidden(true);
    detailsViewModel_ = new HnPacketDetailsModel(this);
    setModel(detailsViewModel_);
    setStyleSheet("QTreeView { color: black; font: 10pt 'Cascadia Mono SemiLight'; }");
}

HnPacketDetails::~HnPacketDetails()
{}

void HnPacketDetails::displayPacket(const HnPacket * packet)
{
    detailsViewModel_->setPacket(packet);
}
