#pragma once

class HnHost {
public:
	HnHost(unsigned short port);
	~HnHost();

	bool initialize();
	u_long interfaceAt(int index) const;
	std::vector<std::string> interfacesStrings() const;

	std::string hostname() const { return hostName_; }
	unsigned short port() const { return port_; }

private:
	bool getAllInterfaces();

	std::string hostName_ = "";
	unsigned short port_ = 0;
	std::vector<u_long> interfaces_;
};

