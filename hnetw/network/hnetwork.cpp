#include "stdafx.h"
#include "hnetwork.h"

HNetwork::init_result HNetwork::initialize()
{
    WSADATA wsadata;
    int startupResult = WSAStartup(MAKEWORD(2, 2), &wsadata);

    if (startupResult != 0) 
        return Startup_err;
    else if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2)
        return Vers_err;

    return Success;
}

void HNetwork::shutdown()
{
    WSACleanup();
}

int HNetwork::getLastError()
{
    return WSAGetLastError();
}
