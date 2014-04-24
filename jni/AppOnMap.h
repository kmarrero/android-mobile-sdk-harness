#ifndef __ExampleApp__AppOnMap__
#define __ExampleApp__AppOnMap__

#include "IAppOnMap.h"
#include "Rendering.h"
#include "Space.h"
#include "Terrain.h"
#include "Roads.h"
#include "Navigation.h"
#include "Streaming.h"
#include "Traffic.h"
#include "Lighting.h"
#include "Helpers.h"
#include "Web.h"
#include "Location.h"
#include "AndroidInputHandler.h"
#include "AndroidNativeState.h"
#include "GlobeCamera.h"
#include "GameId.h"
#include "IGame.h"
#include "IGameController.h"

class MyApp : public Eegeo::IAppOnMap, public Eegeo::Android::Input::IAndroidInputHandler, public Game::IGameController
{
private:
	Eegeo::Android::Input::AndroidInputHandler& pInputHandler;
	AndroidNativeState& m_nativeState;
    Eegeo::Camera::GlobeCamera::GlobeCameraController* m_pGlobeCameraController;
    Eegeo::Camera::GlobeCamera::GlobeCameraTouchController* m_pCameraTouchController;
    Eegeo::Camera::ICameraJumpController* m_pCameraJumpController;
    Eegeo::Camera::GlobeCamera::GlobeCameraInterestPointProvider& m_globeCameraInterestPointProvider;

    typedef std::vector<Game::IGame*> IGamePtrVec;
    IGamePtrVec m_games;
    Game::IGame* m_pActiveGame;

    void CreateGames();

public:
	MyApp
	(
		Eegeo::Android::Input::AndroidInputHandler* inputHandler,
		AndroidNativeState& nativeState,
		Eegeo::Camera::GlobeCamera::GlobeCameraInterestPointProvider& globeCameraInterestPointProvider
	);

	virtual ~MyApp();

	void OnStart ();

    void Update (float dt);

    void Draw (float dt);

    void JumpTo(double latitudeDegrees, double longitudeDegrees, double altitudeMetres, double headingDegrees, double distanceToInterestMetres);

    size_t GetNumOfGames() const;
    void ActivateGame(GameId::Values gameId);

    Eegeo::Camera::GlobeCamera::GlobeCameraController& GetCameraController() { return *m_pGlobeCameraController; }

    void Event_TouchRotate 			(const AppInterface::RotateData& data);
    void Event_TouchRotate_Start	(const AppInterface::RotateData& data);
    void Event_TouchRotate_End 		(const AppInterface::RotateData& data);

    void Event_TouchPinch 			(const AppInterface::PinchData& data);
    void Event_TouchPinch_Start 	(const AppInterface::PinchData& data);
    void Event_TouchPinch_End 		(const AppInterface::PinchData& data);

    void Event_TouchPan				(const AppInterface::PanData& data);
    void Event_TouchPan_Start		(const AppInterface::PanData& data);
    void Event_TouchPan_End 		(const AppInterface::PanData& data);

    void Event_TouchTap 			(const AppInterface::TapData& data);
    void Event_TouchDoubleTap		(const AppInterface::TapData& data);

    void Event_TouchDown 			(const AppInterface::TouchData& data);
    void Event_TouchMove 			(const AppInterface::TouchData& data);
    void Event_TouchUp 				(const AppInterface::TouchData& data);

	bool Event_KeyPress(const AppInterface::KeyboardData& data) { return false; }
	void AddKeyPressListener(Eegeo::UI::NativeInput::IKeyboardInputKeyPressedHandler* handler) { }
	bool RemoveKeyPressListener(Eegeo::UI::NativeInput::IKeyboardInputKeyPressedHandler* handler) { return false; }
};

#endif /* defined(__ExampleApp__AppOnMap__) */
