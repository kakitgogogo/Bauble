#pragma once

#include <map>
#include <vector>

#include "base/Timestamp.h"
#include "EventLoop.h"

struct epoll_event;

namespace kakui
{

class Channel;

class Epoller: boost::noncopyable
{
public:
	typedef std::vector<Channel*> ChannelList;

	Epoller(EventLoop* loop);
	~Epoller();

	Timestamp poll(int timeoutMs, ChannelList* activeChannels);

	void updateChannel(Channel* channel);

	void removeChannel(Channel* channel);

	void assertInLoopThread() { _loop->assertInLoopThread(); }

private:
	static const int kInitEventListSize = 16;

	void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;
	void update(int operation, Channel* channel);

	typedef std::vector<struct epoll_event> EventList;
	typedef std::map<int, Channel*> ChannelMap;

	EventLoop* _loop;
	int _epollfd;
	EventList _events;
	ChannelMap _channels;
};

}