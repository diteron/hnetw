#include "stdafx.h"
#include "hn_interface_dialog.h"

HnInterfaceDialog::HnInterfaceDialog(QWidget* parent) : QDialog(parent), interfacesIp_()
{
    this->setFixedSize(300, 150);
    layout_ = new QFormLayout(this);
    
    addChangeInterfaceRow();
    addSpacerItem();
    addButtonsBox();
}

HnInterfaceDialog::~HnInterfaceDialog()
{}

void HnInterfaceDialog::setInterfacesIp(const std::vector<std::string>& ipStrings)
{
    for (const auto& string : ipStrings) {
        interfacesIp_ << string.c_str();
    }
    interfacesCbox_->addItems(interfacesIp_);
}

int HnInterfaceDialog::selectedInterfaceId()
{
    return selectedInterfaceId_;
}

int HnInterfaceDialog::exec()
{
    interfacesCbox_->setCurrentIndex(selectedInterfaceId_);
    return QDialog::exec();
}

void HnInterfaceDialog::addChangeInterfaceRow()
{
    interfacesLabel_ = new QLabel("Select interface IP:   ", this);
    interfacesCbox_ = new QComboBox(this);
    interfacesCbox_->setFixedSize(150, 25);
    layout_->addRow(interfacesLabel_, interfacesCbox_);
    interfacesCbox_->setCurrentIndex(0);
}

void HnInterfaceDialog::addSpacerItem()
{
    spacer_ = new QSpacerItem(30, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);
    layout_->addItem(spacer_);
}

void HnInterfaceDialog::addButtonsBox()
{
    okButton_ = new QPushButton("OK", buttons_);
    okButton_->connect(okButton_, &QPushButton::pressed, this, &HnInterfaceDialog::setInterfaceId);
    cancelButton_ = new QPushButton("Cancel", buttons_);
    cancelButton_->connect(cancelButton_, &QPushButton::pressed, this, &QDialog::reject);

    buttons_ = new QDialogButtonBox(this);
    buttons_->addButton(okButton_, QDialogButtonBox::AcceptRole);
    buttons_->addButton(cancelButton_, QDialogButtonBox::RejectRole);
    layout_->addWidget(buttons_);
}

void HnInterfaceDialog::setInterfaceId() 
{
    selectedInterfaceId_ = interfacesCbox_->currentIndex();
    this->accept();
}
