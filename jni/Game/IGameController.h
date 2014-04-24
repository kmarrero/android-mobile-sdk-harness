/*
 * IGameController.h
 *
 *  Created on: Apr 24, 2014
 */

#ifndef IGAMECONTROLLER_H_
#define IGAMECONTROLLER_H_

namespace Game
{
	class IGameController
	{
	public:
		virtual ~IGameController() {};
		virtual size_t GetNumOfGames() const = 0;
		virtual void ActivateGame(GameId::Values gameId) = 0;
	};
}

#endif /* IGAMECONTROLLER_H_ */
