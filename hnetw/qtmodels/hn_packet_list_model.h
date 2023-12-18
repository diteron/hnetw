#pragma once

#include "hn_packet_list_row.h"

class HnPacketListModel : public QAbstractItemModel {

    Q_OBJECT

public:
    HnPacketListModel(QObject* parent = nullptr);
    ~HnPacketListModel();

    virtual int	columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual QModelIndex	index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    virtual QModelIndex	parent(const QModelIndex& index) const override;
    virtual int	rowCount(const QModelIndex& parent = QModelIndex()) const override;

    bool appendPacket();

private:
    HnPacketListRow* rowByIndex(const QModelIndex& index) const;


    const int columnCount_ = 6;
    QVector<HnPacketListRow*> packetsRows_;

};

