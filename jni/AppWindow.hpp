#include <android_native_app_glue.h>
#include "Graphics.h"
#include "AndroidFileIO.h"
#include "AndroidHttpCache.h"
#include "AndroidTextureFileLoader.h"
#include "AndroidWebRequestService.hpp"
#include "NewGlobeCamera.h"
#include "AppOnMap.h"
#include "AndroidDebugStats.h"
#include "AndroidTaskQueue.h"
#include "DefaultMaterialFactory.h"
#include "AndroidWebLoadRequestFactory.h"
#include "AndroidInputProcessor.h"
#include "AndroidLocationService.h"

namespace Eegeo
{
	class EegeoWorld;
}

class AppWindow 
{
private:
	MyApp* pAppOnMap;
	Eegeo::Android::Input::AndroidInputProcessor* pInputProcessor;
	Eegeo::EegeoWorld* pWorld;
	struct android_app* pState;
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
	Eegeo::Android::AndroidDebugStats* pAndroidDebugStats;
	Eegeo::Camera::CameraModel* pCameraModel;
	Eegeo::Camera::NewGlobeCamera* pGlobeCamera;
	Eegeo::Android::AndroidTaskQueue* pTaskQueue;
	Eegeo::Rendering::RenderContext* pRenderContext;
	Eegeo::Lighting::GlobalLighting *pLighting;
	Eegeo::Blitter* pBlitter;
	Eegeo::Rendering::DefaultMaterialFactory* pMaterialFactory ;
	Eegeo::Android::AndroidWebLoadRequestFactory* pAndroidWebLoadRequestFactory;
	Eegeo::Traffic::VehicleModelRepository* pVehicleModelRepository;
	Eegeo::Traffic::VehicleModelLoader* pVehicleModelLoader;
	Eegeo::Android::AndroidTextRenderer * pTextRenderer;
	Eegeo::RenderCamera* pCamera;
	Eegeo::Space::LatLongAltitude lastGlobeCameraLatLong;
	Eegeo::v3 lastGlobeCameraHeading;
	float lastGlobeCameraDistanceToInterest;
	Eegeo::Android::AndroidLocationService* pAndroidLocationService;

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
