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

class IGameController
{
public:
	virtual ~IGameController() {};
	virtual size_t GetNumOfGames() const = 0;
	virtual void ActivateGame(GameId::Values gameId) = 0;
};

class IGame
{
public:
	virtual ~IGame() {};

	virtual void OnActivated() = 0;
	virtual void OnDeactivated() = 0;

	virtual void OnUpdate(float dt) = 0;
	virtual void OnDraw(float dt) = 0;
};

class IGameFactory
{
public:
	virtual ~IGameFactory() {};
	virtual IGame* CreateGame() const = 0;
};

class Game : public IGame, protected Eegeo::NonCopyable
{
public:
	Game(const std::string& name)
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

private:
	const std::string m_name;
};

class MyApp : public Eegeo::IAppOnMap, public Eegeo::Android::Input::IAndroidInputHandler, public IGameController
{
private:
	Eegeo::Android::Input::AndroidInputHandler& pInputHandler;
	AndroidNativeState& m_nativeState;
    Eegeo::Camera::GlobeCamera::GlobeCameraController* m_globeCameraController;
    Eegeo::Camera::GlobeCamera::GlobeCameraTouchController* m_cameraTouchController;
    Eegeo::Camera::ICameraJumpController* m_cameraJumpController;
    Eegeo::Camera::GlobeCamera::GlobeCameraInterestPointProvider& m_globeCameraInterestPointProvider;

    typedef std::vector<IGame*> IGamePtrVec;
    IGamePtrVec m_games;
    IGame* m_pActiveGame;

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

    Eegeo::Camera::GlobeCamera::GlobeCameraController& GetCameraController() { return *m_globeCameraController; }

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
