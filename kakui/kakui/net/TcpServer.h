#pragma once

#include "Callbacks.h"
#include "TcpConnection.h"

#include <map>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace kakui
{

class Acceptor;
class EventLoop;
class EventLoopThreadPool;

class TcpServer: boost::noncopyable
{
public:
	TcpServer(EventLoop* loop, const InetAddress& listenAddr);
	~TcpServer();

	void setThreadNum(int num);

	void start();

	void setConnectionCallback(const ConnectionCallback& cb)
	{
		_connectionCallback = cb;
	}
	void setMessageCallback(const MessageCallback& cb)
	{
		_messageCallback = cb;
	}
	void setWriteCompleteCallback(const WriteCompleteCallback& cb)
	{
		_writeCompleteCallback = cb;
	}

private:
	void newConnection(int sokcfd, const InetAddress& peerAddr);
	void removeConnection(const TcpConnectionPtr& conn);
	void removeConnectionInLoop(const TcpConnectionPtr& conn);

	typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;

	EventLoop* _loop;
	const std::string _name;
	boost::scoped_ptr<Acceptor> _acceptor;
	boost::scoped_ptr<EventLoopThreadPool> _threadPool;

	ConnectionCallback _connectionCallback;
	MessageCallback _messageCallback;
	WriteCompleteCallback _writeCompleteCallback;

	bool _started;
	int _nextConnId;
	ConnectionMap _connections;
};

}