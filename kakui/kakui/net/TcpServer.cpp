#include "TcpServer.h"

#include "base/Logging.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"
#include "SocketsOps.h"

#include <boost/bind.hpp>
#include <stdio.h>

using namespace kakui;

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddr):
	_loop(CHECK_NOTNULL(loop)),
	_name(listenAddr.toHostPort()),
	_acceptor(new Acceptor(loop, listenAddr)),
	_threadPool(new EventLoopThreadPool(loop)),
	_started(false),
	_nextConnId(1)
{
	_acceptor->setNewConnectionCallback(boost::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer()
{

}

void TcpServer::setThreadNum(int num)
{
	assert(num >= 0);
	_threadPool->setThreadNum(num);
}

void TcpServer::start()
{
	if(!_started)
	{
		_started = true;
		_threadPool->start();
	}
	if(!_acceptor->listening())
	{
		_loop->runInLoop(boost::bind(&Acceptor::listen, get_pointer(_acceptor)));
	}
}

void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr)
{
	_loop->assertInLoopThread();
	char buf[32];
	snprintf(buf, sizeof(buf), "#%d", _nextConnId);
	++_nextConnId;
	std::string connName = _name + buf;

	LOG_INFO << "TcpServer::newConnection [" << _name << "] - new connection [" << connName << "] from " << peerAddr.toHostPort();
	InetAddress localAddr(sockets::getLocalAddr(sockfd));
	EventLoop* ioLoop = _threadPool->getNextLoop();
	TcpConnectionPtr conn(new TcpConnection(ioLoop, connName, sockfd, localAddr, peerAddr));
	_connections[connName] = conn;

	conn->setConnectionCallback(_connectionCallback);
	conn->setMessageCallback(_messageCallback);
	conn->setWriteCompleteCallback(_writeCompleteCallback);
	conn->setCloseCallback(boost::bind(&TcpServer::removeConnection, this, _1));

	ioLoop->runInLoop(boost::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
	_loop->runInLoop(boost::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
	_loop->assertInLoopThread();
	LOG_INFO << "TcpServer::removeConnectionInLoop [" << _name << "] - connection " << conn->name();
	size_t n = _connections.erase(conn->name());
	assert(n == 1);
	EventLoop* ioLoop = conn->getLoop();
	ioLoop->queueInLoop(boost::bind(&TcpConnection::connectDestroyed, conn));
}

