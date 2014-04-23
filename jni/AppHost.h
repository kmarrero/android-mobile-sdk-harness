/*
 * AppHost.h
 *
 *  Created on: Apr 21, 2014
 */

#ifndef APPHOST_H_
#define APPHOST_H_

#include "Types.h"
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
#include "GlobalShadowing.h"
#include "PersistentAppState.h"
#include "AppOnMap.h"
#include "TouchEventWrapper.h"
#include <vector>

class AppHost : protected Eegeo::NonCopyable
{
public:
	AppHost(
			const std::string& apiKey,
			AndroidNativeState& nativeState,
			const PersistentAppState& persistentAppState,
			float displayWidth,
			float displayHeight,
		    EGLDisplay display,
		    EGLSurface shareSurface,
		    EGLContext resourceBuildShareContext
			);
	~AppHost();

	void ProcessTouchInputEvent(const Eegeo::Android::Input::TouchInputEvent& event);

	void Update(float dt);

	void OnStarted();
	void OnStopped();

	// Suspect.
	void SaveAppState(PersistentAppState& persistentAppState) const;
	void SetSharedSurface(EGLSurface sharedSurface);
	void SetViewportOffset(float x, float y);

private:
	Eegeo::Android::AndroidTaskQueue* m_pTaskQueue;
	Eegeo::Rendering::EnvironmentFlatteningService* m_pEnvironmentFlatteningService;
	Eegeo::Android::AndroidWebLoadRequestFactory* m_pAndroidWebLoadRequestFactory;
	Eegeo::Android::AndroidWebRequestService* m_pAndroidWebRequestService;
	Eegeo::Blitter* m_pBlitter;
	Eegeo::Android::AndroidTextureFileLoader* m_pTextureLoader;
	Eegeo::Android::AndroidHttpCache* m_pHttpCache;
	Eegeo::Android::AndroidFileIO* m_pFileIO;
	Eegeo::Lighting::GlobalLighting* m_pLighting;
	Eegeo::Lighting::GlobalFogging* m_pFogging;
	Eegeo::Lighting::GlobalShadowing* m_pShadowing;
	Eegeo::Rendering::RenderContext* m_pRenderContext;
	Eegeo::Android::AndroidLocationService* m_pAndroidLocationService;
	Eegeo::Android::AndroidUrlEncoder* m_pAndroidUrlEncoder;
	Eegeo::EegeoWorld* m_pWorld;
	Eegeo::Camera::GlobeCamera::GlobeCameraInterestPointProvider* m_pInterestPointProvider;

    Eegeo::Resources::Terrain::Heights::TerrainHeightRepository m_terrainHeightRepository;
    Eegeo::Resources::Terrain::Heights::TerrainHeightProvider m_terrainHeightProvider;

	Eegeo::Android::Input::AndroidInputHandler m_inputHandler;
	Eegeo::UI::NativeInput::Android::AndroidInputBoxFactory m_androidInputBoxFactory;
	Eegeo::UI::NativeInput::Android::AndroidKeyboardInputFactory m_androidKeyboardInputFactory;
	Eegeo::UI::NativeAlerts::Android::AndroidAlertBoxFactory m_androidAlertBoxFactory;
	Eegeo::UI::NativeUIFactories m_androidNativeUIFactories;

	MyApp* m_pAppOnMap;

	Eegeo::Android::Input::AndroidInputProcessor* m_pInputProcessor;
};

#endif /* APPHOST_H_ */
