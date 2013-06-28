#ifndef __ExampleApp__AppOnMap__
#define __ExampleApp__AppOnMap__

#include "IAppOnMap.h"
#include "EegeoWorld.h"
#include "RenderContext.h"
#include "NewGlobeCamera.h"
#include "AndroidInputHandler.h"
#include "TerrainHeightProvider.h"
#include "NavigationGraphRepository.h"
#include "IStreamingVolume.h"
#include "GlobalLighting.h"
#include "WeatherController.h"

#include "DebugSphereExample.h"
#include "ScreenUnprojectExample.h"
#include "LoadModelExample.h"
#include "EnvironmentNotifierExample.h"
#include "FileIOExample.h"
#include "WebRequestExample.h"
#include "NavigationGraphExample.h"
#include "ModifiedRenderingExample.h"
#include "ToggleTrafficExample.h"
#include "ResourceSpatialQueryExample.h"
#include "EnvironmentFlatteningExample.h"
#include "SearchExample.h"

namespace ExampleTypes
{
	enum Examples
	{
		DebugSphere=0,
		ScreenUnproject,
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
        Search
	};
}

class MyApp : public Eegeo::IAppOnMap, public Eegeo::Android::Input::AndroidInputHandler
{
private:
	Examples::IExample *pExample;
	Eegeo::Camera::NewGlobeCamera* globeCamera;

public:
	~MyApp()
	{
		pExample->Suspend();
		delete pExample;
	}

	void OnStart ()
	{
		ExampleTypes::Examples selectedExample = ExampleTypes::DebugSphere;

		float interestPointLatitudeDegrees = 37.7858f;
		float interestPointLongitudeDegrees = -122.401f;
		float interestPointAltitudeMeters = 2.7;

		Eegeo::Space::LatLongAltitude location = Eegeo::Space::LatLongAltitude(interestPointLatitudeDegrees,
				interestPointLongitudeDegrees,
				interestPointAltitudeMeters,
				Eegeo::Space::LatLongUnits::Degrees);

		World().GetCameraModel().SetWorldPosition(location.ToECEF());
		World().GetWeatherController().SetWeather(Eegeo::Weather::Sunny, 1.0f);

		float cameraControllerOrientationDegrees = 0.0f;
		float cameraControllerDistanceFromInterestPointMeters = 1781.0f;

		globeCamera = &((Eegeo::Camera::NewGlobeCamera&)World().GetCameraController());

		globeCamera->SetInterestHeadingDistance(location,
				cameraControllerOrientationDegrees,
				cameraControllerDistanceFromInterestPointMeters);


		Eegeo::Search::Service::SearchService* searchService = NULL;
		if (World().IsSearchServiceAvailable())
		{
		    searchService = &World().GetSearchService();
		}
        pExample = CreateExample(selectedExample,
                                 World().GetRenderContext(),
                                 location,
                                 World().GetCameraModel(),
                                 *globeCamera,
                                 *globeCamera->GetCamera(),
                                 World().GetTerrainHeightProvider(),
                                 World().GetTextureLoader(),
                                 World().GetFileIO(),
                                 World().GetTerrainStreaming(),
                                 World().GetWebRequestFactory(),
                                 World().GetNavigationGraphRepository(),
                                 World().GetBuildingMeshPool(),
                                 World().GetShadowMeshPool(),
                                 World().GetStreamingVolume(),
                                 World().GetGlobalLighting(),
                                 World().GetGlobalFogging(),
                                 World().GetTrafficSimulation(),
                                 World().GetResourceSpatialQueryService(),
                                 World().GetEnvironmentFlatteningService(),
                                 searchService);

		pExample->Start();
	}

	void Update (float dt)
	{
		World().Update(dt);
		pExample->Update();
	}

	void Draw (float dt)
	{
		Eegeo::Rendering::GLState& glState = World().GetRenderContext().GetGLState();
		glState.ClearColor(0.8f, 0.8f, 0.8f, 1.f);
		World().Draw(dt);
		pExample->Draw();
	}

