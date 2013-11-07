/*
 * ShowJavaPlaceJumpUIExample.h
 *
 *  Created on: Oct 30, 2013
 *      Author: kimbleoperations
 */

#ifndef SHOWJAVAPLACEJUMPUIEXAMPLE_H_
#define SHOWJAVAPLACEJUMPUIEXAMPLE_H_

#include "IExample.h"
#include "AndroidNativeState.h"
#include "LatLongAltitude.h"
#include "Camera.h"

#include <map>
#include <string>
#include <jni.h>

extern "C"
{
    JNIEXPORT void JNICALL Java_com_eegeo_examples_PlaceJumpMenu_visitLocation(JNIEnv* jenv, jobject obj, jlong nativeAppWindowPtr, jstring location);
}

namespace Examples
{
    class ShowJavaPlaceJumpUIExample : public IExample
    {
    	struct ViewLocation
    	{
    		ViewLocation()
    		:location(0, 0, 0, Eegeo::Space::LatLongUnits::Degrees)
    		{ }

    		ViewLocation(float lat, float lon, float alt, float heading, float distance)
    		:location(lat, lon, alt, Eegeo::Space::LatLongUnits::Degrees)
    		,heading(heading)
    		,distance(distance) { }

    		Eegeo::Space::LatLongAltitude location;
    		float heading;
    		float distance;
    	};

    	std::map<std::string, ViewLocation> m_locations;
    	AndroidNativeState& m_nativeState;
        Eegeo::Camera::ICameraJumpController& m_cameraJumpController;

    	jclass m_placeJumpMenuClass;
    	jobject m_placeJumpMenu;

    public:
    	ShowJavaPlaceJumpUIExample(
    			AndroidNativeState& pNativeState,
    			Eegeo::Camera::ICameraJumpController& cameraJumpController);

    	void JumpToLocation(const std::string& location);
        void Start();
        void Update() {}
        void Draw() {}
        void Suspend();
    };
}

#endif /* SHOWJAVAPLACEJUMPUIEXAMPLE_H_ */
