#include <stdio.h>

#include "AppWindow.hpp"
#include "AndroidWebRequestService.hpp"
#include "AndroidTaskQueue.h"
#include "LatLongAltitude.h"
#include "EegeoWorld.h"
#include "RenderContext.h"
#include "GlobalLighting.h"
#include "GlobalFogging.h"
#include "DefaultMaterialFactory.h"
#include "AppInterface.h"
#include "Blitter.h"
#include "EffectHandler.h"
#include "VehicleModelLoader.h"
#include "VehicleModelRepository.h"
#include "SearchServiceCredentials.h"
#include "AndroidThreadHelper.h"
#include "GlobeCameraController.h"
#include "RenderCamera.h"
#include "CameraHelpers.h"

using namespace Eegeo::Android;
using namespace Eegeo::Android::Input;

#define API_KEY "OBTAIN API_KEY FROM https://appstore.eegeo.com AND INSERT IT HERE"

void AppWindow::EnqueuePointerEvent(TouchInputEvent& e)
{
	pthread_mutex_lock(&m_inputMutex);
	touchBuffer.push_back(e);
	pthread_mutex_unlock(&m_inputMutex);
}

AppWindow::AppWindow(AndroidNativeState* pState, PersistentAppState* pPersistentState)
: pState(pState)
, pPersistentState(pPersistentState)
, pAppOnMap(NULL)
, pInputProcessor(NULL)
, pWorld(NULL)
, pAndroidLocationService(NULL)
, appRunning(false)
, displayAvailable(false)
, worldInitialised(false)
, m_androidInputBoxFactory(pState)
, m_androidKeyboardInputFactory(pState, pInputHandler)
, m_androidAlertBoxFactory(pState)
, m_androidNativeUIFactories(m_androidAlertBoxFactory, m_androidInputBoxFactory, m_androidKeyboardInputFactory)
, m_terrainHeightRepository()
, m_terrainHeightProvider(&m_terrainHeightRepository)
{
	//Eegeo_TTY("CONSTRUCTING AppWindow");
    pthread_mutex_init(&m_mutex, 0);
    pthread_mutex_init(&m_inputMutex, 0);
}

AppWindow::~AppWindow()
{
	pthread_mutex_destroy(&m_mutex);
	pthread_mutex_destroy(&m_inputMutex);
}

void AppWindow::Pause(PersistentAppState* pPersistentState)
{
	pthread_mutex_lock(&m_mutex);
	appRunning = false;
	pthread_mutex_unlock(&m_mutex);

	if(pPersistentState != NULL)
	{
		Eegeo::Camera::GlobeCamera::GlobeCameraController& cameraController = GetAppOnMap().GetCameraController();
	    const Eegeo::Space::EcefTangentBasis& cameraInterest = cameraController.GetInterestBasis();

	    pPersistentState->lastGlobeCameraDistanceToInterest = cameraController.GetDistanceToInterest();
		float cameraHeadingRadians = Eegeo::Camera::CameraHelpers::GetAbsoluteBearingRadians(cameraInterest.GetPointEcef(), cameraInterest.GetForward());

		pPersistentState->lastGlobeCameraHeadingDegrees = Eegeo::Math::Rad2Deg(cameraHeadingRadians);
		pPersistentState->lastGlobeCameraLatLong = Eegeo::Space::LatLongAltitude::FromECEF(cameraInterest.GetPointEcef());
	}

    pthread_join(m_mainNativeThread, 0);
}

void AppWindow::Resume()
{
	pthread_mutex_lock(&m_mutex);
	appRunning = true;
	displayAvailable = false;
	worldInitialised = false;
	pthread_mutex_unlock(&m_mutex);

    pthread_create(&m_mainNativeThread, 0, Run, this);
}

void AppWindow::ActivateSurface()
{
	pthread_mutex_lock(&m_mutex);
	displayAvailable = true;
	pthread_mutex_unlock(&m_mutex);
}

void* AppWindow::Run(void* self)
{
	Eegeo::Helpers::ThreadHelpers::SetThisThreadAsMainThread();

	AppWindow* pSelf = (AppWindow*)self;
	Eegeo_TTY("STARTING RUN");

    while (1)
    {
    	pthread_mutex_lock(&pSelf->m_mutex);
    	bool running = pSelf->appRunning;
    	bool displayAvailable = pSelf->displayAvailable;
    	bool worldInitialised = pSelf->worldInitialised;
    	pthread_mutex_unlock(&pSelf->m_mutex);

        if(running)
        {
        	if(displayAvailable)
        	{
        		if(!worldInitialised)
        		{
        			pSelf->InitDisplay();
        		}

            	pSelf->UpdateWorld();
        	}
        }
        else
        {
        	break;
        }
    }

    pSelf->TerminateDisplay();

    pthread_exit(0);
    return NULL;
}

