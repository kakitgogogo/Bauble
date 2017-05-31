#include "kakui/net/EventLoop.h"
#include <stdio.h>

kakui::EventLoop* gLoop;
int gFlag = 0;

void run4()
{
	printf("run4(): pid = %d, flag = %d\n", getpid(), gFlag);
	gLoop->quit();
}

void run3()
{
	printf("run3(): pid = %d, flag = %d\n", getpid(), gFlag);
	gLoop->runAfter(3, run4);
	gFlag = 3;
}

void run2()
{
	printf("run2(): pid = %d, flag = %d\n", getpid(), gFlag);
	gLoop->queueInLoop(run3);
}

void run1()
{
	gFlag = 1;
	printf("run1(): pid = %d, flag = %d\n", getpid(), gFlag);
	gLoop->runInLoop(run2);
	gFlag = 2;
}

int main()
{
	printf("main(): pid = %d, flag = %d\n", getpid(), gFlag);

	kakui::EventLoop loop;
	gLoop = &loop;

	loop.runAfter(2, run1);
	loop.loop();
	printf("main(): pid = %d, flag = %d\n", getpid(), gFlag);
}