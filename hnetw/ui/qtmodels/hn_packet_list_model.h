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
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void appendRow(HnPacketListRow* row);
    const HnPacket* packetAt(int index) const;

    void clear();

public slots:
    void insertNewRows();

private:
    HnPacketListRow* rowByIndex(const QModelIndex& index) const;

    const int columnCount_ = 6;
    const QStringList headerColumns_ = {
        "No",
        "Time",
        "Source",
        "Destination",
        "Protocol",
        "Length"
    };

    QVector<HnPacketListRow*> packetsRows_;
    QVector<HnPacketListRow*> visibleRows_;
    QVector<HnPacketListRow*> newPacketsRows_;

    mutable std::mutex mutex_;
    std::condition_variable cond_var_;
    std::atomic<bool> modelIsChanging_ = false;
};
