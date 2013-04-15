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
#include "AndroidPayloadLoadRequestItemFactory.hpp"
#include "AndroidInputProcessor.h"

namespace Eegeo
{
	class EegeoWorld;
}

class AppWindow 
{
private:
	MyApp* pAppOnMap;
	AndroidInputProcessor* pInputProcessor;
	Eegeo::EegeoWorld* pWorld;
	struct android_app* pState;
	int width;
	int height;
    EGLDisplay display;
    EGLSurface surface;
    EGLSurface shareSurface;
    EGLContext context;
    EGLContext resourceBuildShareContext;
    AndroidWebRequestService* pAndroidWebRequestService;
    AndroidFileIO* pFileIO;
	AndroidHttpCache* pHttpCache;
	AndroidTextureFileLoader* pTextureLoader;
	AndroidDebugStats* pAndroidDebugStats;
	Eegeo::Camera::CameraModel* pCameraModel;
	Eegeo::Camera::NewGlobeCamera* pGlobeCamera;
	AndroidTaskQueue* pTaskQueue;
	Eegeo::Rendering::RenderContext* pRenderContext;
	Eegeo::Lighting::GlobalLighting *pLighting;
	Eegeo::Blitter* pBlitter;
	Eegeo::Rendering::DefaultMaterialFactory* pMaterialFactory ;
	AndroidPayloadLoadRequestItemFactory* pPayloadLoadRequestItemFactory;
	Eegeo::Traffic::VehicleModelRepository* pVehicleModelRepository;
	Eegeo::Traffic::VehicleModelLoader* pVehicleModelLoader;
	AndroidTextRenderer * pTextRenderer;
	Eegeo::RenderCamera* pCamera;
	Eegeo::Space::LatLongAltitude lastGlobeCameraLatLong;
	Eegeo::v3 lastGlobeCameraHeading;
	float lastGlobeCameraDistanceToInterest;

	bool active;
	bool firstTime;

	void InitDisplay();
	void TerminateDisplay();
	void InitWorld();
	void UpdateWorld();

public:

	AppWindow(struct android_app* pState, MyApp* pAppOnMap, AndroidInputProcessor* pInputProcessor);
	void Run();
	int32_t HandleInput(AInputEvent* event);
	void HandleCommand(int32_t cmd);
};
