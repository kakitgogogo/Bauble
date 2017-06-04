#pragma once

#include <boost/noncopyable.hpp>

#include "base/Mutex.h"
#include "TcpConnection.h"

namespace kakui
{

class Connector;
typedef boost::shared_ptr<Connector> ConnectorPtr;

class TcpClient: boost::noncopyable
{
public:
	TcpClient(EventLoop* loop, const InetAddress& serverAddr);
	~TcpClient();

	void connect();
	void disconnect();
	void stop();

	TcpConnectionPtr connection() const 
	{
		MutexLockGuard lock(_mutex);
		return _connection;
	}

	bool retry() const;
	void enableRetry() { _retry = true; }

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
	void newConnection(int sokcfd);
	void removeConnection(const TcpConnectionPtr& conn);

	EventLoop* _loop;
	ConnectorPtr _connector;

	ConnectionCallback _connectionCallback;
	MessageCallback _messageCallback;
	WriteCompleteCallback _writeCompleteCallback;

	bool _retry;
	bool _connect;
	int _nextConnId;
	mutable MutexLock _mutex;
	TcpConnectionPtr _connection;
};

}