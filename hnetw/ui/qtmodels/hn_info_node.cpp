#include <stdafx.h>
#include "hn_info_node.h"

HnInfoNode::HnInfoNode(QString label, HnInfoNode* parent) 
    : label_(label), parent_(parent)
{}

HnInfoNode::~HnInfoNode()
{
    if (!children_.isEmpty()) qDeleteAll(children_);
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
