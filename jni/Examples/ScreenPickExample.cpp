//
//  ScreenPickExample.cpp
//  ExampleApp
//

#include "ScreenPickExample.h"
#include "CameraHelpers.h"
#include "ICameraProvider.h"
#include "EarthConstants.h"
#include "SphereMesh.h"
#include "TerrainHeightProvider.h"
#include "RenderCamera.h"
#include "TerrainRayPicker.h"

namespace Examples
{
    ScreenPickExample::ScreenPickExample(Eegeo::Rendering::RenderContext& renderContext,
                                         Eegeo::Camera::ICameraProvider& cameraProvider,
                                         Eegeo::Resources::Terrain::Heights::TerrainHeightProvider& terrainHeightProvider,
                                         const Eegeo::Resources::Terrain::Collision::ICollisionMeshResourceProvider& collisionMeshResourceProvider)
    : m_renderContext(renderContext)
    , m_cameraProvider(cameraProvider)
    {
        const float radius = 20.f;
        const int numSegments = 16;
        const Eegeo::v3 red(1.f, 0.f, 0.f);
        
        m_sphere = new Eegeo::DebugRendering::SphereMesh(
                                                       renderContext,
                                                       radius,
                                                       numSegments, numSegments,
                                                       Eegeo::dv3(),
                                                       NULL,
                                                       red
                                                       );
        m_sphere->Tesselate();

        m_rayPicker = new Eegeo::Resources::Terrain::Collision::TerrainRayPicker(terrainHeightProvider, collisionMeshResourceProvider);
    }
    
    ScreenPickExample::~ScreenPickExample()
    {
        delete m_rayPicker;
        delete m_sphere;
    }
    
    void ScreenPickExample::Start()
    {

    }
    
    void ScreenPickExample::Suspend()
    {
        
    }
    
    void ScreenPickExample::Update(float dt)
    {
        
    }
    
    void ScreenPickExample::Draw()
    {
        m_sphere->Draw(m_renderContext);
    }
    

    
    bool ScreenPickExample::Event_TouchDown(const AppInterface::TouchData& data)
    {
        const Eegeo::Camera::RenderCamera& renderCamera = m_cameraProvider.GetRenderCamera();

        float screenPixelX = data.point.GetX();
        float screenPixelY = data.point.GetY();
        
        Eegeo::dv3 rayOrigin = renderCamera.GetEcefLocation();
        Eegeo::dv3 rayDirection;
        Eegeo::Camera::CameraHelpers::GetScreenPickRay(renderCamera, screenPixelX, screenPixelY, rayDirection);
        
        Eegeo::dv3 rayIntersectionPoint;
        double intersectionParam;
        bool rayPick = m_rayPicker->TryGetRayIntersection(rayOrigin, rayDirection, rayIntersectionPoint, intersectionParam);
        if (rayPick)
        {
            m_sphere->SetPositionECEF(rayIntersectionPoint);
        }
        
        return true;
    }
}
