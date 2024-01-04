#include "stdafx.h"
#include <mutex>
#include <QtWidgets/QApplication>

#include "ui/hn_main_window.h"
#include "network/hnetwork.h"
#include "network/hn_host.h"
#include "capture/hn_packet_capturer.h"

int main(int argc, char* argv[])
{
    const int startWidth = 1220, startHeight = 650;
    const int minWidth = 900, minHeight = 510;

    if (HNetwork::initialize() != HNetwork::Success)
        return 1;

    QApplication app(argc, argv);

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0) && defined(Q_OS_WIN)
    app.setStyle(QStyleFactory::create("fusion"));
#endif

    HnMainWindow mainWnd(startWidth, startHeight);
    if (!mainWnd.setupNetwork()) {
        HNetwork::shutdown();
        return 1;
    }
    
    mainWnd.setMinimumSize(minWidth, minHeight);
    mainWnd.show();

    int errCode = app.exec();

    HNetwork::shutdown();
    return errCode;
}
