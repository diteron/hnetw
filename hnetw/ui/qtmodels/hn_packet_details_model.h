#pragma once

#include "hn_info_node.h"

class HnPacketDetailsModel : public QAbstractItemModel {

    Q_OBJECT

public:
    HnPacketDetailsModel(QObject* parent = nullptr);
    ~HnPacketDetailsModel();

    virtual int	columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual QModelIndex	index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    virtual QModelIndex	parent(const QModelIndex& index) const override;
    virtual int	rowCount(const QModelIndex& parent = QModelIndex()) const override;

    void setPacket(const HnPacket* packet);
    void setRootNode(HnInfoNode* node);

    void clear();

private:
    HnInfoNode* nodeByIndex(const QModelIndex& index) const;
    QModelIndex indexFromNode(HnInfoNode* node) const;

    HnInfoNode* rootNode_ = nullptr;
};
