/*
 * Thread.cpp
 *
 *  Created on: Apr 21, 2014
 */

#include "Thread.h"

Thread::Thread()
: m_thread(0)
{
}

Thread::~Thread()
{
	Eegeo_ASSERT(m_thread != 0, "Attempt to destroy an active object that is still running. Please call Stop() before destroying.");
}

void* Thread::Run(void* pObj)
{
	Thread* pThread = (Thread*) pObj;

	pThread->OnStarted();

	bool continueRunning = true;
	while(continueRunning)
	{
		continueRunning = pThread->operator()();
	}

	pThread->OnStopped();

	return NULL;
}

void Thread::Start()
{
	Eegeo_ASSERT(m_thread == 0, "Attempt to Start() an active thread that has already been Start()ed.");
	pthread_create(&m_thread, 0, Run, this);
}

void Thread::WaitForStop()
{
	Eegeo_ASSERT(m_thread != 0, "Attempt to Stop() an active object that hasn't been started.");
	pthread_join(m_thread, 0);
	m_thread = 0;
}
