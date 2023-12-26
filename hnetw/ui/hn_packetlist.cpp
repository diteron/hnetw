#include <stdafx.h>

#include "hn_packetlist.h"
#include "qtmodels/hn_packet_list_row.h"

HnPacketList::HnPacketList(HnPacketListModel* model, QWidget* parent) : QTreeView(parent)
{
    setItemsExpandable(false);
    setRootIsDecorated(false);
    setUniformRowHeights(true);     // Fixes performance issues when displaying a large number of packets
    
    setModel(model);

    int fontHeight = fontMetrics().height();
    setMinimumSize(fontHeight, fontHeight);
    setStyleSheet("QTreeView { color: black; font: 10pt 'Consolas'; }");
    setColumnWidth(HnPacketListRow::id,          70);
    setColumnWidth(HnPacketListRow::time,        150);
    setColumnWidth(HnPacketListRow::source,      180);
    setColumnWidth(HnPacketListRow::destination, 180);
    setColumnWidth(HnPacketListRow::protocol,    100);
}

HnPacketList::~HnPacketList()
{}

void HnPacketList::mousePressEvent(QMouseEvent * event)
{
    setAutoScroll(false);
    QTreeView::mousePressEvent(event);
    setAutoScroll(true);
}

void HnPacketList::mouseMoveEvent(QMouseEvent* event)
{}

void HnPacketList::selectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    QTreeView::selectionChanged(selected, deselected);
}
