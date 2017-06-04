#include "TcpConnection.h"

#include "base/Logging.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include "SocketsOps.h"

#include <boost/bind.hpp>
#include <errno.h>
#include <stdio.h>

using namespace kakui;

TcpConnection::TcpConnection(EventLoop* loop, const std::string& name, int sockfd, const InetAddress& localAddr, const InetAddress& peerAddr):
	_loop(loop),
	_name(name),
	_state(kConnecting),
	_socket(new Socket(sockfd)),
	_channel(new Channel(loop, sockfd)),
	_localAddr(localAddr),
	_peerAddr(peerAddr)
{
	LOG_DEBUG << "TcpConnection::ctor[" <<  _name << "] at " << this << " fd=" << sockfd;
	_channel->setReadCallback(boost::bind(&TcpConnection::handleRead, this, _1));
	_channel->setWriteCallback(boost::bind(&TcpConnection::handleWrite, this));
	_channel->setCloseCallback(boost::bind(&TcpConnection::handleClose, this));
	_channel->setErrorCallback(boost::bind(&TcpConnection::handleError, this));
}

TcpConnection::~TcpConnection()
{
	LOG_DEBUG << "TcpConnection::dtor[" <<  _name << "] at " << this << " fd=" << _channel->fd();
}

void TcpConnection::send(const std::string& message)
{
	if(_state == kConnected)
	{
		if(_loop->isInLoopThread())
		{
			sendInLoop(message);
		}
		else
		{
			_loop->runInLoop(boost::bind(&TcpConnection::sendInLoop, this, message));
		}
	}
}

void TcpConnection::sendInLoop(const std::string& message)
{
	_loop->assertInLoopThread();
	ssize_t nwritten = 0;

	if(!_channel->isWriting() && _outputBuffer.readableBytes() == 0)
	{
		nwritten = ::write(_channel->fd(), message.data(), message.size());
		if(nwritten >= 0)
		{
			if(implicit_cast<size_t>(nwritten) < message.size())
			{
				LOG_TRACE << "I am going to write more data";
			}
			else if(_writeCompleteCallback)
			{
				_loop->queueInLoop(boost::bind(_writeCompleteCallback, shared_from_this()));
			}
		}
		else
		{
			nwritten = 0;
			if(errno != EWOULDBLOCK)
			{
				LOG_SYSERR << "TcpConnection::sendInLoop";
			}
		}
	}

	assert(nwritten >= 0);
	if(implicit_cast<size_t>(nwritten) < message.size())
	{
		_outputBuffer.append(message.data()+nwritten, message.size()-nwritten);
		if(!_channel->isWriting())
		{
			_channel->enableWriting();
		}
	}
}


void TcpConnection::shutdown()
{
	if(_state == kConnected)
	{
		setState(kDisconnecting);
	}
	_loop->runInLoop(boost::bind(&TcpConnection::shutdownInLoop, this));
}

void TcpConnection::shutdownInLoop()
{
	_loop->assertInLoopThread();
	if(!_channel->isWriting())
	{
		_socket->shutdownWrite();
	}
}

void TcpConnection::setTcpNoDelay(bool on)
{
	_socket->setTcpNoDelay(on);
}

void TcpConnection::connectEstablished()
{
	_loop->assertInLoopThread();
	assert(_state == kConnecting);
	setState(kConnected);
	_channel->enableReading();
	_connectionCallback(shared_from_this());
}

void TcpConnection::connectDestroyed()
{
	_loop->assertInLoopThread();
	assert(_state == kDisconnecting || _state == kConnected);
	setState(kDisconnected);
	_channel->disableAll();
	_connectionCallback(shared_from_this());

	_loop->removeChannel(get_pointer(_channel));
}

void TcpConnection::handleRead(Timestamp receiveTime)
{
	int savedErrno = 0;
	ssize_t n = _inputBuffer.readFd(_channel->fd(), &savedErrno);
	if(n > 0)
	{
		_messageCallback(shared_from_this(), &_inputBuffer, receiveTime);
	}
	else if(n == 0)
	{
		handleClose();
	}
	else
	{
		errno = savedErrno;
		LOG_SYSERR << "TcpConnection::handleRead";
		handleError();
	}
}

void TcpConnection::handleWrite()
{
	_loop->assertInLoopThread();
	if(_channel->isWriting())
	{
		ssize_t n = ::write(_channel->fd(), _outputBuffer.peek(), _outputBuffer.readableBytes());
		if(n > 0)
		{
			_outputBuffer.retrieve(n);
			if(_outputBuffer.readableBytes() == 0)
			{
				_channel->disableWriting();
				if(_writeCompleteCallback)
				{
					_loop->queueInLoop(boost::bind(_writeCompleteCallback, shared_from_this()));
				}
				if(_state == kDisconnecting)
				{
					shutdownInLoop();
				}
			}
			else
			{
				LOG_TRACE << "I am going to write more data";
			}
		}
		else
		{
			LOG_SYSERR << "TcpConnection::handleWrite";
		}
	}
	else
	{
		LOG_TRACE << "Connection is down, no more writing";
	}
}

void TcpConnection::handleClose()
{
	_loop->assertInLoopThread();
	LOG_TRACE << "TcpConnection::handleClose state = " << _state;
	assert(_state == kConnected || _state == kDisconnecting);
	_channel->disableAll();
	_closeCallback(shared_from_this());
}

void TcpConnection::handleError()
{
	int err = sockets::getSocketError(_channel->fd());
	LOG_ERROR << "TcpConnection::handleError [" << _name << "] - SO_ERROR = " << err << " " << strError(err);
}
