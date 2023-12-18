#pragma once

#include "hn_central_widget.h"
#include "hn_menubar.h"

class HnMainWindow : public QMainWindow {
public:
    HnMainWindow(int startWidth, int startHeight, QWidget* parent = nullptr);
    ~HnMainWindow();

private:
    HnMenuBar* menuBar_ = nullptr;
    HnCentralWidget* centralWidget_ = nullptr;
};

