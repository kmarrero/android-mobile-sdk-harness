/*
 * AppStartGameMessage.cpp
 *
 *  Created on: Apr 24, 2014
 */

#include "AppStartGameMessage.h"
#include "IAppMessageDispatcher.h"

namespace AppMessages
{
	AppStartGameMessage::AppStartGameMessage(GameId::Values gameId)
	: m_gameId(gameId)
	{
	}

	bool AppStartGameMessage::Handle(AppMessages::IAppMessageDispatcher& dispatcher) const
	{
		return dispatcher.HandleMessage(*this);
	}

	GameId::Values AppStartGameMessage::GetGameId() const
	{
		return m_gameId;
	}
}





