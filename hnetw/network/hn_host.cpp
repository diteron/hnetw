#include <stdafx.h>
#include "hn_host.h"

HnHost::HnHost()
{}

HnHost::HnHost(unsigned short port) : port_(port)
{}

HnHost::~HnHost()
{}

void HnHost::setPort(unsigned short port)
{
    port_ = port;
}

bool HnHost::initialize()
{
    const int MaxHostNameLen = 128;

    char hostName[MaxHostNameLen] = "";
    int result = gethostname(hostName, sizeof(hostName));
    if (result == SOCKET_ERROR) return false;

    hostName_.assign(hostName);
    result = getAllInterfacesIp();
    return result;
}

u_long HnHost::interfaceIpAt(int index) const
{
    return (index < interfacesIp_.size()) ? interfacesIp_[index] : 0;
}

const char* HnHost::interfaceIpStringAt(int index) const
{
    const int IPStringLen = 16;

    in_addr inAddr;
    char stringBuff[IPStringLen] = "";
    inAddr.s_addr = interfacesIp_[index];

    return (inet_ntop(AF_INET, &inAddr, stringBuff, sizeof(stringBuff)));
}

std::vector<std::string> HnHost::interfacesIpStrings() const
{
    const int IPStringLen = 16;

    in_addr inAddr;
    char stringBuff[IPStringLen] = "";
    std::vector<std::string> strings;
    for (u_long interface_ : interfacesIp_) {
        inAddr.s_addr = interface_;
        strings.push_back(inet_ntop(AF_INET, &inAddr, stringBuff, sizeof(stringBuff)));
    }
    return strings;
}

std::string HnHost::hostname() const
{
    return hostName_;
}

unsigned short HnHost::port() const
{
    return port_;
}

bool HnHost::getAllInterfacesIp()
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
        interfacesIp_.push_back(ipLong);
        hostinfo = hostinfo->ai_next;
    }

    return true;
}
