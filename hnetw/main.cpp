#include "stdafx.h"
#include <mutex>

#include "hnetwork_app.h"
#include "ui/hn_main_window.h"
#include "network/hn_network.h"
#include "network/hn_host.h"
#include "capture/hn_packet_capturer.h"

int main(int argc, char* argv[])
{
    const int startWidth = 1220, startHeight = 650;
    const int minWidth = 900, minHeight = 510;

    if (HnNetwork::initialize() != HnNetwork::Success)
        return 1;

    HnetworkApp app(argc, argv);

    HnMainWindow mainWnd(startWidth, startHeight);    
    mainWnd.setMinimumSize(minWidth, minHeight);
    mainWnd.show();
    if (!mainWnd.setupNetwork()) {
        mainWnd.close();
        HnNetwork::shutdown();
        return 1;
    }

    int errCode = app.exec();
    HnNetwork::shutdown();

    return errCode;
}
