/*
 * AppHost.cpp
 *
 *  Created on: Apr 21, 2014
 */

#include "AppHost.h"
#include "AndroidWebRequestService.hpp"
#include "AndroidTaskQueue.h"
#include "LatLongAltitude.h"
#include "EegeoWorld.h"
#include "RenderContext.h"
#include "GlobalLighting.h"
#include "GlobalFogging.h"
#include "AppInterface.h"
#include "Blitter.h"
#include "EffectHandler.h"
#include "SearchServiceCredentials.h"
#include "AndroidThreadHelper.h"
#include "GlobeCameraController.h"
#include "RenderCamera.h"
#include "CameraHelpers.h"
#include "LoadingScreen.h"
#include "PlatformConfig.h"
#include "AndroidPlatformConfigBuilder.h"
#include "AndroidUrlEncoder.h"
#include "AndroidFileIO.h"
#include "AndroidLocationService.h"
#include "EegeoWorld.h"

using namespace Eegeo::Android;
using namespace Eegeo::Android::Input;

AppHost::AppHost(
		const std::string& apiKey,
		AndroidNativeState& nativeState,
		float displayWidth,
		float displayHeight,
	    EGLDisplay display,
	    EGLSurface shareSurface,
	    EGLContext resourceBuildShareContext
		)
:m_pTaskQueue(NULL)
,m_pEnvironmentFlatteningService(NULL)
,m_pAndroidWebLoadRequestFactory(NULL)
,m_pAndroidWebRequestService(NULL)
,m_pBlitter(NULL)
,m_pTextureLoader(NULL)
,m_pHttpCache(NULL)
,m_pFileIO(NULL)
,m_pLighting(NULL)
,m_pFogging(NULL)
,m_pShadowing(NULL)
,m_pRenderContext(NULL)
,m_pAndroidLocationService(NULL)
,m_pAndroidUrlEncoder(NULL)
,m_pWorld(NULL)
,m_pInterestPointProvider(NULL)
,m_androidInputBoxFactory(&nativeState)
,m_androidKeyboardInputFactory(&nativeState, m_inputHandler)
,m_androidAlertBoxFactory(&nativeState)
,m_androidNativeUIFactories(m_androidAlertBoxFactory, m_androidInputBoxFactory, m_androidKeyboardInputFactory)
,m_terrainHeightRepository()
,m_terrainHeightProvider(&m_terrainHeightRepository)
,m_pAppOnMap(NULL)
,m_pInputProcessor(NULL)
{
	Eegeo_ASSERT(resourceBuildShareContext != EGL_NO_CONTEXT);

	m_pAndroidUrlEncoder = new AndroidUrlEncoder(&nativeState);
	m_pAndroidLocationService = new AndroidLocationService(&nativeState);

	m_pRenderContext = new Eegeo::Rendering::RenderContext();
	m_pRenderContext->SetScreenDimensions(displayWidth, displayHeight, 1.0f, nativeState.deviceDpi);

	m_pLighting = new Eegeo::Lighting::GlobalLighting();
	m_pFogging = new Eegeo::Lighting::GlobalFogging();
	m_pShadowing = new Eegeo::Lighting::GlobalShadowing();
	m_pEnvironmentFlatteningService = new Eegeo::Rendering::EnvironmentFlatteningService();

	std::set<std::string> customApplicationAssetDirectories;
	customApplicationAssetDirectories.insert("MyAppDataDirectory");
	customApplicationAssetDirectories.insert("MyAppDataDirectory/MySubDirectory");
	m_pFileIO = new AndroidFileIO(&nativeState, customApplicationAssetDirectories);

	m_pHttpCache = new AndroidHttpCache(m_pFileIO, "http://d2xvsc8j92rfya.cloudfront.net/");
	m_pTextureLoader = new AndroidTextureFileLoader(m_pFileIO, m_pRenderContext->GetGLState());

	Eegeo::EffectHandler::Initialise();
	m_pBlitter = new Eegeo::Blitter(1024 * 128, 1024 * 64, 1024 * 32, *m_pRenderContext);
	m_pBlitter->Initialise();

	m_pTaskQueue = new AndroidTaskQueue(10, resourceBuildShareContext, shareSurface, display);

	const int webLoadPoolSize = 10;
	const int cacheLoadPoolSize = 40;
	const int cacheStorePoolSize = 20;
	m_pAndroidWebRequestService = new AndroidWebRequestService(*m_pFileIO, m_pHttpCache, m_pTaskQueue, webLoadPoolSize, cacheLoadPoolSize, cacheStorePoolSize);

	m_pAndroidWebLoadRequestFactory = new AndroidWebLoadRequestFactory(m_pAndroidWebRequestService, m_pHttpCache);

	m_pInterestPointProvider = new Eegeo::Camera::GlobeCamera::GlobeCameraInterestPointProvider();

	const Eegeo::EnvironmentCharacterSet::Type environmentCharacterSet = Eegeo::EnvironmentCharacterSet::Latin;
	std::string deviceModel = std::string(nativeState.deviceModel, strlen(nativeState.deviceModel));
	Eegeo::Config::PlatformConfig config = Eegeo::Android::AndroidPlatformConfigBuilder(deviceModel).Build();

	m_pWorld = new Eegeo::EegeoWorld(
			apiKey,
            m_pHttpCache,
            m_pFileIO,
            m_pTextureLoader,
            m_pAndroidWebLoadRequestFactory,
            m_pTaskQueue,
            *m_pRenderContext,
            m_pLighting,
            m_pFogging,
            m_pShadowing,
            m_pAndroidLocationService,
            m_pBlitter,
            m_pAndroidUrlEncoder,
            *m_pInterestPointProvider,
            m_androidNativeUIFactories,
            &m_terrainHeightRepository,
            &m_terrainHeightProvider,
            m_pEnvironmentFlatteningService,
            environmentCharacterSet,
            config,
            new Eegeo::Search::Service::SearchServiceCredentials("", ""),
            "",
            "Default-Landscape@2x~ipad.png",
            "http://cdn1.eegeo.com/coverage-trees/v207/manifest.txt.gz",
            "http://cdn1.eegeo.com/mobile-themes-new/v99/manifest.txt.gz",
            NULL,
            Eegeo::Rendering::LoadingScreenLayout::FullScreen
            );

	m_pAppOnMap = new MyApp(&m_inputHandler, nativeState, *m_pInterestPointProvider);
	m_pInputProcessor = new Eegeo::Android::Input::AndroidInputProcessor(&m_inputHandler, m_pRenderContext->GetScreenWidth(), m_pRenderContext->GetScreenHeight());

	m_pAppOnMap->Start(m_pWorld);
}

