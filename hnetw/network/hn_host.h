#pragma once

class HnHost {
public:
	HnHost();
	HnHost(unsigned short port);
	~HnHost();

	void setPort(unsigned short port);
	bool initialize();
	u_long interfaceIpAt(int index) const;

	const char* interfaceIpStringAt(int index) const;
	std::vector<std::string> interfacesIpStrings() const;

	std::string hostname() const;
	unsigned short port() const;

private:
	bool getAllInterfacesIp();

	std::string hostName_ = "";
	unsigned short port_ = 0;
	std::vector<u_long> interfacesIp_;
};

