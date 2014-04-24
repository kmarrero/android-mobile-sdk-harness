/*
 * AppOnMap.cpp
 *
 *  Created on: Nov 5, 2013
 */

#include "AppOnMap.h"
#include "EegeoWorld.h"
#include "AndroidInputHandler.h"
#include "AndroidNativeState.h"
#include "GlobeCameraTouchController.h"
#include "RenderCamera.h"
#include "GlobeCameraController.h"
#include "GlobeCameraInterestPointProvider.h"
#include "GlobeCameraController.h"
#include "CameraHelpers.h"
#include "NativeUIFactories.h"
#include "LocalAsyncTextureLoader.h"
#include "ExampleCameraJumpController.h"
#include "TestGame.h"
#include "DefaultGame.h"

MyApp::MyApp
(
	Eegeo::Android::Input::AndroidInputHandler* inputHandler,
	AndroidNativeState& nativeState,
	Eegeo::Camera::GlobeCamera::GlobeCameraInterestPointProvider& globeCameraInterestPointProvider
)
: pInputHandler(*inputHandler)
, m_nativeState(nativeState)
, m_globeCameraInterestPointProvider(globeCameraInterestPointProvider)
, m_pGlobeCameraController(NULL)
, m_pCameraTouchController(NULL)
, m_pCameraJumpController(NULL)
, m_pActiveGame(NULL)
{
	Eegeo_ASSERT(&m_globeCameraInterestPointProvider != NULL);
	pInputHandler.AddDelegateInputHandler(this);
}

MyApp::~MyApp()
{
	for(IGamePtrVec::iterator it = m_games.begin(); it != m_games.end(); ++it)
	{
		Eegeo_DELETE(*it);
	}
	m_games.clear();

	pInputHandler.RemoveDelegateInputHandler(this);
    delete m_pGlobeCameraController;
    delete m_pCameraTouchController;
    delete m_pCameraJumpController;
}

void MyApp::CreateGames()
{
	Eegeo_ASSERT(m_pActiveGame == NULL);
	Eegeo_ASSERT(m_games.size() == 0);
	Eegeo_ASSERT(m_pCameraTouchController != NULL);

	Game::IGame* pDefaultGame = Eegeo_NEW(Game::DefaultGame)(*m_pCameraTouchController);
	m_games.push_back(pDefaultGame);

	Game::IGame* pScavengerHuntGame = Eegeo_NEW(Game::TestGame)("Scavenger Hunt");
	m_games.push_back(pScavengerHuntGame);

	Game::IGame* pHelicopterGame = Eegeo_NEW(Game::TestGame)("Helicopter");
	m_games.push_back(pHelicopterGame);
}

void MyApp::OnStart ()
{
    Eegeo_ASSERT(&World() != NULL);

    Eegeo::EegeoWorld& eegeoWorld = World();


    Eegeo::Camera::GlobeCamera::GlobeCameraTouchControllerConfiguration touchControllerConfig = Eegeo::Camera::GlobeCamera::GlobeCameraTouchControllerConfiguration::CreateDefault();

    // override default configuration to enable two-finger pan gesture to control additional camera pitch
    touchControllerConfig.tiltEnabled = true;

    m_pCameraTouchController = new Eegeo::Camera::GlobeCamera::GlobeCameraTouchController(touchControllerConfig);

    const bool useLowSpecSettings = false;
    Eegeo::Camera::GlobeCamera::GlobeCameraControllerConfiguration globeCameraControllerConfig = Eegeo::Camera::GlobeCamera::GlobeCameraControllerConfiguration::CreateDefault(useLowSpecSettings);

    m_pGlobeCameraController = new Eegeo::Camera::GlobeCamera::GlobeCameraController(eegeoWorld.GetTerrainHeightProvider(),
                                                                                    eegeoWorld.GetEnvironmentFlatteningService(),
                                                                                    eegeoWorld.GetResourceCeilingProvider(),
                                                                                    *m_pCameraTouchController,
                                                                                    globeCameraControllerConfig);

    m_pCameraJumpController = new ExampleCameraJumpController(*m_pGlobeCameraController, *m_pCameraTouchController);

    Eegeo::Camera::RenderCamera* renderCamera = m_pGlobeCameraController->GetCamera();
    const Eegeo::Rendering::RenderContext& renderContext = eegeoWorld.GetRenderContext();
    renderCamera->SetViewport(0.f, 0.f, renderContext.GetScreenWidth(), renderContext.GetScreenHeight());
    eegeoWorld.SetCamera(renderCamera);

    m_globeCameraInterestPointProvider.SetGlobeCamera(m_pGlobeCameraController);

    float interestPointLatitudeDegrees = 37.7858f;
    float interestPointLongitudeDegrees = -122.401f;
    float interestPointAltitudeMeters = 2.7;

    Eegeo::Space::LatLongAltitude location = Eegeo::Space::LatLongAltitude::FromDegrees(interestPointLatitudeDegrees,
                                                                                        interestPointLongitudeDegrees,
                                                                                        interestPointAltitudeMeters);



    float cameraControllerOrientationDegrees = 0.0f;
    float cameraControllerDistanceFromInterestPointMeters = 1781.0f;

    Eegeo::Space::EcefTangentBasis cameraInterestBasis;
    Eegeo::Camera::CameraHelpers::EcefTangentBasisFromPointAndHeading(location.ToECEF(), cameraControllerOrientationDegrees, cameraInterestBasis);

    m_pGlobeCameraController->SetView(cameraInterestBasis, cameraControllerDistanceFromInterestPointMeters);

    Eegeo::Search::Service::SearchService* searchService = NULL;

    if (eegeoWorld.IsSearchServiceAvailable())
    {
        searchService = &eegeoWorld.GetSearchService();
    }

    CreateGames();
    ActivateGame(Game::GameId::Default);
}