AppHost::~AppHost()
{
	m_pTaskQueue->StopWorkQueue();

	delete m_pAppOnMap;
	m_pAppOnMap = NULL;

	m_pHttpCache->FlushInMemoryCacheRepresentation();

    delete m_pInterestPointProvider;
    m_pInterestPointProvider = NULL;

    delete m_pWorld;
    m_pWorld = NULL;

    delete m_pAndroidUrlEncoder;
    m_pAndroidUrlEncoder = NULL;

    delete m_pAndroidLocationService;
    m_pAndroidLocationService = NULL;

    delete m_pRenderContext;
    m_pRenderContext = NULL;

    delete m_pShadowing;
    m_pShadowing = NULL;

    delete m_pFogging;
    m_pFogging = NULL;

    delete m_pLighting;
    m_pLighting = NULL;

    delete m_pFileIO;
    m_pFileIO = NULL;

    delete m_pHttpCache;
    m_pHttpCache = NULL;

    delete m_pTextureLoader;
    m_pTextureLoader = NULL;

    Eegeo::EffectHandler::Reset();
    Eegeo::EffectHandler::Shutdown();
    m_pBlitter->Shutdown();
    delete m_pBlitter;
    m_pBlitter = NULL;

    delete m_pAndroidWebRequestService;
    m_pAndroidWebRequestService = NULL;

    delete m_pAndroidWebLoadRequestFactory;
    m_pAndroidWebLoadRequestFactory = NULL;

    delete m_pEnvironmentFlatteningService;
    m_pEnvironmentFlatteningService = NULL;

    delete m_pTaskQueue;
    m_pTaskQueue = NULL;
}

void AppHost::OnStarted()
{
	m_pHttpCache->ReloadCacheRepresentationFromStorage();
}

void AppHost::OnStopped()
{
	m_pHttpCache->FlushInMemoryCacheRepresentation();
    m_pAndroidLocationService->StopListening();
}

void AppHost::StartGame(Game::GameId::Values gameId)
{
	m_pAppOnMap->ActivateGame(gameId);
}

void AppHost::SetSharedSurface(EGLSurface sharedSurface)
{
	m_pTaskQueue->UpdateSurface(sharedSurface);
}

void AppHost::SetViewportOffset(float x, float y)
{
	m_inputHandler.SetViewportOffset(x, y);
}

void AppHost::ProcessTouchInputEvent(const Eegeo::Android::Input::TouchInputEvent& event)
{
	m_pInputProcessor->HandleInput(event);
}

void AppHost::Update(float dt)
{
	m_pAndroidWebRequestService->Update();
	m_pAppOnMap->Update(dt);
	m_pAppOnMap->Draw(dt);
}




