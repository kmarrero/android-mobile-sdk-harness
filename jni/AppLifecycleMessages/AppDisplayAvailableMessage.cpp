/*
 * AppDisplayAvailableMessage.cpp
 *
 *  Created on: Apr 23, 2014
 */

#include "AppDisplayAvailableMessage.h"
#include "IAppMessageDispatcher.h"

namespace AppLifecycleMessages
{
	bool AppDisplayAvailableMessage::Handle(AppMessages::IAppMessageDispatcher& dispatcher) const
	{
		return dispatcher.HandleMessage(*this);
	}
}




