/*
 * RouteThicknessPolicyExample.h
 *
 *  Created on: Nov 28, 2013
 *      Author: kimbleoperations
 */

#ifndef ROUTETHICKNESSPOLICYEXAMPLE_H_
#define ROUTETHICKNESSPOLICYEXAMPLE_H_

#include <vector>
#include "IExample.h"
#include "RouteService.h"
#include "Route.h"
#include "EegeoWorld.h"
#include "IdentityRouteThicknessPolicy.h"
#include "LinearAltitudeBasedRouteThicknessPolicy.h"
#include "FittedRoute.h"

namespace Examples
{
    class RouteThicknessPolicyExample : public IExample
    {
    private:
        class MyScalingRouteThicknessPolicy : public Eegeo::Routes::Style::Thickness::IRouteThicknessPolicy
        {
            float m_scale;
            bool m_scaledown;
            float GetThicknessScale(const Eegeo::Routes::Fitting::FittedRoute& fittedRoute) const;
        public:
            MyScalingRouteThicknessPolicy():m_scale(1.f), m_scaledown(false) {}
            void UpdateScale();
        };

        Eegeo::Routes::RouteService& m_routeService;
        Eegeo::EegeoWorld& m_world;

        bool m_createdRoutes;
        std::vector<Eegeo::Routes::Route*> m_routes;
        Eegeo::Routes::Style::Thickness::IdentityRouteThicknessPolicy m_identityRouteThicknessPolicy;
        Eegeo::Routes::Style::Thickness::LinearAltitudeBasedRouteThicknessPolicy m_linearAltitudeBasedRouteThicknessPolicy;
        MyScalingRouteThicknessPolicy m_myScalingRouteThicknessPolicy;

    public:
        RouteThicknessPolicyExample(Eegeo::Routes::RouteService& routeService,
                                    Eegeo::Rendering::RenderContext& renderContext,
                                    Eegeo::EegeoWorld& eegeoWorld);

        void Start() {}
        void Update(float dt);
        void Draw() {}
        void Suspend();
    };
}

#endif /* ROUTETHICKNESSPOLICYEXAMPLE_H_ */
