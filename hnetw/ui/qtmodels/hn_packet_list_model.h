#pragma once

#include <capture/hn_capturer_observer.h>
#include "hn_packet_list_row.h"

class HnPacketListModel : public QAbstractItemModel, public IHnCapturerObserver {

    Q_OBJECT

public:
    HnPacketListModel(QObject* parent = nullptr);
    ~HnPacketListModel();

    virtual int	columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual QModelIndex	index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    virtual QModelIndex	parent(const QModelIndex& index) const override;
    virtual int	rowCount(const QModelIndex& parent = QModelIndex()) const override;

    void appendPacket(HnPacket* packet);
    const HnPacket* packetAt(int index) const;

public slots:
    void insertNewRows();

private:
    HnPacketListRow* rowByIndex(const QModelIndex& index) const;
    void processPacket(HnPacket* packet) override;

    const int columnCount_ = 6;
    QVector<HnPacketListRow*> packetsRows_;
    QVector<HnPacketListRow*> newPacketsRows_;
};
