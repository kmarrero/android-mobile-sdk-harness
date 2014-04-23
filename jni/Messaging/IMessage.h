/*
 * IMessage.h
 *
 *  Created on: Apr 22, 2014
 */

#ifndef IMESSAGE_H_
#define IMESSAGE_H_

template<typename D>
class IMessage
{
public:
	virtual ~IMessage() {};
	virtual bool Handle(D& dispatcher) const = 0;
};

#endif /* IMESSAGE_H_ */
