#include "stdafx.h"
#include "hnetwork_app.h"

HnetworkApp::HnetworkApp(int& argc, char** argv)
    : QApplication(argc, argv)
{
#ifdef Q_OS_WIN
    setSystemColorScheme();
#endif
}

HnetworkApp::~HnetworkApp()
{}

void HnetworkApp::setSystemColorScheme()
{
    Qt::ColorScheme systemColorScheme = qApp->styleHints()->colorScheme();
    if (systemColorScheme == Qt::ColorScheme::Dark) {
        this->setStyle(QStyleFactory::create("fusion"));
    }
}
