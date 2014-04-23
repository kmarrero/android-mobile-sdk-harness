/*
 * IMessageHandlerChain.h
 *
 *  Created on: Apr 22, 2014
 */

#ifndef IMESSAGEHANDLERCHAIN_H_
#define IMESSAGEHANDLERCHAIN_H_

#include "IMessageHandler.h"

template<typename M>
class IMessageHandlerChain : public IMessageHandler<M>
{
public:
	virtual ~IMessageHandlerChain() {};

	virtual void AddHandler(IMessageHandler<M>& messageHandler) = 0;
	virtual void RemoveHandler(IMessageHandler<M>& messageHandler) = 0;
};

#endif /* IMESSAGEHANDLERCHAIN_H_ */
