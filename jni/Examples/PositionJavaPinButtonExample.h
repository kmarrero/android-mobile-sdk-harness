/*
 * PositionJavaPinButton.h
 *
 *  Created on: Oct 31, 2013
 *      Author: kimbleoperations
 */

#ifndef POSITIONJAVAPINBUTTON_H_
#define POSITIONJAVAPINBUTTON_H_

#include <jni.h>
#include "IExample.h"
#include "AndroidNativeState.h"
#include "RenderContext.h"


namespace Examples
{
    class PositionJavaPinButtonExample : public IExample
    {
    	AndroidNativeState& m_nativeState;
    	Eegeo::Rendering::RenderContext& m_renderContext;

    	int m_buttonID;
    	jmethodID m_updateLocationMethodId;

    public:
    	PositionJavaPinButtonExample(
    			AndroidNativeState& pNativeState,
    			Eegeo::Rendering::RenderContext& renderContext);

        void Start();
        void Update() {}
        void Draw();
        void Suspend() {}

    private:
        void Project (const Eegeo::Space::LatLongAltitude& location, Eegeo::v3& screenPosition);
    };
}

#endif /* POSITIONJAVAPINBUTTON_H_ */
