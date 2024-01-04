#include <stdafx.h>
#include "hn_network.h"

HnNetwork::init_result HnNetwork::initialize()
{
    WSADATA wsadata;
    int startupResult = WSAStartup(MAKEWORD(2, 2), &wsadata);

    if (startupResult != 0) 
        return Startup_err;
    else if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2)
        return Vers_err;

    return Success;
}

void HnNetwork::shutdown()
{
    WSACleanup();
}

int HnNetwork::getLastError()
{
    return WSAGetLastError();
}
