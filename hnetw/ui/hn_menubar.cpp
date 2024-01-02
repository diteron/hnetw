#include <stdafx.h>

#include "hn_menubar.h"

HnMenuBar::HnMenuBar(int width, QWidget* parent) : QMenuBar(parent), interfacesIpStrings_()
{
    setGeometry(QRect(0, 0, width, 22));
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

void HnMenuBar::handleInterfaceChange()
{
    if (changeInterfaceDlg_->exec() == QDialog::Accepted) {
        int newInterfaceIdx = changeInterfaceDlg_->selectedInterfaceId();
        emit interfaceIdChanged(newInterfaceIdx);
    }
}
