#pragma once

#include <boost/noncopyable.hpp>

namespace kakui
{

class InetAddress;

class Socket: boost::noncopyable
{
public:
	explicit Socket(int sockfd): _sockfd(sockfd) {}

	~Socket();

	int fd() const { return _sockfd; }

	void bindAddress(const InetAddress& localaddr);

	void listen();

	int accept(InetAddress* peeraddr);

	void setReuseAddr(bool on);

	void shutdownWrite();

	void setTcpNoDelay(bool on);

private:
	const int _sockfd;
};

}