void AppWindow::UpdateWorld()
{
	if(pWorld!= NULL)
	{
		//get latest input buffer
		std::vector<TouchInputEvent> inputs;
		pthread_mutex_lock(&m_inputMutex);
		inputs = touchBuffer;
		touchBuffer.clear();
		pthread_mutex_unlock(&m_inputMutex);

		//process input events
		size_t events = inputs.size();
		for(size_t i = 0; i < events; ++ i)
		{
			this->pInputProcessor->HandleInput(inputs[i]);
		}

		//update and render world
		//Eegeo_TTY("UPDATING WORLD");
	    Eegeo_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

		float fps = 1.0f/30.0f;
		pAndroidWebRequestService->Update();
		pAppOnMap->Update(fps);
		pAppOnMap->Draw(fps);
		currentWeatherModel.SetWeatherType(pAppOnMap->World().GetWeatherController().GetWeatherType());

		Eegeo_GL(glFinish());
		Eegeo_GL(eglSwapBuffers(display, surface));
	}
}

void AppWindow::InitDisplay()
{
    // initialize OpenGL ES and EGL
    const EGLint attribs[] = {
    		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 16,
            EGL_STENCIL_SIZE, 8,
            EGL_NONE
    };

    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(pState->window, 0, 0, format);

    static const EGLint contextAttribs[] = {
              EGL_CONTEXT_CLIENT_VERSION,        2,
              EGL_NONE
           };

    surface = eglCreateWindowSurface(display, config, pState->window, NULL);
    context = eglCreateContext(display, config, NULL, contextAttribs);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        Eegeo_TTY("Unable to eglMakeCurrent");

        return;
    }

    //Eegeo_TTY("printing extensions\n");
    //char * extensionsString =  (char *) glGetString(GL_EXTENSIONS);
    //Eegeo_TTY("%s\n",extensionsString);

    Eegeo_GL(eglQuerySurface(display, surface, EGL_WIDTH, &w));
    Eegeo_GL(eglQuerySurface(display, surface, EGL_HEIGHT, &h));

    this->display = display;
    this->context = context;
    this->surface = surface;

#ifdef EEGEO_DROID_EMULATOR
    this->shareSurface = EGL_NO_SURFACE;
    this->resourceBuildShareContext = EGL_NO_CONTEXT;
#else
    resourceBuildShareContext = eglCreateContext(display, config, context, contextAttribs);

    const EGLint sharedSurfaceAttributes[] = {
      		  EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
              EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
              EGL_BLUE_SIZE, 8,
              EGL_GREEN_SIZE, 8,
              EGL_RED_SIZE, 8,
              EGL_ALPHA_SIZE, 8,
              EGL_DEPTH_SIZE, 16,
              EGL_STENCIL_SIZE, 8,
              EGL_NONE,

      };

    EGLint pbufferAttribs[] =
        {
            EGL_WIDTH, 1,
            EGL_HEIGHT, 1,
            EGL_NONE
        };
    EGLConfig sharedSurfaceConfig;
    eglChooseConfig(display, sharedSurfaceAttributes, &sharedSurfaceConfig, 1, &numConfigs);
    this->shareSurface = eglCreatePbufferSurface(display, sharedSurfaceConfig, pbufferAttribs);
#endif
    this->width = w;
    this->height = h;

    // Initialize GL state.
    Eegeo_GL(glClearDepthf(1.0f));
	Eegeo_GL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

	// Set up default Depth test.
	Eegeo_GL(glEnable(GL_DEPTH_TEST));
	Eegeo_GL(glDepthMask(GL_TRUE));
	Eegeo_GL(glDepthFunc(GL_LEQUAL))

	// Set up default culling.
	Eegeo_GL(glEnable(GL_CULL_FACE));
	Eegeo_GL(glFrontFace(GL_CW));
	Eegeo_GL(glCullFace(GL_BACK));

	// Turn off the stencil test.
	Eegeo_GL(glDisable(GL_STENCIL_TEST));
	Eegeo_GL(glStencilFunc(GL_NEVER, 0, 0xFFFFFFFF));
	Eegeo_GL(glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP));

	// Set the default blend mode and colour mask.
	Eegeo_GL(glDisable(GL_BLEND));
	Eegeo_GL(glColorMask(true, true, true, true));

	//when window and gl context are ready, init the world
	InitWorld();
}


void AppWindow::TerminateDisplay()
{
	delete pAppOnMap;
	pHttpCache->FlushInMemoryCacheRepresentation();

    delete pTaskQueue;

    delete m_pInterestPointProvider;

    delete pWorld;
    pWorld = NULL;

    delete pAndroidUrlEncoder;
    delete pAndroidLocationService;
    delete pRenderContext;
    delete pLighting;
    delete pFileIO;
    delete pHttpCache;
    delete pTextureLoader;
    Eegeo::EffectHandler::Reset();
    Eegeo::EffectHandler::Shutdown();
    pBlitter->Shutdown();
    delete pBlitter;
    delete pMaterialFactory;
    delete pAndroidWebRequestService;
    delete pAndroidWebLoadRequestFactory;
    delete pVehicleModelRepository;
    delete pVehicleModelLoader;

    if (this->display != EGL_NO_DISPLAY)
    {
    	Eegeo_GL(eglMakeCurrent(this->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT));

    	if (this->surface != EGL_NO_SURFACE)
		{
			Eegeo_GL(eglDestroySurface(this->display, this->surface));
		}

    	if (this->shareSurface != EGL_NO_SURFACE)
		{
    		Eegeo_GL(eglDestroySurface(this->display, this->shareSurface));
		}

    	if (this->context != EGL_NO_CONTEXT)
        {
        	Eegeo_GL(eglDestroyContext(this->display, this->context));
        }

        if(this->resourceBuildShareContext != EGL_NO_CONTEXT)
        {
        	Eegeo_GL(eglDestroyContext(this->display, this->resourceBuildShareContext));
        }

        Eegeo_GL(eglTerminate(this->display));
    }

    this->display = EGL_NO_DISPLAY;
    this->context = EGL_NO_CONTEXT;
    this->resourceBuildShareContext = EGL_NO_CONTEXT;
    this->surface = EGL_NO_SURFACE;
    this->shareSurface = EGL_NO_SURFACE;
}

