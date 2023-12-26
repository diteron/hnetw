#pragma once

#include "qtmodels/hn_packet_list_model.h"

class HnPacketList : public QTreeView {

    Q_OBJECT

public:
    HnPacketList(HnPacketListModel* model, QWidget* parent = nullptr);
    ~HnPacketList();

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected) override;
};
