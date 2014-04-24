/*
 * TestGame.h
 *
 *  Created on: Apr 24, 2014
 */

#ifndef GAME_H_
#define GAME_H_

#include "Types.h"
#include "IGame.h"

namespace Game
{
	class TestGame : public IGame, protected Eegeo::NonCopyable
	{
	public:
		TestGame(const std::string& name)
		: m_name(name)
		{
			Eegeo_TTY("Created %s game\n", m_name.c_str());
		}

		void OnActivated()
		{
			Eegeo_TTY("%s game activate\n", m_name.c_str());
		}

		void OnDeactivated()
		{
			Eegeo_TTY("%s game deactivate\n", m_name.c_str());
		}

		void OnUpdate(float dt)
		{
			Eegeo_TTY("%s game update\n", m_name.c_str());
		}

		void OnDraw(float dt)
		{
			Eegeo_TTY("%s game draw\n", m_name.c_str());
		}

		void Event_TouchRotate 			(const AppInterface::RotateData& data)
		{
			Eegeo_TTY("%s game received Event_TouchRotate\n", m_name.c_str());
		}

		void Event_TouchRotate_Start	(const AppInterface::RotateData& data)
		{
			Eegeo_TTY("%s game received Event_TouchRotate_Start\n", m_name.c_str());
		}

		void Event_TouchRotate_End 		(const AppInterface::RotateData& data)
		{
			Eegeo_TTY("%s game received Event_TouchRotate_End\n", m_name.c_str());
		}

		void Event_TouchPinch 			(const AppInterface::PinchData& data)
		{
			Eegeo_TTY("%s game received Event_TouchPinch\n", m_name.c_str());
		}

		void Event_TouchPinch_Start 	(const AppInterface::PinchData& data)
		{
			Eegeo_TTY("%s game received Event_TouchPinch_Start\n", m_name.c_str());
		}

		void Event_TouchPinch_End 		(const AppInterface::PinchData& data)
		{
			Eegeo_TTY("%s game received Event_TouchPinch_End\n", m_name.c_str());
		}

		void Event_TouchPan				(const AppInterface::PanData& data)
		{
			Eegeo_TTY("%s game received Event_TouchPan\n", m_name.c_str());
		}

		void Event_TouchPan_Start		(const AppInterface::PanData& data)
		{
			Eegeo_TTY("%s game received Event_TouchPan_Start\n", m_name.c_str());
		}

		void Event_TouchPan_End 		(const AppInterface::PanData& data)
		{
			Eegeo_TTY("%s game received Event_TouchPan_End\n", m_name.c_str());
		}

		void Event_TouchTap				(const AppInterface::TapData& data)
		{
			Eegeo_TTY("%s game received Event_TouchTap\n", m_name.c_str());
		}

		void Event_TouchDoubleTap		(const AppInterface::TapData& data)
		{
			Eegeo_TTY("%s game received Event_TouchDoubleTap\n", m_name.c_str());
		}

		void Event_TouchDown 			(const AppInterface::TouchData& data)
		{
			Eegeo_TTY("%s game received Event_TouchDown\n", m_name.c_str());
		}

		void Event_TouchMove 			(const AppInterface::TouchData& data)
		{
			Eegeo_TTY("%s game received Event_TouchMove\n", m_name.c_str());
		}

		void Event_TouchUp 				(const AppInterface::TouchData& data)
		{
			Eegeo_TTY("%s game received Event_TouchUp\n", m_name.c_str());
		}

		bool Event_KeyPress(const AppInterface::KeyboardData& data)
		{
			Eegeo_TTY("%s game received Event_KeyPress\n", m_name.c_str());
			return false;
		}

		void AddKeyPressListener(Eegeo::UI::NativeInput::IKeyboardInputKeyPressedHandler* handler)
		{
		}

		bool RemoveKeyPressListener(Eegeo::UI::NativeInput::IKeyboardInputKeyPressedHandler* handler)
		{
			return false;
		};

	private:
		const std::string m_name;
	};
}

#endif /* GAME_H_ */
