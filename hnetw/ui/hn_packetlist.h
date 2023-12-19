#pragma once

#include "../capture/hn_capturer_observer.h"

class HnPacketList : public QTreeView, public IHnCapturerObserver {

    Q_OBJECT

public:
    HnPacketList(QWidget* parent = nullptr);
    ~HnPacketList();

private:
    virtual void processPacket(HnPacket* packet) override;

};
