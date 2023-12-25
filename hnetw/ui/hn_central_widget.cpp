#include <stdafx.h>

#include "hn_central_widget.h"

HnCentralWidget::HnCentralWidget(QWidget* parent) : QWidget(parent)
{
    layout_ = new QVBoxLayout(this);
}

HnCentralWidget::~HnCentralWidget()
{}

void HnCentralWidget::addWidget(QWidget * widget)
{
    widget->setParent(this);
    layout_->addWidget(widget);
}


