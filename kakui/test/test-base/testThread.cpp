#include "kakui/base/Thread.h"
#include "kakui/base/CurrentThread.h"

#include <string>
#include <boost/bind.hpp>
#include <stdio.h>
#include <unistd.h>

void mysleep(int seconds)
{
	timespec t = { seconds, 0 };
	nanosleep(&t, NULL);
}

void threadFunc1()
{
	printf("tid=%d\n", kakui::CurrentThread::tid());
}

void threadFunc2(int x)
{
	printf("tid=%d, x=%d\n", kakui::CurrentThread::tid(), x);
}

void threadFunc3()
{
	printf("tid=%d\n", kakui::CurrentThread::tid());
	mysleep(1);
}

class Foo
{
public:
	explicit Foo(double x): _x(x) {}

	void memberFunc1()
	{
		printf("tid=%d, x=%f\n", kakui::CurrentThread::tid(), _x);
	}

	void memberFunc2(const std::string& text)
	{
		printf("tid=%d, x=%f, text=%s\n", kakui::CurrentThread::tid(), _x, text.c_str());
	}

private:
	double _x;
};

int main()
{
	printf("pid=%d, tid=%d\n", ::getpid(), kakui::CurrentThread::tid());

	kakui::Thread t1(threadFunc1);
	t1.start();
	t1.join();

	kakui::Thread t2(boost::bind(threadFunc2, 42), "thread for free function with argument");
	t2.start();
	t2.join();

	Foo foo(11.11);
	kakui::Thread t3(boost::bind(&Foo::memberFunc1, &foo), "tthread for member function without argument");
	t3.start();
	t3.join();

	kakui::Thread t4(boost::bind(&Foo::memberFunc2, boost::ref(foo), std::string("kakit")));
	t4.start();
	t4.join();

	{
		kakui::Thread t5(threadFunc3);
		t5.start();
	}
	mysleep(2);
	{
		kakui::Thread t6(threadFunc3);
		t6.start();
		mysleep(2);
	}
	sleep(2);
	printf("number of created threads %d\n", kakui::Thread::numCreated());
}