/*
 * IAppMessageDispatcher.h
 *
 *  Created on: Apr 22, 2014
 */

#ifndef IAPPMESSAGEDISPATCHER_H_
#define IAPPMESSAGEDISPATCHER_H_

#include "AppMessages.h"
#include "AppLifecycleMessages.h"
#include "InputMessages.h"

#include "IAppLifecycleMessageDispatcher.h"
#include "IInputMessageDispatcher.h"

namespace AppMessages
{
	class IAppMessageDispatcher
	{
	public:
		virtual ~IAppMessageDispatcher() {};

		virtual bool HandleMessage(const AppLifecycleMessages::AppPauseMessage& message) = 0;
		virtual bool HandleMessage(const AppLifecycleMessages::AppDisplayAvailableMessage& message) = 0;
		virtual bool HandleMessage(const InputMessages::TouchEventMessage& message) = 0;
	};
}

#endif /* IAPPMESSAGEDISPATCHER_H_ */
