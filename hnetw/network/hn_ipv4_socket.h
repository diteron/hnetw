#pragma once

class HnIPv4Socket {
public:
    typedef int socket_type;

    HnIPv4Socket(SOCKET socketHandle = INVALID_SOCKET);
    ~HnIPv4Socket();

    enum socketTypes {
        Undefined,
        Stream,
        Datagram,
        Raw
    };

    enum errorCodes {
        Success,
        Sock_already_init,
        WSA_error,
        Sock_already_closed,
        Bind_error
    };

    int createStreamSocket();
    int createDatagramSocket();
    int createRawSocket();
    int close();

    int bindToInterface(u_long intrface, unsigned short port);

    const SOCKET socketHandle() const { return socketHandle_; }
    const socket_type socketType() const { return socketType_; }

private:
    SOCKET socketHandle_ = INVALID_SOCKET;
    socket_type socketType_ = Undefined;
};
