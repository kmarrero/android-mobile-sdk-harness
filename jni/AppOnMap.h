#ifndef __ExampleApp__AppOnMap__
#define __ExampleApp__AppOnMap__

#include "IAppOnMap.h"
#include "IExample.h"
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

namespace ExampleTypes
{
	enum Examples
	{
		DebugSphere=0,
		ScreenUnproject,
		ScreenPick,
		TerrainHeightQuery,
		LoadModel,
		EnvironmentNotifier,
		FileIO,
		WebRequest,
		NavigationGraph,
        ModifiedRendering,
        ToggleTraffic,
        ResourceSpatialQuery,
        EnvironmentFlattening,
        Search,
        KeyboardInput,
        PODAnimation,
        Pick3DObject,
        ShowJavaPlaceJumpUI,
        PositionJavaPinButton,
        DebugPrimitiveRendering,
        ControlCityThemes,
        RouteDrawing,
        Pins,
        PinsWithAttachedJavaUI,
        RouteSimulation,
        RouteThicknessPolicy,
        JavaHudCrossThreadCommunication,
        RouteMatching,
        RouteSimulationAnimation,
        DynamicText3D,
        SingleCity,
        PinOverModel,
        TrafficCongestion
	};
}

static const ExampleTypes::Examples g_defaultSelectedExample = ExampleTypes::JavaHudCrossThreadCommunication;

namespace Eegeo
{
    class ITouchController;
    class Blitter;

    namespace Search
    {
        namespace Service
        {
            class SearchService;
        }
    }

    namespace UI
    {
        class NativeUIFactories;
    }

    namespace Routes
    {
        class RouteService;
    }
}

class IGameController
{
public:
	virtual ~IGameController() {};
	virtual size_t GetNumOfGames() const = 0;
	virtual void ActivateGame(int gameIndex) = 0;
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
	Examples::IExample *pExample;
	Eegeo::Android::Input::AndroidInputHandler& pInputHandler;
	AndroidNativeState& m_nativeState;
    ExampleTypes::Examples m_selectedExampleType;
    Eegeo::Camera::GlobeCamera::GlobeCameraController* m_globeCameraController;
    Eegeo::Camera::GlobeCamera::GlobeCameraTouchController* m_cameraTouchController;
    Eegeo::Camera::ICameraJumpController* m_cameraJumpController;
    Eegeo::Camera::GlobeCamera::GlobeCameraInterestPointProvider& m_globeCameraInterestPointProvider;

    typedef std::vector<IGame*> IGamePtrVec;
    IGamePtrVec m_games;
    IGame* m_pActiveGame;

public:
	MyApp(
			Eegeo::Android::Input::AndroidInputHandler* inputHandler,
			AndroidNativeState& nativeState,
			Eegeo::Camera::GlobeCamera::GlobeCameraInterestPointProvider& globeCameraInterestPointProvider,
			ExampleTypes::Examples selectedExample = g_defaultSelectedExample);

	virtual ~MyApp();

	void OnStart ();

    void Update (float dt);

    void Draw (float dt);

    void JumpTo(double latitudeDegrees, double longitudeDegrees, double altitudeMetres, double headingDegrees, double distanceToInterestMetres);

    size_t GetNumOfGames() const;
    void ActivateGame(int gameIndex);

    Eegeo::Camera::GlobeCamera::GlobeCameraController& GetCameraController() { return *m_globeCameraController; }

    Examples::IExample* CreateExample(ExampleTypes::Examples example,
                                      Eegeo::Rendering::RenderContext& renderContext,
                                      Eegeo::Space::LatLongAltitude interestLocation,
                                      Eegeo::Camera::ICameraProvider& cameraProvider,
                                      Eegeo::Camera::GlobeCamera::GlobeCameraController& globeCameraController,
                                      Eegeo::ITouchController& cameraTouchController,
                                      Eegeo::Resources::Terrain::Heights::TerrainHeightProvider& terrainHeightProvider,
                                      Eegeo::Helpers::ITextureFileLoader& textureLoader,
                                      Eegeo::Helpers::IFileIO& fileIO,
                                      Eegeo::Resources::Terrain::TerrainStreaming& terrainStreaming,
                                      Eegeo::Web::IWebLoadRequestFactory& webRequestFactory,
                                      Eegeo::Resources::Roads::Navigation::NavigationGraphRepository& navigationGraphs,
                                      Eegeo::Rendering::Scene::SceneElementRepository<Eegeo::Rendering::Renderables::PackedRenderable>& buildingRepository,
                                      Eegeo::Rendering::Filters::PackedRenderableFilter& buildingFilter,
                                      Eegeo::Rendering::Scene::SceneElementRepository<Eegeo::Rendering::Renderables::PackedRenderable>& shadowRepository,
                                      Eegeo::Rendering::Filters::PackedRenderableFilter& shadowFilter,
                                      Eegeo::Streaming::IStreamingVolume& visibleVolume,
                                      Eegeo::Lighting::GlobalLighting& lighting,
                                      Eegeo::Lighting::GlobalFogging& fogging,
                                      Eegeo::Traffic::TrafficSimulationController& trafficSimulation,
                                      Eegeo::Resources::ResourceSpatialQueryService& resourceSpatialQueryService,
                                      Eegeo::Rendering::EnvironmentFlatteningService& environmentFlatteningService,
                                      Eegeo::Search::Service::SearchService* searchService,
                                      Eegeo::UI::NativeUIFactories& nativeInputFactories,
                                      Eegeo::Location::IInterestPointProvider& interestPointProvider,
                                      Eegeo::Routes::RouteService& routeService,
                                      const Eegeo::Resources::Terrain::Collision::ICollisionMeshResourceProvider& collisionMeshResourceProvider,
                                      Eegeo::EegeoWorld& world
                                      );

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
