#include "kakui/net/Acceptor.h"
#include "kakui/net/EventLoop.h"
#include "kakui/net/InetAddress.h"
#include "kakui/net/SocketsOps.h"
#include <stdio.h>

void newConnection(int sockfd, const kakui::InetAddress& peeraddr)
{
	printf("newConnection(): accepted a new connection from %s\n", peeraddr.toHostPort().c_str());
	::write(sockfd, "How are you?\n", 13);
	kakui::sockets::close(sockfd);
}

int main()
{
	printf("main(): pid = %d\n", getpid());

	kakui::InetAddress listenAddr(8888);
	kakui::EventLoop loop;

	kakui::Acceptor acceptor(&loop, listenAddr);
	acceptor.setNewConnectionCallback(newConnection);
	acceptor.listen();

	loop.loop();
}