#include "stdafx.h"
#include "hn_ipv4_socket.h"

HnIPv4Socket::HnIPv4Socket(SOCKET socketHandle) : socketHandle_(socketHandle)
{}

HnIPv4Socket::~HnIPv4Socket()
{
    if (socketHandle_ != INVALID_SOCKET)
        closesocket(socketHandle_);
}

int HnIPv4Socket::createStreamSocket()
{
    if (socketHandle_ != INVALID_SOCKET)
        return Sock_already_init;
    
    socketHandle_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socketHandle_ == INVALID_SOCKET)
        return WSA_error;

    socketType_ = Stream;
    return Success;
}

int HnIPv4Socket::createDatagramSocket()
{
    if (socketHandle_ != INVALID_SOCKET)
        return Sock_already_init;

    socketHandle_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socketHandle_ == INVALID_SOCKET)
        return WSA_error;

    socketType_ = Datagram;
    return Success;
}

int HnIPv4Socket::createRawSocket()
{
    if (socketHandle_ != INVALID_SOCKET)
        return Sock_already_init;

    socketHandle_ = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    if (socketHandle_ == INVALID_SOCKET)
        return WSA_error;

    socketType_ = Raw;
    return Success;
}

int HnIPv4Socket::close()
{
    if (socketHandle_ == INVALID_SOCKET)
        return Sock_already_closed;

    int closeResult = closesocket(socketHandle_);
    if (closeResult != 0)
        return WSA_error;

    socketHandle_ = INVALID_SOCKET;
    socketType_ = Undefined;
    return Success;
}
