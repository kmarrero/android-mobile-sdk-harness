/*
 * SingleCityExample.h
 *
 *  Created on: Mar 6, 2014
 *      Author: kimbleoperations
 */

#ifndef SINGLECITYEXAMPLE_H_
#define SINGLECITYEXAMPLE_H_

#include "IExample.h"
#include "EegeoWorld.h"
#include "PrecacheService.h"
#include "GlobeCameraController.h"
#include "StreamingVolumeController.h"

namespace Examples
{
    class SingleCityExample : public IExample
    {
    private:
        Eegeo::Camera::GlobeCamera::GlobeCameraController& m_globeCamera;
        Eegeo::Web::PrecacheService& m_precacheService;
        Eegeo::Streaming::StreamingVolumeController& m_streamingVolumeController;
        Eegeo::EegeoWorld& m_world;

        bool m_startedPrecaching;
        bool m_precacheComplete;

        void ConstrainCamera();

    public:
        SingleCityExample(Eegeo::Camera::GlobeCamera::GlobeCameraController& globeCamera,
                          Eegeo::Web::PrecacheService& precacheService,
                          Eegeo::Streaming::StreamingVolumeController& streamingVolumeController,
                          Eegeo::EegeoWorld& world);

        void Start() {}
        void Update(float dt);
        void AfterCameraUpdate();
        void Draw() {}
        void Suspend() {}
    };
}

#endif /* SINGLECITYEXAMPLE_H_ */
