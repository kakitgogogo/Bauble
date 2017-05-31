#include "Acceptor.h"

#include "base/Logging.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "SocketsOps.h"

#include <boost/bind.hpp>

using namespace kakui;

Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr):
	_loop(loop),
	_acceptSocket(sockets::createNonblockingOrDie()),
	_acceptChannel(loop, _acceptSocket.fd()),
	_listening(false)
{
	_acceptSocket.setReuseAddr(true);
	_acceptSocket.bindAddress(listenAddr);
	_acceptChannel.setReadCallback(boost::bind(&Acceptor::handleRead, this));
}

void Acceptor::listen()
{
	_loop->assertInLoopThread();
	_listening = true;
	_acceptSocket.listen();
	_acceptChannel.enableReading();
}

void Acceptor::handleRead()
{
	_loop->assertInLoopThread();
	InetAddress peeraddr(0);

	int connfd = _acceptSocket.accept(&peeraddr);
	if(connfd >= 0)
	{
		if(_newConnectionCallback)
		{
			_newConnectionCallback(connfd, peeraddr);
		}
		else
		{
			sockets::close(connfd);
		}
	}
}