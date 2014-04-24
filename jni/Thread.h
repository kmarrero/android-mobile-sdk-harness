/*
 * Thread.h
 *
 *  Created on: Apr 21, 2014
 */

#ifndef THREAD_H_
#define THREAD_H_

#include "Types.h"
#include "IRunnable.h"
#include <pthread.h>

class Thread : protected Eegeo::NonCopyable
{
public:
	Thread(IRunnable& runnable);
	virtual ~Thread();

	void Start();
	void WaitForStop();

private:
	IRunnable& m_runnable;
	pthread_t m_thread;
	static void* Run(void* pObj);

	IRunnable& GetRunnable() const;
};

#endif
