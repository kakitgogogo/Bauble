#pragma once

#include "Atomic.h"
#include "Types.h"

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <pthread.h>


namespace kakui
{

class Thread: boost::noncopyable
{
public:
	typedef boost::function<void()> ThreadFunc;
	explicit Thread(const ThreadFunc&, const string& name = string());
#ifdef __GXX_EXPERIMENTAL_CXX0X__
	explicit Thread(ThreadFunc&&, const string& name = string());
#endif
	~Thread();

	void start();
	int join();

	bool started() const { return _started; }
	pid_t tid() const { return *_tid; }
	const string& name() const { return _name; }

	static int numCreated() { return _numCreated.get(); }

private:
	void setDefaultName();

	bool _started;
	bool _joined;
	pthread_t _pthreadId;
	boost::shared_ptr<pid_t> _tid;
	ThreadFunc _func;
	string _name;

	static AtomicInt32 _numCreated;
};

}