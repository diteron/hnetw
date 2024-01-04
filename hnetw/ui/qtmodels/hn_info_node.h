#pragma once

#include <packet/packets/hn_packet.h>

class HnInfoNode {
public:
    HnInfoNode(QString label, HnInfoNode* parent = nullptr);
    ~HnInfoNode();

    void addChild(HnInfoNode* child);
    void setParent(HnInfoNode* parent);

    QString label() const;
    HnInfoNode* parent() const;
    const QVector<HnInfoNode*>& children() const;
    int childrenCount() const;
    bool isChild() const;

    HnInfoNode* childAt(int index) const;
    int rowNo() const;

private:
    QString label_;
    QVector<HnInfoNode*> children_;
    HnInfoNode* parent_ = nullptr;
};
