#include <stdafx.h>

#include "hn_packet_list_model.h"

HnPacketListModel::HnPacketListModel(QObject* parent) : QAbstractItemModel(parent)
{
    packetsRows_.reserve(100'000);
    newPacketsRows_.reserve(1'000);
}

HnPacketListModel::~HnPacketListModel()
{
    if (!packetsRows_.isEmpty()) {
        qDeleteAll(packetsRows_);
        packetsRows_.clear();
    }
    if (!newPacketsRows_.isEmpty()) {
        qDeleteAll(newPacketsRows_);
        newPacketsRows_.clear();
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
    if (!row)
        return QVariant();
    const HnPacket* rowPacket = row->packet();
    if (!rowPacket)
        return QVariant();

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

//void HnPacketListModel::processPacket(HnPacket* packet)
//{
//    HnPacketListRow* newRow = new HnPacketListRow(packet);
//    packetsRows_.append(newRow);
//}

HnPacketListRow* HnPacketListModel::rowByIndex(const QModelIndex& index) const
{
    return static_cast<HnPacketListRow*>(index.internalPointer());
}

void HnPacketListModel::processPacket(HnPacket* packet)
{
    appendPacket(packet);
}

void HnPacketListModel::appendPacket(HnPacket* packet)
{
    HnPacketListRow* newRow = new HnPacketListRow(packet);
    newPacketsRows_ << newRow;
    if (newPacketsRows_.count() < 2) {
        QTimer::singleShot(0, this, &HnPacketListModel::insertNewRows);
    }
}

void HnPacketListModel::insertNewRows()
{
    int lastRowPos = static_cast<int>(packetsRows_.count());
    if (newPacketsRows_.count() > 0) {
        beginInsertRows(QModelIndex(), lastRowPos, lastRowPos + static_cast<int>(newPacketsRows_.count()));
        for (HnPacketListRow* newRow : newPacketsRows_) {
            packetsRows_ << newRow;
        }
        endInsertRows();
        newPacketsRows_.resize(0);
    }
}

