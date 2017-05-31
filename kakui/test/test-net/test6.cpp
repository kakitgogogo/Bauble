#include "kakui/net/EventLoop.h"
#include "kakui/net/EventLoopThread.h"
#include <stdio.h>

void runInThread()
{
	printf("runInThread(): pid = %d, tid = %d\n", getpid(), kakui::CurrentThread::tid());
}

int main()
{
	printf("main(): pid = %d, tid = %d\n", getpid(), kakui::CurrentThread::tid());

	kakui::EventLoopThread loopThread;
	kakui::EventLoop* loop = loopThread.startLoop();
	loop->runInLoop(runInThread);
	sleep(1);
	loop->runAfter(2, runInThread);
	sleep(3);
	loop->quit();

	printf("exit main().\n");
}