/*
 * MessageQueue.h
 *
 *  Created on: Apr 22, 2014
 */

#ifndef MESSAGEQUEUE_H_
#define MESSAGEQUEUE_H_

#include "IMessageQueue.h"
#include "MPMCQueue.h"

template <typename T>
class MessageQueue : public IMessageQueue<T>
{
public:
	void Enqueue(T const message) { m_queue.Enqueue(message); }
	bool TryDequeue(T& out_message) { return m_queue.TryDequeue(out_message); }

private:
	Eegeo::Android::Queues::MPMCQueue<T> m_queue;
};

#endif
