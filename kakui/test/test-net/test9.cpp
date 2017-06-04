#include "kakui/net/EventLoop.h"
#include "kakui/net/TcpServer.h"
#include "kakui/net/InetAddress.h"
#include <stdio.h>

void onConnection(const kakui::TcpConnectionPtr& conn)
{
	if(conn->connected())
	{
		printf("onConnection(): tid=%d new connection [%s] from %s\n",
				kakui::CurrentThread::tid(),
				conn->name().c_str(),
				conn->peerAddress().toHostPort().c_str());
	}
	else
	{
		printf("onConnection(): tid=%d connection [%s] is down\n",
				kakui::CurrentThread::tid(),
				conn->name().c_str());
	}
}

void onMessage(const kakui::TcpConnectionPtr& conn, kakui::Buffer* buf, kakui::Timestamp receiveTime)
{
	printf("onMessage(): tid=%d received %zd bytes from connection [%s] at %s\n",
			kakui::CurrentThread::tid(),
			buf->readableBytes(),
			conn->name().c_str(),
			receiveTime.toFormattedString().c_str());

	conn->send(buf->retrieveAsString());
}

int main(int argc, char* argv[])
{
	printf("main(): pid = %d\n", getpid());

	kakui::InetAddress listenAddr(8888);
	kakui::EventLoop loop;

	kakui::TcpServer server(&loop, listenAddr);
	server.setConnectionCallback(onConnection);
	server.setMessageCallback(onMessage);
	if(argc > 1)
	{
		server.setThreadNum(atoi(argv[1]));
	}
	server.start();

	loop.loop();
}
