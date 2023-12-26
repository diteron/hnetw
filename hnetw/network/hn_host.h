#pragma once

class HnHost {
public:
	HnHost();
	HnHost(unsigned short port);
	~HnHost();

	void setPort(unsigned short port);
	bool initialize();
	u_long interfaceAt(int index) const;
	std::vector<std::string> interfacesStrings() const;

	std::string hostname() const;
	unsigned short port() const;

private:
	bool getAllInterfaces();

	std::string hostName_ = "";
	unsigned short port_ = 0;
	std::vector<u_long> interfaces_;
};

