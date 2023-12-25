#include "stdafx.h"
#include <QtWidgets/QApplication>

#include "ui/hn_main_window.h"
#include "network/hnetwork.h"

int main(int argc, char* argv[])
{
    if (HNetwork::initialize() != HNetwork::Success)
        return 1;

    QApplication app(argc, argv);
    const int startWidth = 1100, startHeight = 620;
    HnMainWindow mainWnd(startWidth, startHeight);
    mainWnd.show();
    int errCode = app.exec();
    HNetwork::shutdown();
    return errCode;
}
