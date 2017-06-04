#pragma once

#include "InetAddress.h"
#include "TimerId.h"

#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace kakui
{

class Channel;
class EventLoop;

class Connector: boost::noncopyable
{
public:
	typedef boost::function<void (int sockfd)> NewConnectionCallback;

	Connector(EventLoop* loop, const InetAddress& serverAddr);
	~Connector();

	void setNewConnectionCallback(const NewConnectionCallback& cb)
	{ _newConnectionCallback = cb; }

	void start();
	void restart();
	void stop();

	const InetAddress& serverAddress() const { return _serverAddr; }

private:
	enum States { kDisconnected, kConnecting, kConnected };
	static const int kMaxRetryDelayMs = 30*1000;
	static const int kInitRetryDelayMs = 500;

	void setState(States s) { _state = s; }
	void startInLoop();
	void connect();
	void connecting(int sockfd);
	void handleWrite();
	void handleError();
	void retry(int sockfd);
	int removeAndResetChannel();
	void resetChannel();

	EventLoop* _loop;
	InetAddress _serverAddr;
	bool _connect;
	States _state;
	boost::scoped_ptr<Channel> _channel;
	NewConnectionCallback _newConnectionCallback;
	int _retryDelayMs;
	TimerId _timerId;
};

typedef boost::shared_ptr<Connector> ConnectorPtr;

}