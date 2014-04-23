/*
 * AppMessageQueue.h
 *
 *  Created on: Apr 22, 2014
 */

#ifndef APPMESSAGEQUEUE_H_
#define APPMESSAGEQUEUE_H_

#include "IAppMessage.h"
#include "IMessageQueue.h"
#include "MPMCQueue.h"

class AppMessageQueue : public IMessageQueue<const AppMessages::IAppMessage*>
{
public:
	void Enqueue(const AppMessages::IAppMessage* const message);
	bool TryDequeue(const AppMessages::IAppMessage*& out_message);

private:
	Eegeo::Android::Queues::MPMCQueue<const AppMessages::IAppMessage*> m_queue;
};

#endif /* APPMESSAGEQUEUE_H_ */
