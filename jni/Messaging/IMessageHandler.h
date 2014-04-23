/*
 * IMessageHandler.h
 *
 *  Created on: Apr 22, 2014
 *      Author: olivernorton
 */

#ifndef IMESSAGEHANDLER_H_
#define IMESSAGEHANDLER_H_

template<typename M>
class IMessageHandler
{
public:
	virtual ~IMessageHandler() {};
	virtual bool HandleMessage(const M& message) = 0;
};

#endif /* IMESSAGEHANDLER_H_ */
