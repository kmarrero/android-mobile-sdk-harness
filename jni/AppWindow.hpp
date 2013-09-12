#include <android_native_app_glue.h>
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
#include "MyAndroidInputHandler.h"

namespace Eegeo
{
	class EegeoWorld;
}

class AppWindow 
{
private:
	AppOnMap* pAppOnMap;
	Eegeo::Android::Input::AndroidInputProcessor* pInputProcessor;
	Eegeo::Android::Input::AndroidInputHandler pInputHandler;
	MyAndroidInputHandler* pAppInputHandler;
	struct android_app* pState;
	int width;
	int height;
    EGLDisplay display;
    EGLSurface surface;
    EGLSurface shareSurface;
    EGLContext context;
    EGLContext resourceBuildShareContext;
    Eegeo::Android::AndroidWebRequestService* pAndroidWebRequestService;
	Eegeo::Space::LatLongAltitude lastGlobeCameraLatLong;
	Eegeo::v3 lastGlobeCameraHeading;
	float lastGlobeCameraDistanceToInterest;
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

	bool active;
	bool firstTime;

	void InitDisplay();
	void TerminateDisplay();
	void InitWorld();
	void UpdateWorld();

public:

	AppWindow(struct android_app* pState);
	void Run();
	int32_t HandleInput(AInputEvent* event);
	void HandleCommand(int32_t cmd);
};
