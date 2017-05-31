#include "InetAddress.h"
#include "SocketsOps.h"

#include <strings.h>
#include <netinet/in.h>

#include <boost/static_assert.hpp>

using namespace kakui;

static const in_addr_t kInaddrAny = INADDR_ANY;

BOOST_STATIC_ASSERT(sizeof(InetAddress) == sizeof(struct sockaddr_in));

InetAddress::InetAddress(uint16_t port)
{
	bzero(&_addr, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = sockets::hostToNetwork32(kInaddrAny);
	_addr.sin_port = sockets::hostToNetwork16(port);
}

InetAddress::InetAddress(const std::string& ip, uint16_t port)
{
	bzero(&_addr, sizeof(_addr));
	sockets::fromHostPort(ip.c_str(), port, &_addr);
}

std::string InetAddress::toHostPort() const
{
	char buf[32];
	sockets::toHostPort(buf, sizeof(buf), _addr);
	return buf;
}