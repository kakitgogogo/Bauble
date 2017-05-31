#pragma once

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include "Channel.h"
#include "Socket.h"

namespace kakui
{

class EventLoop;
class InetAddress;

class Acceptor: boost::noncopyable
{
public:
	typedef boost::function<void (int sockfd, const InetAddress&)> NewConnectionCallback;

	Acceptor(EventLoop* loop, const InetAddress& listenAddr);

	void setNewConnectionCallback(const NewConnectionCallback& cb)
	{ _newConnectionCallback = cb; }

	bool listening() const { return _listening; }
	void listen();

private:
	void handleRead();

	EventLoop* _loop;
	Socket _acceptSocket;
	Channel _acceptChannel;
	NewConnectionCallback _newConnectionCallback;
	bool _listening;
};

}