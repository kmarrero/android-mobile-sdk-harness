/*
 * DefaultGame.h
 *
 *  Created on: Apr 24, 2014
 */

#ifndef DEFAULTGAME_H_
#define DEFAULTGAME_H_

#include "Types.h"
#include "IGame.h"
#include "GlobeCamera.h"

namespace Game
{
	class DefaultGame : public IGame, protected Eegeo::NonCopyable
	{
	public:
		DefaultGame(Eegeo::Camera::GlobeCamera::GlobeCameraTouchController& cameraTouchController);

		void OnActivated();
		void OnDeactivated();
		void OnUpdate(float dt);
		void OnDraw(float dt);

		void Event_TouchRotate 			(const AppInterface::RotateData& data);
		void Event_TouchRotate_Start	(const AppInterface::RotateData& data);
		void Event_TouchRotate_End 		(const AppInterface::RotateData& data);
		void Event_TouchPinch 			(const AppInterface::PinchData& data);
		void Event_TouchPinch_Start 	(const AppInterface::PinchData& data);
		void Event_TouchPinch_End 		(const AppInterface::PinchData& data);
		void Event_TouchPan				(const AppInterface::PanData& data);
		void Event_TouchPan_Start		(const AppInterface::PanData& data);
		void Event_TouchPan_End 		(const AppInterface::PanData& data);
		void Event_TouchTap				(const AppInterface::TapData& data);
		void Event_TouchDoubleTap		(const AppInterface::TapData& data);
		void Event_TouchDown 			(const AppInterface::TouchData& data);
		void Event_TouchMove 			(const AppInterface::TouchData& data);
		void Event_TouchUp 				(const AppInterface::TouchData& data);

		bool Event_KeyPress(const AppInterface::KeyboardData& data);
		void AddKeyPressListener(Eegeo::UI::NativeInput::IKeyboardInputKeyPressedHandler* handler);
		bool RemoveKeyPressListener(Eegeo::UI::NativeInput::IKeyboardInputKeyPressedHandler* handler);

	private:
		Eegeo::Camera::GlobeCamera::GlobeCameraTouchController& m_cameraTouchController;
	};
}
#endif /* DEFAULTGAME_H_ */
