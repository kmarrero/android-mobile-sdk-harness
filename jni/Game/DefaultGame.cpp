/*
 * DefaultGame.cpp
 *
 *  Created on: Apr 24, 2014
 */

#include "DefaultGame.h"
#include "GlobeCameraTouchController.h"

namespace Game
{
	DefaultGame::DefaultGame(Eegeo::Camera::GlobeCamera::GlobeCameraTouchController& cameraTouchController)
	: m_cameraTouchController(cameraTouchController)
	{
	}

	void DefaultGame::OnActivated()
	{
	}

	void DefaultGame::OnDeactivated()
	{
	}

	void DefaultGame::OnUpdate(float dt)
	{
	}

	void DefaultGame::OnDraw(float dt)
	{
	}

	void DefaultGame::Event_TouchRotate 			(const AppInterface::RotateData& data)
	{
		m_cameraTouchController.Event_TouchRotate(data);
	}

	void DefaultGame::Event_TouchRotate_Start	(const AppInterface::RotateData& data)
	{
		m_cameraTouchController.Event_TouchRotate_Start(data);
	}

	void DefaultGame::Event_TouchRotate_End 		(const AppInterface::RotateData& data)
	{
		m_cameraTouchController.Event_TouchRotate_End(data);
	}

	void DefaultGame::Event_TouchPinch 			(const AppInterface::PinchData& data)
	{
		m_cameraTouchController.Event_TouchPinch(data);
	}

	void DefaultGame::Event_TouchPinch_Start 	(const AppInterface::PinchData& data)
	{
		m_cameraTouchController.Event_TouchPinch_Start(data);
	}

	void DefaultGame::Event_TouchPinch_End 		(const AppInterface::PinchData& data)
	{
		m_cameraTouchController.Event_TouchPinch_End(data);
	}

	void DefaultGame::Event_TouchPan				(const AppInterface::PanData& data)
	{
		m_cameraTouchController.Event_TouchPan(data);
	}

	void DefaultGame::Event_TouchPan_Start		(const AppInterface::PanData& data)
	{
		m_cameraTouchController.Event_TouchPan_Start(data);
	}

	void DefaultGame::Event_TouchPan_End 		(const AppInterface::PanData& data)
	{
		m_cameraTouchController.Event_TouchPan_End(data);
	}

	void DefaultGame::Event_TouchTap				(const AppInterface::TapData& data)
	{
		m_cameraTouchController.Event_TouchTap(data);
	}

	void DefaultGame::Event_TouchDoubleTap		(const AppInterface::TapData& data)
	{
		m_cameraTouchController.Event_TouchDoubleTap(data);
	}

	void DefaultGame::Event_TouchDown 			(const AppInterface::TouchData& data)
	{
		m_cameraTouchController.Event_TouchDown(data);
	}

	void DefaultGame::Event_TouchMove 			(const AppInterface::TouchData& data)
	{
		m_cameraTouchController.Event_TouchMove(data);
	}

	void DefaultGame::Event_TouchUp 				(const AppInterface::TouchData& data)
	{
		m_cameraTouchController.Event_TouchUp(data);
	}

	bool DefaultGame::Event_KeyPress(const AppInterface::KeyboardData& data)
	{
		return false;
	}

	void DefaultGame::AddKeyPressListener(Eegeo::UI::NativeInput::IKeyboardInputKeyPressedHandler* handler)
	{
	}

	bool DefaultGame::RemoveKeyPressListener(Eegeo::UI::NativeInput::IKeyboardInputKeyPressedHandler* handler)
	{
		return false;
	};
}
