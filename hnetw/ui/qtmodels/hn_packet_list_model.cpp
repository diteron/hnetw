#include <stdafx.h>

#include "hn_packet_list_model.h"

HnPacketListModel::HnPacketListModel(QObject* parent) : QAbstractItemModel(parent)
{}

HnPacketListModel::~HnPacketListModel()
{
    if (!packetsRows_.isEmpty()) {
        qDeleteAll(packetsRows_);
        packetsRows_.clear();
    }
}

int HnPacketListModel::columnCount(const QModelIndex& parent) const
{
    return columnCount_;
}

QVariant HnPacketListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();

    HnPacketListRow* row = rowByIndex(index);
    if (!row) return QVariant();

    switch (role) {
        case Qt::DisplayRole:
        {
            int column = index.column();
            QString columnString = row->columnString(column);
            return columnString;
        }
        default:
            return QVariant();
    }
}

QModelIndex HnPacketListModel::index(int row, int column, const QModelIndex& parent) const
{
    if (row >= packetsRows_.count() || row < 0 || column >= columnCount_)
        return QModelIndex();

    HnPacketListRow* plistRow = packetsRows_[row];

    return createIndex(row, column, plistRow);
}

QModelIndex HnPacketListModel::parent(const QModelIndex& index) const
{
    return QModelIndex();   // All rows are not expandable
}

int HnPacketListModel::rowCount(const QModelIndex& parent) const
{
    return static_cast<int>(packetsRows_.count());
}

void HnPacketListModel::processPacket(HnPacket* packet)
{
    HnPacketListRow* newRow = new HnPacketListRow(packet);
    packetsRows_.append(newRow);
}

HnPacketListRow* HnPacketListModel::rowByIndex(const QModelIndex& index) const
{
    return static_cast<HnPacketListRow*>(index.internalPointer());
}
