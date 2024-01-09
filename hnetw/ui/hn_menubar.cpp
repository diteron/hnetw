#include <stdafx.h>

#include "hn_menubar.h"

HnMenuBar::HnMenuBar(int width, QWidget* parent) : QMenuBar(parent), interfacesIpStrings_()
{
    setGeometry(QRect(0, 0, width, 22));
    createFileSubmenu();
    createCaptureSubmenu();
}

HnMenuBar::~HnMenuBar()
{}

void HnMenuBar::setInterfacesIp(std::vector<std::string> ipStrings)
{
    interfacesIpStrings_ = ipStrings;
    changeInterfaceDlg_->setInterfacesIp(interfacesIpStrings_);
}

void HnMenuBar::showChangeInterfaceDialog()
{
    handleInterfaceChange();
}

void HnMenuBar::createFileSubmenu()
{
    addSubmenu(&menuFile_, "File");
    addSubmenuAction(this, menuFile_, &fileOpenAction_, "openAction",
                     "Open", "Ctrl+O");
    fileOpenAction_->connect(fileOpenAction_, &QAction::triggered,
                             this, &HnMenuBar::handleOpenFile);
    addSubmenuAction(this, menuFile_, &fileSaveAction_, "saveAction",
                     "Save", "Ctrl+S");
    fileSaveAction_->connect(fileSaveAction_, &QAction::triggered,
                             this, &HnMenuBar::handleSaveFile);
}

void HnMenuBar::createCaptureSubmenu()
{
    changeInterfaceDlg_ = new HnInterfaceDialog(this);

    addSubmenu(&captureMenu_, "Capture");
    addSubmenuAction(this, captureMenu_, &changeInterfaceAction_, "changeInterfaceAction",
                     "Change Interface", "Ctrl+I");
    changeInterfaceAction_->connect(changeInterfaceAction_, &QAction::triggered,
                                    this, &HnMenuBar::handleInterfaceChange);
}

void HnMenuBar::addSubmenu(QMenu** submenu, const QString& title) {
    *submenu = new QMenu(this);
    this->addAction((*submenu)->menuAction());
    (*submenu)->setTitle(title);
}

void HnMenuBar::addSubmenuAction(QWidget* parent, QMenu* submenu, QAction** action, const QString& actionName,
                                 const QString& title, const QString& shortcut) {
    (*action) = new QAction(parent);
    submenu->addAction(*action);
    (*action)->setText(title);
    (*action)->setShortcut(QKeySequence(shortcut));
}

void HnMenuBar::handleOpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, nullptr, nullptr, "Hnetwork File (*.hnw)");
    emit openFileTriggered(fileName);
}

void HnMenuBar::handleSaveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, nullptr, "Untitled.hnw", "Hnetwork File (*.hnw)");
    emit saveFileTriggered(fileName);
}

void HnMenuBar::handleInterfaceChange()
{
    if (changeInterfaceDlg_->exec() == QDialog::Accepted) {
        int newInterfaceIdx = changeInterfaceDlg_->selectedInterfaceId();
        emit interfaceIdChanged(newInterfaceIdx);
    }
}
