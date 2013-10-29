#include "Graphics.h"
#include "AndroidFileIO.h"
#include "AndroidHttpCache.h"
#include "AndroidTextureFileLoader.h"
#include "AndroidWebRequestService.hpp"
#include "NewGlobeCamera.h"
#include "AppOnMap.h"
#include "AndroidTaskQueue.h"
#include "DefaultMaterialFactory.h"
#include "AndroidWebLoadRequestFactory.h"
#include "AndroidInputProcessor.h"
#include "AndroidLocationService.h"
#include "AndroidUrlEncoder.h"
#include "GlobeCameraInterestPointProvider.h"
#include "TerrainHeightProvider.h"
#include "AndroidInputHandler.h"
#include "AndroidInputBoxFactory.h"
#include "AndroidKeyboardInputFactory.h"
#include "AndroidAlertBoxFactory.h"
#include "NativeUIFactories.h"
#include "WeatherUpdateModel.h"
#include "AndroidNativeState.h"
#include "TouchEventWrapper.h"

namespace Eegeo
{
	class EegeoWorld;
}

struct PersistentAppState
{
	PersistentAppState():lastGlobeCameraLatLong(0,0,0){}

	Eegeo::Space::LatLongAltitude lastGlobeCameraLatLong;
	Eegeo::v3 lastGlobeCameraHeading;
	float lastGlobeCameraDistanceToInterest;
	bool gpsActive;
};

class AppWindow 
{
private:
	MyApp* pAppOnMap;
	PersistentAppState* pPersistentState;
	Eegeo::Android::Input::AndroidInputProcessor* pInputProcessor;
	Eegeo::Android::Input::AndroidInputHandler pInputHandler;
	Eegeo::EegeoWorld* pWorld;
	AndroidNativeState* pState;
	int width;
	int height;
    EGLDisplay display;
    EGLSurface surface;
    EGLSurface shareSurface;
    EGLContext context;
    EGLContext resourceBuildShareContext;
    Eegeo::Android::AndroidWebRequestService* pAndroidWebRequestService;
    Eegeo::Android::AndroidFileIO* pFileIO;
    Eegeo::Android::AndroidHttpCache* pHttpCache;
    Eegeo::Android::AndroidTextureFileLoader* pTextureLoader;
	Eegeo::Camera::CameraModel* pCameraModel;
	Eegeo::Camera::NewGlobeCamera* pGlobeCamera;
	Eegeo::Android::AndroidTaskQueue* pTaskQueue;
	Eegeo::Rendering::RenderContext* pRenderContext;
	Eegeo::Lighting::GlobalLighting *pLighting;
	Eegeo::Lighting::GlobalFogging *pFogging;
	Eegeo::Blitter* pBlitter;
	Eegeo::Rendering::DefaultMaterialFactory* pMaterialFactory ;
	Eegeo::Android::AndroidWebLoadRequestFactory* pAndroidWebLoadRequestFactory;
	Eegeo::Traffic::VehicleModelRepository* pVehicleModelRepository;
	Eegeo::Traffic::VehicleModelLoader* pVehicleModelLoader;
	Eegeo::RenderCamera* pCamera;
	Eegeo::Android::AndroidLocationService* pAndroidLocationService;
	Eegeo::Android::AndroidUrlEncoder* pAndroidUrlEncoder;
	Eegeo::Location::GlobeCameraInterestPointProvider* m_pGlobeCameraInterestPointProvider;
	Eegeo::Weather::CurrentWeatherModel currentWeatherModel;
    Eegeo::Resources::Terrain::Heights::TerrainHeightRepository m_terrainHeightRepository;
    Eegeo::Resources::Terrain::Heights::TerrainHeightProvider m_terrainHeightProvider;

	Eegeo::UI::NativeInput::Android::AndroidInputBoxFactory m_androidInputBoxFactory;
	Eegeo::UI::NativeInput::Android::AndroidKeyboardInputFactory m_androidKeyboardInputFactory;
	Eegeo::UI::NativeAlerts::Android::AndroidAlertBoxFactory m_androidAlertBoxFactory;
	Eegeo::UI::NativeUIFactories m_androidNativeUIFactories;

	bool appRunning;
	bool displayAvailable;
	bool worldInitialised;

	pthread_t m_mainNativeThread;
	pthread_mutex_t m_mutex;

	double m_timeOfLastUpdateCall;

	std::deque<double> m_frameDeltas;

	double CalcSmoothedDelta() const;

	void InitDisplay();
	void TerminateDisplay();
	void InitWorld();
	void UpdateWorld();
	double GetTimeNowSeconds();

	pthread_mutex_t m_inputMutex;
	std::vector<Eegeo::Android::Input::TouchInputEvent> touchBuffer;

	static void* Run(void* self);

public:

	AppWindow(AndroidNativeState* pState, PersistentAppState* pPersistentState);
	~AppWindow();

	void Pause(PersistentAppState* pPersistentState);
	void Resume();
	void ActivateSurface();

	void EnqueuePointerDown(Eegeo::Android::Input::TouchInputEvent& e);
	void EnqueuePointerUp(Eegeo::Android::Input::TouchInputEvent& e);
	void EnqueuePointerMove(Eegeo::Android::Input::TouchInputEvent& e);

	Eegeo::EegeoWorld& GetWorld() { return *pWorld; }
};
