#include "stdafx.h"

#include "hn_menubar.h"

HnMenuBar::HnMenuBar(int width, QWidget* parent) : QMenuBar(parent) {
    setGeometry(QRect(0, 0, width, 22));
    createFileSubmenu();
}

void HnMenuBar::handleOpenFile() 
{
    //QString fileName = QFileDialog::getOpenFileName(this, nullptr, nullptr, "Simple Paint File (*.smp)");
    //QFile file(fileName);
    //QDataStream stream(&file);
    //file.open(QIODevice::ReadOnly);

    //if (file.size() == 0) {
    //    file.close();
    //    return;
    //}

    //QVector<Shape*>& shapesList = SmpDrawer::getInstance()->getShapesList();
    //if (shapesList.size() > 0) { SmpDrawer::getInstance()->removeDrawnShapes(); }

    //deserializeShapesList(stream, shapesList);
    //file.close();
}

void HnMenuBar::handleSaveFile() 
{
    //QString fileName = QFileDialog::getSaveFileName(this, nullptr, "Untitled.smp", "Simple Paint File (*.smp)");
    //QFile file(fileName);
    //QDataStream stream(&file);
    //file.open(QIODevice::WriteOnly);

    //QVector<Shape*>& shapesList = SmpDrawer::getInstance()->getShapesList();

    //if (shapesList.isEmpty()) {
    //    file.close();
    //    return;
    //}

    //serializeShapesList(stream, shapesList);
    //file.close();
}

void HnMenuBar::createFileSubmenu() {
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
