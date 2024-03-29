#pragma once

class HnIPv4Socket {
public:
    typedef int socket_type;

    HnIPv4Socket(SOCKET socketHandle = INVALID_SOCKET);
    ~HnIPv4Socket();

    enum SocketTypes {
        Undefined,
        Stream,
        Datagram,
        Raw
    };

    enum ErrorCodes {
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

    int bindToInterface(u_long interfaceIp, unsigned short port);
    bool setPacketCaptureMode();

    const SOCKET socketHandle() const;
    const socket_type socketType() const;
    bool isValid() const;

private:
    SOCKET socketHandle_ = INVALID_SOCKET;
    socket_type socketType_ = Undefined;
};
