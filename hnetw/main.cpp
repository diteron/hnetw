#include "stdafx.h"
#include <mutex>
#include <QtWidgets/QApplication>

#include "ui/hn_main_window.h"
#include "network/hnetwork.h"
#include "network/hn_host.h"
#include "capture/hn_packet_capturer.h"

int main(int argc, char* argv[])
{
    const int startWidth = 1100, startHeight = 620;

    if (HNetwork::initialize() != HNetwork::Success)
        return 1;
   

    QApplication app(argc, argv);
    HnMainWindow mainWnd(startWidth, startHeight);
    mainWnd.show();
    mainWnd.startCapture();
    int errCode = app.exec();

    HNetwork::shutdown();
    return errCode;
}
