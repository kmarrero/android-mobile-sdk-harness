/*
 * ToggleTrafficExample.h
 *
 *  Created on: May 14, 2013
 *      Author: eegeo
 */

#ifndef TOGGLETRAFFICEXAMPLE_H_
#define TOGGLETRAFFICEXAMPLE_H_

#include "IExample.h"
#include "TrafficSimulation.h"

namespace Examples
{
    class ToggleTrafficExample : public IExample
    {
    private:
        long long lastToggle;
        Eegeo::Traffic::TrafficSimulation& trafficSimulation;

    public:
        ToggleTrafficExample(Eegeo::Traffic::TrafficSimulation& trafficSimulation);

        void Start() {}
        void Update();
        void Draw() {}
        void Suspend() {}
    };
}

#endif /* TOGGLETRAFFICEXAMPLE_H_ */