void AppWindow::InitWorld()
{
	pAndroidUrlEncoder = new AndroidUrlEncoder(pState);
	pAndroidLocationService = new AndroidLocationService(pState);

	pRenderContext = new Eegeo::Rendering::RenderContext();
	pRenderContext->SetScreenDimensions(width, height, 1.0f);

	pLighting = new Eegeo::Lighting::GlobalLighting();
	pFogging = new Eegeo::Lighting::GlobalFogging();

	std::set<std::string> customApplicationAssetDirectories;
	customApplicationAssetDirectories.insert("MyAppDataDirectory");
	customApplicationAssetDirectories.insert("MyAppDataDirectory/MySubDirectory");
	pFileIO = new AndroidFileIO(pState, customApplicationAssetDirectories);

	pHttpCache = new AndroidHttpCache(pFileIO, "http://d2xvsc8j92rfya.cloudfront.net/");
	pTextureLoader = new AndroidTextureFileLoader(pFileIO, pRenderContext->GetGLState());

	Eegeo::EffectHandler::Initialise();
	pBlitter = new Eegeo::Blitter(1024 * 128, 1024 * 64, 1024 * 32, *pRenderContext);
	pBlitter->Initialise();

	pMaterialFactory = new Eegeo::Rendering::DefaultMaterialFactory;
	pMaterialFactory->Initialise(&currentWeatherModel, pRenderContext, pLighting, pFogging, pBlitter, pFileIO, pTextureLoader);

	pTaskQueue = new AndroidTaskQueue(10, resourceBuildShareContext, shareSurface, display);

	const int webLoadPoolSize = 10;
	const int cacheLoadPoolSize = 40;
	const int cacheStorePoolSize = 20;
	pAndroidWebRequestService = new AndroidWebRequestService(*pFileIO, pHttpCache, pTaskQueue, webLoadPoolSize, cacheLoadPoolSize, cacheStorePoolSize);

	pAndroidWebLoadRequestFactory = new AndroidWebLoadRequestFactory(pAndroidWebRequestService, pHttpCache);

	pVehicleModelRepository = new Eegeo::Traffic::VehicleModelRepository;
	pVehicleModelLoader = new Eegeo::Traffic::VehicleModelLoader(pRenderContext->GetGLState(),
																									 *pTextureLoader,
																									 *pFileIO);
	Eegeo::Traffic::VehicleModelLoaderHelper::LoadAllVehicleResourcesIntoRepository(*pVehicleModelLoader, *pVehicleModelRepository);

	m_pInterestPointProvider = new Eegeo::Camera::GlobeCamera::GlobeCameraInterestPointProvider();

	pWorld = new Eegeo::EegeoWorld(
		API_KEY,
		pHttpCache,
		pFileIO,
		pTextureLoader,
		pAndroidWebLoadRequestFactory,
		pTaskQueue,
		pVehicleModelRepository,
		*pRenderContext,
		pLighting,
		pFogging,
		pMaterialFactory,
		pAndroidLocationService,
		pBlitter,
		pAndroidUrlEncoder,
		*m_pInterestPointProvider,
		m_androidNativeUIFactories,
		&m_terrainHeightRepository,
		&m_terrainHeightProvider,
		new Eegeo::Search::Service::SearchServiceCredentials("", ""));


	pAppOnMap = new MyApp(&pInputHandler, *pState, *m_pInterestPointProvider);
	pInputProcessor = new Eegeo::Android::Input::AndroidInputProcessor(&pInputHandler, pRenderContext->GetScreenWidth(), pRenderContext->GetScreenHeight());

	pAppOnMap->Start(pWorld);

	if (pPersistentState != NULL)
    {
		pAppOnMap->JumpTo(
    			pPersistentState->lastGlobeCameraLatLong.GetLatitudeInDegrees(),
    			pPersistentState->lastGlobeCameraLatLong.GetLongitudeInDegrees(),
    			pPersistentState->lastGlobeCameraLatLong.GetAltitude(),
    			pPersistentState->lastGlobeCameraHeadingDegrees,
    			pPersistentState->lastGlobeCameraDistanceToInterest);
    }

    pthread_mutex_lock(&m_mutex);
    worldInitialised = true;
    pthread_mutex_unlock(&m_mutex);
}
