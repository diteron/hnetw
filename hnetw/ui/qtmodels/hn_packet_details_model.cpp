#include <stdafx.h>

#include <ui/hn_details_tree_builder.h>
#include "hn_packet_details_model.h"

HnPacketDetailsModel::HnPacketDetailsModel(QObject* parent) 
    : QAbstractItemModel(parent)
{}

HnPacketDetailsModel::~HnPacketDetailsModel()
{
    delete rootNode_;
}

int HnPacketDetailsModel::columnCount(const QModelIndex & parent) const
{
    return 1;
}

QVariant HnPacketDetailsModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();

    HnInfoNode* node = nodeByIndex(index);
    
    switch (role) {
        case Qt::DisplayRole:
            return node->label();
        default:
            break;
    }

    return QVariant();
}

QModelIndex HnPacketDetailsModel::index(int row, int column, const QModelIndex& parent) const
{
    HnInfoNode* parentNode = rootNode_;
    if (parent.isValid())
        parentNode = nodeByIndex(parent);
    if (!parentNode)
        return QModelIndex();

    HnInfoNode* childNode = parentNode->childAt(row);
    if (!childNode)
        return QModelIndex();

    return createIndex(row, 0, static_cast<void*>(childNode));
}

QModelIndex HnPacketDetailsModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    HnInfoNode* parentNode = nodeByIndex(index)->parent();
    return indexFromNode(parentNode);
}

int HnPacketDetailsModel::rowCount(const QModelIndex& parent) const
{
    if (!rootNode_) return 0;

    if (parent.isValid()) {
        return nodeByIndex(parent)->childrenCount();
    }
    return rootNode_->childrenCount();
}

void HnPacketDetailsModel::setPacket(const HnPacket* packet)
{
    HnDetailsTreeBuilder treeBuilder;
    HnInfoNode* packetTree = treeBuilder.buildDetailsTree(packet);
    setRootNode(packetTree);
}

void HnPacketDetailsModel::setRootNode(HnInfoNode* node)
{
    beginResetModel();
    delete rootNode_;
    rootNode_ = node;
    endResetModel();
    if (!node) return;

    int rowCount = rootNode_->childrenCount();
    if (rowCount < 1) return;
    beginInsertRows(QModelIndex(), 0, rowCount - 1);
    endInsertRows();
}

void HnPacketDetailsModel::clear()
{
    beginResetModel();
    delete rootNode_;
    rootNode_ = nullptr;
    endResetModel();
}

HnInfoNode* HnPacketDetailsModel::nodeByIndex(const QModelIndex& index) const
{
    return static_cast<HnInfoNode*>(index.internalPointer());
}

QModelIndex HnPacketDetailsModel::indexFromNode(HnInfoNode* node) const
{
    if (!node)
        return QModelIndex();

    int row = node->rowNo();
    if (row < 0) return QModelIndex();

    return createIndex(row, 0, static_cast<void*>(node));
}
