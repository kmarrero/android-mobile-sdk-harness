/*
 * MessageHandlerChain.h
 *
 *  Created on: Apr 22, 2014
 */

#ifndef MESSAGEHANDLERCHAIN_H_
#define MESSAGEHANDLERCHAIN_H_

#include "IMessageHandlerChain.h"
#include <algorithm>

template<typename M>
class MessageHandlerChain : public IMessageHandlerChain<M>
{
	typedef IMessageHandler<M> TMessageHandler;
	typedef TMessageHandler* TMessageHandlerPtr;
	typedef std::vector<TMessageHandlerPtr> TMessageHandlerPtrVec;

public:
	bool HandleMessage(const M& message)
	{
		for(typename TMessageHandlerPtrVec::const_iterator it = m_handlers.begin(); it != m_handlers.end(); ++it)
		{
			if((*it)->HandleMessage(message))
			{
				return true;
			}
		}

		return false;
	}

	void AddHandler(TMessageHandler& messageHandler)
	{
		m_handlers.push_back(&messageHandler);
	}

	void RemoveHandler(TMessageHandler& messageHandler)
	{
		typename TMessageHandlerPtrVec::iterator found = std::find(m_handlers.begin(), m_handlers.end(), &messageHandler);

		if(found != m_handlers.begin())
		{
			m_handlers.erase(found);
		}
	}

private:
	TMessageHandlerPtrVec m_handlers;
};

#endif /* MESSAGEHANDLERCHAIN_H_ */