void MyApp::Update (float dt)
{
    Eegeo::EegeoWorld& eegeoWorld = World();

    eegeoWorld.EarlyUpdate(dt);

    m_pCameraTouchController->Update(dt);
    m_pGlobeCameraController->Update(dt);

    eegeoWorld.Update(dt);

    if(m_pActiveGame != NULL)
    {
    	m_pActiveGame->OnUpdate(dt);
    }
}

void MyApp::Draw (float dt)
{
    Eegeo::Rendering::GLState& glState = World().GetRenderContext().GetGLState();
    glState.ClearColor(0.8f, 0.8f, 0.8f, 1.f);
    World().Draw(dt);

    if(m_pActiveGame != NULL)
    {
    	m_pActiveGame->OnDraw(dt);
    }
}

void MyApp::JumpTo(double latitudeDegrees, double longitudeDegrees, double altitudeMetres, double headingDegrees, double distanceToInterestMetres)
{
    Eegeo::dv3 interestPoint = Eegeo::Space::LatLongAltitude::FromDegrees(latitudeDegrees, longitudeDegrees, altitudeMetres).ToECEF();

    Eegeo::Space::EcefTangentBasis interestBasis;

    Eegeo::Camera::CameraHelpers::EcefTangentBasisFromPointAndHeading(interestPoint, headingDegrees, interestBasis);

    m_pGlobeCameraController->SetView(interestBasis, distanceToInterestMetres);
}

void MyApp::Event_TouchRotate(const AppInterface::RotateData& data)
{
	if(m_pActiveGame != NULL)
	{
		m_pActiveGame->Event_TouchRotate(data);

	}
}

void MyApp::Event_TouchRotate_Start(const AppInterface::RotateData& data)
{
	if(m_pActiveGame != NULL)
    {
		m_pActiveGame->Event_TouchRotate_Start(data);
    }
}

void MyApp::Event_TouchRotate_End(const AppInterface::RotateData& data)
{
	if(m_pActiveGame != NULL)
    {
        m_pActiveGame->Event_TouchRotate_End(data);
    }
}

void MyApp::Event_TouchPinch(const AppInterface::PinchData& data)
{
	if(m_pActiveGame != NULL)
    {
		m_pActiveGame->Event_TouchPinch(data);
    }
}

void MyApp::Event_TouchPinch_Start(const AppInterface::PinchData& data)
{
	if(m_pActiveGame != NULL)
	{
		m_pActiveGame->Event_TouchPinch_Start(data);
    }
}

void MyApp::Event_TouchPinch_End(const AppInterface::PinchData& data)
{
	if(m_pActiveGame != NULL)
    {
		m_pActiveGame->Event_TouchPinch_End(data);
    }
}

void MyApp::Event_TouchPan(const AppInterface::PanData& data)
{
	if(m_pActiveGame != NULL)
    {
		m_pActiveGame->Event_TouchPan(data);
    }
}

void MyApp::Event_TouchPan_Start(const AppInterface::PanData& data)
{
	if(m_pActiveGame != NULL)
    {
		m_pActiveGame->Event_TouchPan_Start(data);
    }
}

void MyApp::Event_TouchPan_End(const AppInterface::PanData& data)
{
	if(m_pActiveGame != NULL)
    {
		m_pActiveGame->Event_TouchPan_End(data);
    }
}

void MyApp::Event_TouchTap(const AppInterface::TapData& data)
{
	if(m_pActiveGame != NULL)
    {
		m_pActiveGame->Event_TouchTap(data);
    }
}

void MyApp::Event_TouchDoubleTap(const AppInterface::TapData& data)
{
	if(m_pActiveGame != NULL)
    {
		m_pActiveGame->Event_TouchDoubleTap(data);
    }
}

void MyApp::Event_TouchDown(const AppInterface::TouchData& data)
{
	if(m_pActiveGame != NULL)
    {
		m_pActiveGame->Event_TouchDown(data);
    }
}

void MyApp::Event_TouchMove(const AppInterface::TouchData& data)
{
	if(m_pActiveGame != NULL)
    {
		m_pActiveGame->Event_TouchMove(data);
    }
}

void MyApp::Event_TouchUp(const AppInterface::TouchData& data)
{
	if(m_pActiveGame != NULL)
    {
        m_pActiveGame->Event_TouchUp(data);
    }
}

void MyApp::ActivateGame(Game::GameId::Values gameIndex)
{
	Game::IGame* pNewGame = m_games.at(gameIndex);

	if(pNewGame != m_pActiveGame)
	{
		if(m_pActiveGame != NULL)
		{
			m_pActiveGame->OnDeactivated();
		}
		m_pActiveGame = pNewGame;
		m_pActiveGame->OnActivated();
	}
}

size_t MyApp::GetNumOfGames() const
{
	return m_games.size();
}

