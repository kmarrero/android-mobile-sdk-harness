/*
 * IMessageQueue.h
 *
 *  Created on: Apr 22, 2014
 */

#ifndef IMESSAGEQUEUE_H_
#define IMESSAGEQUEUE_H_

template<typename M>
class IMessageQueue
{
public:
	virtual ~IMessageQueue() {};
	virtual void Enqueue(M const message) = 0;
	virtual bool TryDequeue(M& out_message) = 0;
};

#endif /* IMESSAGEQUEUE_H_ */
