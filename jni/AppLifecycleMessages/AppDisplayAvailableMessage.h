/*
 * AppDisplayAvailableMessage.h
 *
 *  Created on: Apr 22, 2014
 */

#ifndef APPDISPLAYAVAILABLEMESSAGE_H_
#define APPDISPLAYAVAILABLEMESSAGE_H_

#include "AppMessages.h"
#include "IAppMessage.h"

namespace AppLifecycleMessages
{
	class AppDisplayAvailableMessage : public AppMessages::IAppMessage
	{
	public:
		bool Handle(AppMessages::IAppMessageDispatcher& dispatcher) const;
	};
}

#endif /* APPDISPLAYAVAILABLEMESSAGE_H_ */
