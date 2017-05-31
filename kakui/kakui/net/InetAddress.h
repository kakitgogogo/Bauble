#pragma once

#include <string>
#include <netinet/in.h>

namespace kakui
{

class InetAddress
{
public:
	explicit InetAddress(uint16_t port);

	InetAddress(const std::string& ip, uint16_t port);

	InetAddress(const struct sockaddr_in& addr): _addr(addr) {}

	std::string toHostPort() const;

	const struct sockaddr_in& getSockAddrInet() const { return _addr; }
	void setSockAddrInet(const struct sockaddr_in& addr) { _addr = addr; }

private:
	struct sockaddr_in _addr;
};

}