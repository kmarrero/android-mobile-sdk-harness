/*
 * StartGameMessage.h
 *
 *  Created on: Apr 23, 2014
 */

#ifndef STARTGAMEMESSAGE_H_
#define STARTGAMEMESSAGE_H_

#include "AppMessages.h"
#include "IAppMessage.h"
#include "GameId.h"

namespace AppMessages
{
	class AppStartGameMessage : public AppMessages::IAppMessage
	{
	public:
		AppStartGameMessage(Game::GameId::Values gameId);
		bool Handle(AppMessages::IAppMessageDispatcher& dispatcher) const;
		Game::GameId::Values GetGameId() const;

	private:
		Game::GameId::Values m_gameId;
	};
}

#endif /* STARTGAMEMESSAGE_H_ */
