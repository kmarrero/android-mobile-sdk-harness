/*
 * RouteDrawingExample.h
 *
 *  Created on: Nov 14, 2013
 *      Author: kimbleoperations
 */

#ifndef ROUTEDRAWINGEXAMPLE_H_
#define ROUTEDRAWINGEXAMPLE_H_

#include <vector>
#include "IExample.h"
#include "RouteService.h"
#include "Route.h"
#include "EegeoWorld.h"

namespace Examples
{
    class RouteDrawingExample : public IExample
    {
    private:
        Eegeo::Routes::RouteService& m_routeService;
        Eegeo::EegeoWorld& m_world;

        bool m_createdRoutes;
        std::vector<Eegeo::Routes::Route*> m_routes;

    public:
        RouteDrawingExample(Eegeo::Routes::RouteService& routeService,
                            Eegeo::EegeoWorld& eegeoWorld);

        void Start() {}
        void Update(float dt);
        void Draw() {}
        void Suspend();
    };
}

#endif /* ROUTEDRAWINGEXAMPLE_H_ */
