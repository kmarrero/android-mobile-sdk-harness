/*
 * ResourceSpatialQueryExample.h
 *
 *  Created on: May 15, 2013
 *      Author: eegeo
 */

#ifndef RESOURCESPATIALQUERYEXAMPLE_H_
#define RESOURCESPATIALQUERYEXAMPLE_H_

#include "IExample.h"
#include "ResourceSpatialQueryService.h"
#include "NewGlobeCamera.h"

namespace Examples
{
    class ResourceSpatialQueryExample : public IExample
    {
    private:
        Eegeo::Resources::ResourceSpatialQueryService& resourceSpatialQueryService;
        Eegeo::Camera::NewGlobeCamera& globeCamera;

        Eegeo::Streaming::MortonKeyLong key;
        int numBuildings;

    public:
        ResourceSpatialQueryExample(Eegeo::Resources::ResourceSpatialQueryService& resourceSpatialQueryService,
                                    Eegeo::Camera::NewGlobeCamera& globeCamera);

        void Start() {}
        void Update();
        void Draw() {}
        void Suspend() {}
    };
}

#endif /* RESOURCESPATIALQUERYEXAMPLE_H_ */
