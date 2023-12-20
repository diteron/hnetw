#include "stdafx.h"
#include <QtWidgets/QApplication>

#include "ui/hn_main_window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    const int startWidth = 1100, startHeight = 620;
    HnMainWindow mainWnd(startWidth, startHeight);
    mainWnd.show();
    int errCode = app.exec();
    return errCode;
}
