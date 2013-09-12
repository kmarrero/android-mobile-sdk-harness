/*
 * AppOnMap.h
 *
 *  Created on: Mar 26, 2013
 *      Author: eegeo
 */

#ifndef APPONMAP_H_
#define APPONMAP_H_

#include "NewGlobeCamera.h"
#include "AndroidInputHandler.h"

class MyAndroidInputHandler : public Eegeo::Android::Input::IAndroidInputHandler
{
	Eegeo::Android::Input::AndroidInputHandler& inputHandler;
	Eegeo::Camera::NewGlobeCamera& globeCamera;

public:
	MyAndroidInputHandler(Eegeo::Android::Input::AndroidInputHandler* pInputHandler,
						Eegeo::Camera::NewGlobeCamera& globeCamera)
	:inputHandler(*pInputHandler)
	,globeCamera(globeCamera)
	{
		inputHandler.AddDelegateInputHandler(this);
	}

	virtual ~MyAndroidInputHandler()
	{
		inputHandler.RemoveDelegateInputHandler(this);
	}

	void Event_TouchRotate(const AppInterface::RotateData& data) {
		globeCamera.Event_TouchRotate(data);
	}
	void Event_TouchRotate_Start(const AppInterface::RotateData& data) {
		globeCamera.Event_TouchRotate_Start(data);
	}
	void Event_TouchRotate_End(const AppInterface::RotateData& data) {
		globeCamera.Event_TouchRotate_End(data);
	}

	void Event_TouchPinch(const AppInterface::PinchData& data) {
		globeCamera.Event_TouchPinch(data);
	}
	void Event_TouchPinch_Start(const AppInterface::PinchData& data) {
		globeCamera.Event_TouchPinch_Start(data);
	}
	void Event_TouchPinch_End(const AppInterface::PinchData& data) {
		globeCamera.Event_TouchPinch_End(data);
	}

	void Event_TouchPan(const AppInterface::PanData& data) {
		globeCamera.Event_TouchPan(data);
	}
	void Event_TouchPan_Start(const AppInterface::PanData& data) {
		globeCamera.Event_TouchPan_Start(data);
	}
	void Event_TouchPan_End(const AppInterface::PanData& data) {
		globeCamera.Event_TouchPan_End(data);
	}

	void Event_TouchTap(const AppInterface::TapData& data) {
		globeCamera.Event_TouchTap(data);
	}
	void Event_TouchDoubleTap(const AppInterface::TapData& data) {
		globeCamera.Event_TouchDoubleTap(data);
	}

	void Event_TouchDown(const AppInterface::TouchData& data) {
		globeCamera.Event_TouchDown(data);
	}
	void Event_TouchMove(const AppInterface::TouchData& data) {
		globeCamera.Event_TouchMove(data);
	}
	void Event_TouchUp(const AppInterface::TouchData& data) {
		globeCamera.Event_TouchUp(data);
	}

	bool Event_KeyPress(const AppInterface::KeyboardData& data) { return false; }
	void AddKeyPressListener(Eegeo::UI::NativeInput::IKeyboardInputKeyPressedHandler* handler) { }
	bool RemoveKeyPressListener(Eegeo::UI::NativeInput::IKeyboardInputKeyPressedHandler* handler) { return false; }
};

#endif /* APPONMAP_H_ */
