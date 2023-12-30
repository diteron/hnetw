#include <stdafx.h>

#include "hn_packet_list_model.h"

HnPacketListModel::HnPacketListModel(QObject* parent) : QAbstractItemModel(parent)
{}

HnPacketListModel::~HnPacketListModel()
{
    qDeleteAll(packetsRows_);
    packetsRows_.clear();
    visibleRows_.clear();
    newPacketsRows_.clear();
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
    if (row >= visibleRows_.count() || row < 0 || column >= columnCount_)
        return QModelIndex();

    HnPacketListRow* plistRow = visibleRows_[row];

    return createIndex(row, column, plistRow);
}

QModelIndex HnPacketListModel::parent(const QModelIndex& index) const
{
    return QModelIndex();   // All rows are not expandable
}

int HnPacketListModel::rowCount(const QModelIndex& parent) const
{
    return static_cast<int>(visibleRows_.count());
}

QVariant HnPacketListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && section < columnCount_) {
        switch (role) {
        case Qt::DisplayRole:
            return QVariant::fromValue(headerColumns_[section]);
        default:
            break;
        }
    }
    return QVariant();
}

HnPacketListRow* HnPacketListModel::rowByIndex(const QModelIndex& index) const
{
    return static_cast<HnPacketListRow*>(index.internalPointer());
}

void HnPacketListModel::appendRow(HnPacketListRow* row)
{
    std::unique_lock<std::mutex> lock(mutex_);
    while (modelIsChanging_.load()) {
        cond_var_.wait(lock);
    }

    packetsRows_.append(row);
    newPacketsRows_.append(row);
    if (newPacketsRows_.count() < 2) {
        // Queue rows insertion on next update (insertNewRows will be executed on the main GUI thread)
        QTimer::singleShot(0, this, &HnPacketListModel::insertNewRows);
    }
}

const HnPacket* HnPacketListModel::packetAt(int index) const
{
    return visibleRows_.at(index)->packet();
}

void HnPacketListModel::clear()
{
    std::lock_guard<std::mutex> lock(mutex_);
    modelIsChanging_ = true;

    beginResetModel();
    qDeleteAll(packetsRows_);
    packetsRows_.resize(0);
    visibleRows_.resize(0);
    newPacketsRows_.resize(0);
    endResetModel();

    modelIsChanging_ = false;
    cond_var_.notify_one();
}

void HnPacketListModel::insertNewRows()
{
    std::lock_guard<std::mutex> lock(mutex_);
    modelIsChanging_ = true;

    int lastRowPos = static_cast<int>(visibleRows_.count());
    if (newPacketsRows_.count() > 0) {
        beginInsertRows(QModelIndex(), lastRowPos, lastRowPos + static_cast<int>(newPacketsRows_.count()));
        for (HnPacketListRow* row : newPacketsRows_) {
            visibleRows_.append(row);
        }
        endInsertRows();
        newPacketsRows_.resize(0);
    }

    modelIsChanging_ = false;
    cond_var_.notify_one();
}
