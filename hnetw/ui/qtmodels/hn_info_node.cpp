#include <stdafx.h>
#include "hn_info_node.h"

HnInfoNode::HnInfoNode(QString label, HnInfoNode* parent) 
    : label_(label), parent_(parent)
{}

HnInfoNode::~HnInfoNode()
{
    qDeleteAll(children_);
}

void HnInfoNode::addChild(HnInfoNode* child)
{
    child->setParent(this);
    children_.append(child);
}

void HnInfoNode::setParent(HnInfoNode* parent)
{
    parent_ = parent;
}

QString HnInfoNode::label() const
{
    return label_;
}

HnInfoNode* HnInfoNode::parent() const
{
    return parent_;
}

const QVector<HnInfoNode*>& HnInfoNode::children() const
{
    return children_;
}

int HnInfoNode::childrenCount() const
{
    return children_.count();
}

bool HnInfoNode::isChild() const
{
    return parent_;
}

HnInfoNode* HnInfoNode::childAt(int index) const
{
    return children_.at(index);
}

int HnInfoNode::rowNo() const
{
    if (!isChild()) return -1;

    return static_cast<int>(parent_->children_.indexOf(const_cast<HnInfoNode*>(this)));
}
