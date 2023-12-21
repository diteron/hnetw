#include <stdafx.h>

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
    if (socketHandle_ == INVALID_SOCKET) {
#ifdef _DEBUG
        int errCode = WSAGetLastError();
#endif // _DEBUG
        return WSA_error;
    }

    socketType_ = Stream;
    return Success;
}

int HnIPv4Socket::createDatagramSocket()
{
    if (socketHandle_ != INVALID_SOCKET)
        return Sock_already_init;

    socketHandle_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socketHandle_ == INVALID_SOCKET) {
#ifdef _DEBUG
        int errCode = WSAGetLastError();
#endif // _DEBUG
        return WSA_error;
    }

    socketType_ = Datagram;
    return Success;
}

int HnIPv4Socket::createRawSocket()
{
    if (socketHandle_ != INVALID_SOCKET)
        return Sock_already_init;

    socketHandle_ = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    if (socketHandle_ == INVALID_SOCKET) {
#ifdef _DEBUG
        int errCode = WSAGetLastError();
#endif // _DEBUG
        return WSA_error;
    }

    socketType_ = Raw;
    return Success;
}

int HnIPv4Socket::close()
{
    if (socketHandle_ == INVALID_SOCKET)
        return Sock_already_closed;

    int closeResult = closesocket(socketHandle_);
    if (closeResult != 0) {
#ifdef _DEBUG
        int errCode = WSAGetLastError();
#endif // _DEBUG
        return WSA_error;
    }

    socketHandle_ = INVALID_SOCKET;
    socketType_ = Undefined;
    return Success;
}

int HnIPv4Socket::bindToInterface(u_long intrface, unsigned short port)
{
    sockaddr_in bindAddr = {};
    bindAddr.sin_family = AF_INET;
    std::memcpy(&bindAddr.sin_addr, &intrface, sizeof(intrface));
    bindAddr.sin_port = htons(port);

    int bindResult = bind(socketHandle_, (sockaddr*) &bindAddr, sizeof(bindAddr));
    if (bindResult != 0) return Bind_error;
    return Success;
}

bool HnIPv4Socket::setPacketCaptureMode()
{
    if (socketType_ != Raw) return false;

    int rcvallOpt = RCVALL_ON;
    int bytesRetBuff = 0;
    int result = WSAIoctl(socketHandle_, SIO_RCVALL, &rcvallOpt, sizeof(rcvallOpt), 0, 0, (LPDWORD) &bytesRetBuff, 0, 0);
    if (result == SOCKET_ERROR) {
#ifdef _DEBUG
        int errCode = WSAGetLastError();
#endif // _DEBUG
        return false;
    }

    return true;
}

const SOCKET HnIPv4Socket::socketHandle() const
{
    return socketHandle_;
}

const HnIPv4Socket::socket_type HnIPv4Socket::socketType() const
{
    return socketType_;
}
