#include <stdafx.h>
#include "hn_packet_details_model.h"

HnPacketDetailsModel::HnPacketDetailsModel(QObject* parent) 
    : QAbstractItemModel(parent)
{}

HnPacketDetailsModel::~HnPacketDetailsModel()
{}

int HnPacketDetailsModel::columnCount(const QModelIndex & parent) const
{
    return 1;
}

QVariant HnPacketDetailsModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();

    return QVariant();
}

QModelIndex HnPacketDetailsModel::index(int row, int column, const QModelIndex& parent) const
{
    return QModelIndex();
}

QModelIndex HnPacketDetailsModel::parent(const QModelIndex& index) const
{
    return QModelIndex();
}

int HnPacketDetailsModel::rowCount(const QModelIndex& parent) const
{
    return 0;
}

void HnPacketDetailsModel::setRootNode(HnInfoNode* node)
{
    beginResetModel();
    if (rootNode_) delete rootNode_;
    rootNode_ = node;
    endResetModel();
    if (!node) return;

    int rowCount = rootNode_->childrenCount();
    if (rowCount < 1) return;
    beginInsertRows(QModelIndex(), 0, rowCount - 1);
    endInsertRows();
}
