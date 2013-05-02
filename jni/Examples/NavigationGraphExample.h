/*
 * NavigationGraphExample.h
 *
 *  Created on: May 2, 2013
 *      Author: eegeo
 */

#ifndef NAVIGATIONGRAPHEXAMPLE_H_
#define NAVIGATIONGRAPHEXAMPLE_H_

#include "IExample.h"

#include "NavigationGraphRepository.h"
#include "NavigationGraphAddedCallback.h"
#include "NavigationGraphRemovalCallback.h"
#include "NavigationGraph.h"
#include "DebugRendering.h"
#include "RenderContext.h"
#include "CameraModel.h"

namespace Examples
{
    class NavigationGraphExample : public IExample
    {
    private:
        typedef std::map<const Eegeo::Resources::Roads::Navigation::NavigationGraph*, Eegeo::DebugRendering::DebugRenderable*> MapType;

        struct Added : public Eegeo::Resources::Roads::Navigation::INavigationGraphAddedCallback {
            NavigationGraphExample& example;
            Added(NavigationGraphExample& example):example(example){}
            void operator()(const Eegeo::Resources::Roads::Navigation::NavigationGraph& navGraph) { example.HandleAddedGraph(navGraph); }
        };

        struct Removed : public Eegeo::Resources::Roads::Navigation::INavigationGraphRemovalCallback {
            NavigationGraphExample& example;
            Removed(NavigationGraphExample& example):example(example){}
            void operator()(const Eegeo::Resources::Roads::Navigation::NavigationGraph& navGraph) { example.HandleRemovedGraph(navGraph); }
        };


        void HandleAddedGraph(const Eegeo::Resources::Roads::Navigation::NavigationGraph& navGraph);
        void HandleRemovedGraph(const Eegeo::Resources::Roads::Navigation::NavigationGraph& navGraph);

        Eegeo::Rendering::RenderContext& renderContext;
        Eegeo::RenderCamera& renderCamera;
        Eegeo::Camera::CameraModel& cameraModel;
        Eegeo::Resources::Roads::Navigation::NavigationGraphRepository& navigationGraphRepository;

        Added addedHandler;
        Removed removedHandler;
        MapType navGraphsToVisualisers;

    public:
        NavigationGraphExample(Eegeo::Rendering::RenderContext& renderContext,
                               Eegeo::RenderCamera& renderCamera,
                               Eegeo::Camera::CameraModel& cameraModel,
                               Eegeo::Resources::Roads::Navigation::NavigationGraphRepository& navigationGraphRepository);

        void Start();
        void Update() {}
        void Draw();
        void Suspend();
    };
}

#endif /* NAVIGATIONGRAPHEXAMPLE_H_ */
