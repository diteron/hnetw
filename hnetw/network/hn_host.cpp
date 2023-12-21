#include <stdafx.h>
#include "hn_host.h"

HnHost::HnHost(unsigned short port) : port_(port)
{}

HnHost::~HnHost()
{}

bool HnHost::initialize()
{
    const int MaxHostNameLen = 128;

    char hostName[MaxHostNameLen] = "";
    int result = gethostname(hostName, sizeof(hostName));
    if (result == SOCKET_ERROR) return false;

    hostName_.assign(hostName);
    result = getAllInterfaces();

    return result;
}

u_long HnHost::interfaceAt(int index) const
{
    return (index < interfaces_.size()) ? interfaces_[index] : 0;
}

std::vector<std::string> HnHost::interfacesStrings() const
{
    const int IPStringLen = 16;

    in_addr inAddr;
    char stringBuff[IPStringLen] = "";
    std::vector<std::string> strings;
    for (u_long interface_ : interfaces_) {
        inAddr.s_addr = interface_;
        strings.push_back(inet_ntop(AF_INET, &inAddr, stringBuff, sizeof(stringBuff)));
    }
    return strings;
}

bool HnHost::getAllInterfaces()
{
    addrinfo hints = {};
    hints.ai_family = AF_INET;
    
    addrinfo* hostinfo = nullptr;
    u_long ipLong = 0;
    sockaddr_in* interfaceAddress = nullptr;

    int result = getaddrinfo(hostName_.c_str(), NULL, &hints, &hostinfo);
    if (result != 0) return false;

    while (hostinfo != NULL) {
        interfaceAddress = reinterpret_cast<sockaddr_in*>(hostinfo->ai_addr);
        ipLong = interfaceAddress->sin_addr.s_addr;
        // Possible error?
        interfaces_.push_back(ipLong);
        hostinfo = hostinfo->ai_next;
    }

    return true;
}
