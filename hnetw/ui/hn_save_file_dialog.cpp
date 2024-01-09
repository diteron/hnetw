#include "stdafx.h"
#include "hn_save_file_dialog.h"

HnSaveFileDialog::HnSaveFileDialog(QWidget* parent) 
    : QDialog(parent)
{
    this->setFixedSize(300, 150);
    layout_ = new QFormLayout(this);

    saveLabel_ = new QLabel("Do you want to save the captured packets?", this);
    layout_->addWidget(saveLabel_);

    spacer_ = new QSpacerItem(30, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);
    layout_->addItem(spacer_);

    addButtonBox();
}

HnSaveFileDialog::~HnSaveFileDialog()
{}

bool HnSaveFileDialog::isDiscarded() const
{
    return isDiscarded_;
}

int HnSaveFileDialog::exec()
{
    isDiscarded_ = true;
    QApplication::beep();
    return QDialog::exec();
}

void HnSaveFileDialog::addButtonBox()
{
    saveButton_ = new QPushButton("Save", buttonBox_);
    saveButton_->connect(saveButton_, &QPushButton::pressed, this, &HnSaveFileDialog::save);
    dontSaveButton_ = new QPushButton("Don't Save", buttonBox_);
    dontSaveButton_->connect(dontSaveButton_, &QPushButton::pressed, this, &HnSaveFileDialog::dontSave);
    cancelButton_ = new QPushButton("Cancel", buttonBox_);
    cancelButton_->connect(cancelButton_, &QPushButton::pressed, this, &HnSaveFileDialog::discard);

    buttonBox_ = new QDialogButtonBox(this);
    buttonBox_->addButton(saveButton_, QDialogButtonBox::AcceptRole);
    buttonBox_->addButton(dontSaveButton_, QDialogButtonBox::RejectRole);
    buttonBox_->addButton(cancelButton_, QDialogButtonBox::RejectRole);
    layout_->addWidget(buttonBox_);
}

void HnSaveFileDialog::save()
{
    isDiscarded_ = false;
    this->accept();
}

void HnSaveFileDialog::dontSave()
{
    isDiscarded_ = false;
    this->done(QDialog::Rejected);
}

void HnSaveFileDialog::discard()
{
    this->reject();
}