    Examples::IExample* CreateExample(ExampleTypes::Examples example,
                                      Eegeo::Rendering::RenderContext& renderContext,
                                      Eegeo::Space::LatLongAltitude interestLocation,
                                      Eegeo::Camera::CameraModel& cameraModel,
                                      Eegeo::Camera::NewGlobeCamera& globeCamera,
                                      Eegeo::RenderCamera& renderCamera,
                                      Eegeo::Resources::Terrain::Heights::TerrainHeightProvider& terrainHeightProvider,
                                      Eegeo::Helpers::ITextureFileLoader& textureLoader,
                                      Eegeo::Helpers::IFileIO& fileIO,
                                      Eegeo::Resources::Terrain::TerrainStreaming& terrainStreaming,
                                      Eegeo::Web::IWebLoadRequestFactory& webRequestFactory,
                                      Eegeo::Resources::Roads::Navigation::NavigationGraphRepository& navigationGraphs,
                                      Eegeo::Resources::MeshPool<Eegeo::Rendering::RenderableItem*>& buildingPool,
                                      Eegeo::Resources::MeshPool<Eegeo::Rendering::RenderableItem*>& shadowPool,
                                      Eegeo::Streaming::IStreamingVolume& visibleVolume,
                                      Eegeo::Lighting::GlobalLighting& lighting,
                                      Eegeo::Lighting::GlobalFogging& fogging,
                                      Eegeo::Traffic::TrafficSimulation& trafficSimulation,
                                      Eegeo::Resources::ResourceSpatialQueryService& resourceSpatialQueryService,
                                      Eegeo::Rendering::EnvironmentFlatteningService& environmentFlatteningService,
                                      Eegeo::Search::Service::SearchService* searchService)
	{
		switch(example)
		{

		case ExampleTypes::LoadModel:
			return new Examples::LoadModelExample(renderContext,
					interestLocation,
					cameraModel,
					renderCamera,
					fileIO,
					textureLoader,
					fogging);

		case ExampleTypes::ScreenUnproject:
		case ExampleTypes::TerrainHeightQuery:
			return new Examples::ScreenUnprojectExample(renderContext,
					cameraModel,
					renderCamera,
					terrainHeightProvider);

		case ExampleTypes::DebugSphere:
			return new Examples::DebugSphereExample(renderContext,
					interestLocation,
					cameraModel,
					renderCamera);

		case ExampleTypes::EnvironmentNotifier:
			return new Examples::EnvironmentNotifierExample(renderContext,
					cameraModel,
					renderCamera,
					terrainStreaming);

		case ExampleTypes::FileIO:
			return new Examples::FileIOExample(fileIO);

		case ExampleTypes::WebRequest:
			return new Examples::WebRequestExample(webRequestFactory);

		case ExampleTypes::NavigationGraph:
			return new Examples::NavigationGraphExample(renderContext,
					renderCamera,
					cameraModel,
					navigationGraphs);

        case ExampleTypes::ModifiedRendering:
            return new Examples::ModifiedRenderingExample(renderContext,
                                                          renderCamera,
                                                          cameraModel,
                                                          globeCamera,
                                                          visibleVolume,
                                                          lighting,
                                                          buildingPool,
                                                          shadowPool);

        case ExampleTypes::ToggleTraffic:
            return new Examples::ToggleTrafficExample(trafficSimulation);

        case ExampleTypes::ResourceSpatialQuery:
            return new Examples::ResourceSpatialQueryExample(resourceSpatialQueryService,
                                                             globeCamera);

        case ExampleTypes::EnvironmentFlattening:
            return new Examples::EnvironmentFlatteningExample(environmentFlatteningService);

        case ExampleTypes::Search:
        	Eegeo_ASSERT(searchService != NULL, "Cannot run Search example, you must set up here.com Credentials in ViewController.mm");
        	return new Examples::SearchExample(*searchService, globeCamera);
		}
	}

	void Event_TouchRotate      (const AppInterface::RotateData& data) { globeCamera->Event_TouchRotate(data); }
	void Event_TouchRotate_Start  (const AppInterface::RotateData& data) { globeCamera->Event_TouchRotate_Start(data); }
	void Event_TouchRotate_End    (const AppInterface::RotateData& data) { globeCamera->Event_TouchRotate_End(data); }

	void Event_TouchPinch       (const AppInterface::PinchData& data) { globeCamera->Event_TouchPinch(data); }
	void Event_TouchPinch_Start   (const AppInterface::PinchData& data) { globeCamera->Event_TouchPinch_Start(data); }
	void Event_TouchPinch_End     (const AppInterface::PinchData& data) { globeCamera->Event_TouchPinch_End(data); }

	void Event_TouchPan       (const AppInterface::PanData& data) { globeCamera->Event_TouchPan(data); }
	void Event_TouchPan_Start   (const AppInterface::PanData& data) { globeCamera->Event_TouchPan_Start(data); }
	void Event_TouchPan_End     (const AppInterface::PanData& data) { globeCamera->Event_TouchPan_End(data); }

	void Event_TouchTap       (const AppInterface::TapData& data) {globeCamera->Event_TouchTap(data); }
	void Event_TouchDoubleTap   (const AppInterface::TapData& data) { globeCamera->Event_TouchDoubleTap(data); }

	void Event_TouchDown      (const AppInterface::TouchData& data) { globeCamera->Event_TouchDown(data); }
	void Event_TouchMove      (const AppInterface::TouchData& data) { globeCamera->Event_TouchMove(data); }
	void Event_TouchUp        (const AppInterface::TouchData& data) { globeCamera->Event_TouchUp(data); }
};

#endif /* defined(__ExampleApp__AppOnMap__) */
