/*
 * IRunnable.h
 *
 *  Created on: Apr 24, 2014
 */

#ifndef IRUNNABLE_H_
#define IRUNNABLE_H_

class IRunnable
{
public:
	virtual bool operator()() = 0;
	virtual void OnStarted() = 0;
	virtual void OnStopped() = 0;
};

#endif /* IRUNNABLE_H_ */
