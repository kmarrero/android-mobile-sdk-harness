/*
 * ActiveObject.h
 *
 *  Created on: Apr 24, 2014
 */

#ifndef ACTIVEOBJECT_H_
#define ACTIVEOBJECT_H_

#include "IRunnable.h"
#include "MessageQueue.h"
#include "Types.h"

template <typename TMessage, typename TDispatcher>
class ActiveObject : public IRunnable, public TDispatcher, protected Eegeo::NonCopyable
{
public:
	virtual ~ActiveObject() { }

	void SendMessage(TMessage message)
	{
		m_messageQueue.Enqueue(message);
	}

	bool operator()()
	{
		TMessage message;

		while(m_messageQueue.TryDequeue(message))
		{
			bool continueRunning = message->Handle(*this);
			OnMessageHandled(message);

			if(!continueRunning)
			{
				return false;
			}
		}

		OnUpdate();

		return true;
	}

	virtual void OnMessageHandled(TMessage message) { }
	virtual void OnUpdate() { }
	virtual void OnStarted() { }
	virtual void OnStopped() { }

private:
    MessageQueue<TMessage> m_messageQueue;
};

#endif
