/*
 * IGame.h
 *
 *  Created on: Apr 24, 2014
 */

#ifndef IGAME_H_
#define IGAME_H_

#include "IAndroidInputHandler.h"

namespace Game
{
	class IGame : public Eegeo::Android::Input::IAndroidInputHandler
	{
	public:
		virtual ~IGame() {};

		virtual void OnActivated() = 0;
		virtual void OnDeactivated() = 0;

		virtual void OnUpdate(float dt) = 0;
		virtual void OnDraw(float dt) = 0;
	};
}

#endif /* IGAME_H_ */
