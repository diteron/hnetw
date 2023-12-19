#pragma once

class HNetwork {
public:
    typedef int init_result;

    enum errorCodes {
        Success,
        Startup_err,
        Vers_err
    };

    static init_result initialize();
    static void shutdown();
    static int getLastError();
};
