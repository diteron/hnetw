#include "stdafx.h"
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
