/*
 * ActiveObject.h
 *
 *  Created on: Apr 21, 2014
 */

#ifndef ACTIVEOBJECT_H_
#define ACTIVEOBJECT_H_

#include "Types.h"
#include <pthread.h>

class Thread : protected Eegeo::NonCopyable
{
public:
	Thread();
	virtual ~Thread();

	void Start();
	void WaitForStop();

protected:
	virtual bool operator()() = 0;
	virtual void OnStarted() {};
	virtual void OnStopped() {};

private:
	pthread_t m_thread;

	static void* Run(void* pObj);
};

#endif /* ACTIVEOBJECT_H_ */
