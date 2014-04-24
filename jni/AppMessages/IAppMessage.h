/*
 * IAppMessage.h
 *
 *  Created on: Apr 22, 2014
 */

#ifndef IAPPMESSAGE_H_
#define IAPPMESSAGE_H_

#include "IMessage.h"
#include "IAppMessageDispatcher.h"

namespace AppMessages
{
	class IAppMessage : public Eegeo::Messaging::IMessage<IAppMessageDispatcher>
	{
	};
}

#endif /* IAPPMESSAGE_H_ */
