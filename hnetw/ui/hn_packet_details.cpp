#include <stdafx.h>
#include "hn_packet_details.h"

HnPacketDetails::HnPacketDetails(QWidget* parent) : QTreeView(parent)
{
    setHeaderHidden(true);
}

HnPacketDetails::~HnPacketDetails()
{}
