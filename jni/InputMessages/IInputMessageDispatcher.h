/*
 * IInputMessageDispatcher.h
 *
 *  Created on: Apr 23, 2014
 */

#ifndef IINPUTMESSAGEDISPATCHER_H_
#define IINPUTMESSAGEDISPATCHER_H_

#include "InputMessages.h"

namespace InputMessages
{
	class IInputMessageDispatcher
	{
	public:
		virtual ~IInputMessageDispatcher() {};
		virtual bool HandleMessage(const TouchEventMessage& message) = 0;
	};
}

#endif /* IINPUTMESSAGEDISPATCHER_H_ */
