#pragma once

class HnNetwork {
public:
    typedef int init_result;

    enum ErrorCodes {
        Success,
        Startup_err,
        Vers_err
    };

    static init_result initialize();
    static void shutdown();
    static int getLastError();
};
