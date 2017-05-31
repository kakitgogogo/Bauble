#include "Epoller.h"
#include "Channel.h"
#include "base/Logging.h"

#include <boost/static_assert.hpp>

#include <assert.h>
#include <errno.h>
#include <poll.h>
#include <sys/epoll.h>

using namespace kakui;

BOOST_STATIC_ASSERT(EPOLLIN == POLLIN);
BOOST_STATIC_ASSERT(EPOLLPRI == POLLPRI);
BOOST_STATIC_ASSERT(EPOLLOUT == POLLOUT);
BOOST_STATIC_ASSERT(EPOLLRDHUP == POLLRDHUP);
BOOST_STATIC_ASSERT(EPOLLERR == POLLERR);
BOOST_STATIC_ASSERT(EPOLLHUP == POLLHUP);

const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;

Epoller::Epoller(EventLoop* loop):
	_loop(loop),
	_epollfd(::epoll_create1(EPOLL_CLOEXEC)),
	_events(kInitEventListSize)
{
	if(_epollfd < 0)
	{
		LOG_SYSFATAL << "Epoller::Epoller";
	}
}

Epoller::~Epoller()
{
	::close(_epollfd);
}

Timestamp Epoller::poll(int timeoutMs, ChannelList* activeChannels)
{
	int numEvents = ::epoll_wait(_epollfd, &*_events.begin(), static_cast<int>(_events.size()), timeoutMs);
	Timestamp now(Timestamp::now());

	if(numEvents > 0)
	{
		LOG_TRACE << numEvents << " events happened";
		fillActiveChannels(numEvents, activeChannels);
		if(implicit_cast<size_t>(numEvents) == _events.size())
		{
			_events.resize(_events.size() * 2);
		}
	}
	else if(numEvents == 0)
	{
		LOG_TRACE << " nothing happened";
	}
	else
	{
		LOG_SYSERR << "Epoller::poll()";
	}

	return now;
}

void Epoller::fillActiveChannels(int numEvents, ChannelList* activeChannels) const
{
	assert(implicit_cast<size_t>(numEvents) <= _events.size());
	for(int i = 0; i < numEvents; ++i)
	{
		Channel* channel = static_cast<Channel*>(_events[i].data.ptr);

		channel->setRevents(_events[i].events);
		activeChannels->push_back(channel);
	}
}

void Epoller::updateChannel(Channel* channel)
{
	assertInLoopThread();
	LOG_TRACE << "fd = " << channel->fd() << " events = " << channel->events();
	const int index = channel->index();
	if(index == kNew || index == kDeleted)
	{
		// a new one, add with EPOLL_CTL_ADD
		int fd = channel->fd();
		if (index == kNew)
		{
			assert(_channels.find(fd) == _channels.end());
			_channels[fd] = channel;
		}
		else // index == kDeleted
		{
			assert(_channels.find(fd) != _channels.end());
			assert(_channels[fd] == channel);
		}
		channel->setIndex(kAdded);
		update(EPOLL_CTL_ADD, channel);
	}
	else 
	{
		int fd = channel->fd();
		assert(_channels.find(fd) != _channels.end());
		assert(_channels[fd] == channel);
		assert(index == kAdded);
		if(channel->isNoneEvent())
		{
			update(EPOLL_CTL_DEL, channel);
			channel->setIndex(kDeleted);
		}
		else
		{
			update(EPOLL_CTL_MOD, channel);
		}
	}
}

void Epoller::removeChannel(Channel* channel)
{
	assertInLoopThread();
	int fd = channel->fd();
	int index = channel->index();

	LOG_TRACE << "fd = " << fd;
	assert(_channels.find(fd) != _channels.end());
	assert(_channels[fd] == channel);
	assert(channel->isNoneEvent());assert(index == kAdded || index == kDeleted);
	size_t n = _channels.erase(fd);
	assert(n == 1);

	if(index == kAdded)
	{
		update(EPOLL_CTL_DEL, channel);
	}
	channel->setIndex(kNew);
}

void Epoller::update(int operation, Channel* channel)
{
	struct epoll_event event;
	bzero(&event, sizeof(event));
	event.events = channel->events();
	event.data.ptr = channel;
	int fd = channel->fd();
	if(::epoll_ctl(_epollfd, operation, fd, &event) < 0)
	{
		if(operation == EPOLL_CTL_DEL)
		{
			LOG_SYSERR << "epoll_ctl op=" << operation << " fd=" << fd;
		}
		else
		{
			LOG_SYSFATAL << "epoll_ctl op=" << operation << " fd=" << fd;
		}
	}
}