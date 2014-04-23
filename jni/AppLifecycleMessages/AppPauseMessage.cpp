/*
 * AppPauseMessage.cpp
 *
 *  Created on: Apr 23, 2014
 */

#include "AppPauseMessage.h"
#include "IAppMessageDispatcher.h"

namespace AppLifecycleMessages
{
	bool AppPauseMessage::Handle(AppMessages::IAppMessageDispatcher& dispatcher) const
	{
		return dispatcher.HandleMessage(*this);
	}
}





