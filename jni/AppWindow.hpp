#include "Graphics.h"
#include "AndroidFileIO.h"
#include "AndroidHttpCache.h"
#include "AndroidTextureFileLoader.h"
#include "AndroidWebRequestService.hpp"
#include "AppOnMap.h"
#include "AndroidTaskQueue.h"
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
#include "TerrainHeightRepository.h"

namespace Eegeo
{
	class EegeoWorld;
}

struct PersistentAppState
{
	PersistentAppState():lastGlobeCameraLatLong(0,0,0){}

	Eegeo::Space::LatLongAltitude lastGlobeCameraLatLong;
	float lastGlobeCameraHeadingDegrees;
	float lastGlobeCameraDistanceToInterest;
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
    Eegeo::Camera::GlobeCamera::GlobeCameraInterestPointProvider* m_pInterestPointProvider;
	Eegeo::Android::AndroidTaskQueue* pTaskQueue;
	Eegeo::Rendering::RenderContext* pRenderContext;
	Eegeo::Lighting::GlobalLighting *pLighting;
	Eegeo::Lighting::GlobalFogging *pFogging;
	Eegeo::Blitter* pBlitter;
	Eegeo::Android::AndroidWebLoadRequestFactory* pAndroidWebLoadRequestFactory;
	Eegeo::Traffic::VehicleModelRepository* pVehicleModelRepository;
	Eegeo::Traffic::VehicleModelLoader* pVehicleModelLoader;
	Eegeo::Android::AndroidLocationService* pAndroidLocationService;
	Eegeo::Android::AndroidUrlEncoder* pAndroidUrlEncoder;
    Eegeo::Resources::Terrain::Heights::TerrainHeightRepository m_terrainHeightRepository;
    Eegeo::Resources::Terrain::Heights::TerrainHeightProvider m_terrainHeightProvider;
    Eegeo::Rendering::EnvironmentFlatteningService* m_pEnvironmentFlatteningService;

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

	void EnqueuePointerEvent(Eegeo::Android::Input::TouchInputEvent& e);

	Eegeo::EegeoWorld& GetWorld() { return *pWorld; }
	MyApp& GetAppOnMap() { return *pAppOnMap; }
};
