#pragma once

class HnMenuBar : public QMenuBar {

    Q_OBJECT

public:
    HnMenuBar(int width, QWidget* parent = nullptr);

public slots:
    void handleOpenFile();
    void handleSaveFile();

private:
    void createFileSubmenu();
    void addSubmenu(QMenu** submenu, const QString& title);
    void addSubmenuAction(QWidget* parent, QMenu* submenu, QAction** action, const QString& actionName,
                          const QString& title, const QString& shortcut);

    QMenu* menuFile_ = nullptr;
    QAction* fileOpenAction_ = nullptr;
    QAction* fileSaveAction_ = nullptr;
};
