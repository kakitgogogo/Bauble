#include "kakui/net/Connector.h"
#include "kakui/net/EventLoop.h"

#include <stdio.h>

kakui::EventLoop* gLoop;

void connectCallback(int sockfd)
{
	printf("connected.\n");
	gLoop->quit();
}

int main()
{
	kakui::EventLoop loop;
	gLoop = &loop;
	kakui::InetAddress addr("127.0.0.1", 8888);
	kakui::ConnectorPtr connector(new kakui::Connector(&loop, addr));
	connector->setNewConnectionCallback(connectCallback);
	connector->start();

	loop.loop();
}