#pragma once

#include <capture/hn_capture_file.h>
#include "qtmodels/hn_packet_list_model.h"
#include "hn_packet_details.h"
#include "hn_byte_view.h"

class HnPacketList : public QTreeView {

    Q_OBJECT

public:
    HnPacketList(HnPacketListModel* model, QWidget* parent = nullptr);
    ~HnPacketList();

    void setDetailsView(HnPacketDetails* detailsView);
    void setBytesView(HnByteView* bytesView);
    void setCaptureFile(HnCaptureFile* capFile);
    void setCaptureInProgress(bool inProgress);

    void clear();

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected) override;

private:
    HnPacketListModel* listModel_ = nullptr;

    bool captureInProgress_ = false;
    bool tailAtEnd_ = false;

    HnPacketDetails* packetDetailsView_ = nullptr;
    HnByteView* bytesView_ = nullptr;

    HnCaptureFile* captureFile_ = nullptr;

protected slots:
    void rowsInserted(const QModelIndex& parent, int start, int end) override;

private slots:
    void listScrolled(int);
};
