#include "stdafx.h"
#include "hn_packet_list_row.h"

HnPacketListRow::HnPacketListRow()
{}

HnPacketListRow::~HnPacketListRow()
{}

const QString HnPacketListRow::columnString(int column) const
{
    if (column >= rowColumns_.count()) return QString();

    return rowColumns_.at(column);
}
