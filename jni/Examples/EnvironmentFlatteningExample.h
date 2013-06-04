/*
 * EnvironmentFlatteningExample.h
 *
 *  Created on: Jun 4, 2013
 *      Author: kimbleoperations
 */

#ifndef ENVIRONMENTFLATTENINGEXAMPLE_H_
#define ENVIRONMENTFLATTENINGEXAMPLE_H_

#include "IExample.h"
#include "EnvironmentFlatteningService.h"

namespace Examples
{
    class EnvironmentFlatteningExample : public IExample
    {
    private:
        long long lastToggle;
        bool scaleUp;
        Eegeo::Rendering::EnvironmentFlatteningService& environmentFlatteningService;

    public:
        EnvironmentFlatteningExample(Eegeo::Rendering::EnvironmentFlatteningService& environmentFlatteningService);

        void Start() {}
        void Update();
        void Draw() {}
        void Suspend() {}
    };
}

#endif /* ENVIRONMENTFLATTENINGEXAMPLE_H_ */
