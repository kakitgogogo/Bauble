#pragma once

#include "Buffer.h"
#include "Callbacks.h"
#include "InetAddress.h"

#include <boost/any.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace kakui
{

class Channel;
class EventLoop;
class Socket;

class TcpConnection: boost::noncopyable, public boost::enable_shared_from_this<TcpConnection>
{
public:
	TcpConnection(EventLoop* loop, const std::string& name, int sockfd, const InetAddress& localAddr, const InetAddress& peerAddr);
	~TcpConnection();

	EventLoop* getLoop() const { return _loop; }
	const std::string name() const { return _name; }
	const InetAddress& localAddress() { return _localAddr; }
	const InetAddress& peerAddress() { return _peerAddr; }
	bool connected() const { return _state == kConnected; }

	void send(const std::string& message);
	void shutdown();
	void setTcpNoDelay(bool on);

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
	void setCloseCallback(const CloseCallback& cb)
	{
		_closeCallback = cb;
	}

	void connectEstablished();
	void connectDestroyed();

private:
	enum StateE { kConnecting, kConnected, kDisconnecting, kDisconnected };

	void setState(StateE s) { _state = s; }
	void handleRead(Timestamp receiveTime);
	void handleWrite();
	void handleClose();
	void handleError();
	void sendInLoop(const std::string& message);
	void shutdownInLoop();	

	EventLoop* _loop;
	std::string _name;
	StateE _state;

	boost::scoped_ptr<Socket> _socket;
	boost::scoped_ptr<Channel> _channel;
	InetAddress _localAddr;
	InetAddress _peerAddr;

	ConnectionCallback _connectionCallback;
	MessageCallback _messageCallback;
	WriteCompleteCallback _writeCompleteCallback;
	CloseCallback _closeCallback;
	
	Buffer _inputBuffer;
	Buffer _outputBuffer;
};

}