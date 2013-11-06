/*
 * ToggleTrafficExample.cpp
 *
 *  Created on: May 14, 2013
 *      Author: eegeo
 */

#include "ToggleTrafficExample.h"
#include "TimeHelpers.h"
#include "Logger.h"

using namespace Examples;
using namespace Eegeo::Traffic;
using namespace Eegeo::Helpers::Time;

#define TRAFFIC_ENABLED_SWITCH_DELAY_MILLISECONDS 5000

ToggleTrafficExample::ToggleTrafficExample(Eegeo::Traffic::TrafficSimulation& trafficSimulation)
:trafficSimulation(trafficSimulation)
,lastToggle(MillisecondsSinceEpoch())
{
}

void ToggleTrafficExample::Update()
{
    long long ms = MillisecondsSinceEpoch();

    if(ms - lastToggle > TRAFFIC_ENABLED_SWITCH_DELAY_MILLISECONDS)
    {
        lastToggle = ms;

        EXAMPLE_LOG("Toggling Traffic - Setting enabled = %s\n", trafficSimulation.Enabled() ? "false!" : "true!");
        trafficSimulation.SetEnabled(!trafficSimulation.Enabled());
    }
}

