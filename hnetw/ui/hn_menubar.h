#pragma once

#include "hn_interface_dialog.h"

class HnMenuBar : public QMenuBar {

    Q_OBJECT

public:
    HnMenuBar(int width, QWidget* parent = nullptr);
    ~HnMenuBar();

    void setInterfacesIp(std::vector<std::string> ipStrings);
    void showChangeInterfaceDialog();
    void deselectInterfaceIp();

private:
    void createFileSubmenu();
    void createCaptureSubmenu();
    void addSubmenu(QMenu** submenu, const QString& title);
    void addSubmenuAction(QWidget* parent, QMenu* submenu, QAction** action, const QString& actionName,
                          const QString& title, const QString& shortcut);

    QMenu* menuFile_ = nullptr;
    QAction* fileOpenAction_ = nullptr;
    QAction* fileSaveAction_ = nullptr;

    QMenu* captureMenu_ = nullptr;
    HnInterfaceDialog* changeInterfaceDlg_ = nullptr;
    QAction* changeInterfaceAction_ = nullptr;

    std::vector<std::string> interfacesIpStrings_;

public slots:
    void handleOpenFile();
    void handleSaveFile();
    void handleInterfaceChange();

signals:
    void openFileTriggered(QString fname);
    void saveFileTriggered(QString fname);
    void interfaceIdChanged(int id);
};
