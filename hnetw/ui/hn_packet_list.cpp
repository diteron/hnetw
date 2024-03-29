#include <stdafx.h>

#include <packet/hn_packet_factory.h>
#include "hn_packet_list.h"
#include "qtmodels/hn_packet_list_row.h"

HnPacketList::HnPacketList(HnPacketListModel* model, QWidget* parent) : QTreeView(parent), listModel_(model)
{
    setItemsExpandable(false);
    setRootIsDecorated(false);
    setUniformRowHeights(true);     // Fixes performance issues when displaying a large number of packets
    
    setModel(listModel_);
    setSelectionMode(QAbstractItemView::SingleSelection);

    int fontHeight = fontMetrics().height();
    setMinimumSize(fontHeight, fontHeight);

    setStyleSheet("QTreeView { font: 10pt 'Cascadia Mono SemiLight'; }");
    setColumnWidth(HnPacketListRow::id,          100);
    setColumnWidth(HnPacketListRow::time,        170);
    setColumnWidth(HnPacketListRow::source,      200);
    setColumnWidth(HnPacketListRow::destination, 200);
    setColumnWidth(HnPacketListRow::protocol,    100);

    connect(verticalScrollBar(), &QScrollBar::actionTriggered, this, &HnPacketList::listScrolled);
}

HnPacketList::~HnPacketList()
{}

void HnPacketList::setDetailsView(HnPacketDetails* detailsView)
{
    packetDetailsView_ = detailsView;
}

void HnPacketList::setBytesView(HnByteView* bytesView)
{
    bytesView_ = bytesView;
}

void HnPacketList::setCaptureFile(HnCaptureFile* capFile)
{
    captureFile_ = capFile;
}

void HnPacketList::setCaptureInProgress(bool inProgress)
{
    captureInProgress_ = inProgress;
}

void HnPacketList::clear()
{
    selectionModel()->clear();
    packetDetailsView_->clear();
    bytesView_->clear();
    listModel_->clear();
}

void HnPacketList::mousePressEvent(QMouseEvent* event)
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

    QModelIndexList selectedRows = selectionModel()->selectedRows();
    if (selectedRows.count() <= 0) return;

    int selectedRow = selectedRows.at(0).row();     // Only one row can be selected (QAbstractItemView::SingleSelection)
    const HnPacketListRow* row = listModel_->rowAt(selectedRow);

    HnPacket* packetToDisplay = captureFile_->readPacket(row->packetOffset(), true);
    if (packetToDisplay == nullptr) return;

    packetDetailsView_->displayPacket(packetToDisplay);
    bytesView_->setRawData(packetToDisplay->rawData(), packetToDisplay->length());  // TODO: Move this function to HnByteView class
    bytesView_->printPacketBytes();

    delete packetToDisplay;
}

void HnPacketList::rowsInserted(const QModelIndex& parent, int start, int end)
{
    QTreeView::rowsInserted(parent, start, end);
    if (captureInProgress_ && tailAtEnd_) {
        scrollToBottom();
    }
}

void HnPacketList::listScrolled(int)
{
    tailAtEnd_ = (verticalScrollBar()->sliderPosition() >= verticalScrollBar()->maximum());
}
