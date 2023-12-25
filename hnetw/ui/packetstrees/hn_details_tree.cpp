#include <stdafx.h>
#include "hn_details_tree.h"

HnDetailsTree::HnDetailsTree()
{}

HnDetailsTree::~HnDetailsTree()
{
    if (rootNode_) delete rootNode_;
}

const HnInfoNode* HnDetailsTree::rootNode() const
{
    return rootNode_;
}
