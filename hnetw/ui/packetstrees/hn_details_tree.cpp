#include <stdafx.h>
#include "hn_details_tree.h"

HnDetailsTree::HnDetailsTree()
{}

HnDetailsTree::~HnDetailsTree()
{}

const HnInfoNode* HnDetailsTree::rootNode() const
{
    return rootNode_;
}
