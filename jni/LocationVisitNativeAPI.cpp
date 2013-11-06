/*
 * LocationVisitNativeAPI.cpp
 *
 *  Created on: Oct 30, 2013
 *      Author: kimbleoperations
 */

#include "LocationVisitNativeAPI.h"
#include "AppWindow.hpp"
#include "AppOnMap.h"
#include "LatLongAltitude.h"
#include <map>
#include <string>

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

JNIEXPORT void JNICALL Java_com_eegeo_MainActivity_visitLocation(JNIEnv* jenv, jobject obj, jlong nativeAppWindowPtr, jstring location)
{
	static bool initialised = false;
	static std::map<std::string, ViewLocation> locations;

	if(!initialised)
	{
		locations["NYC"] = ViewLocation(40.703762, -74.013733, 0, 240.0f, 1800.0f);
		locations["London"] = ViewLocation(51.506172,-0.118915, 0, 351.0f, 2731.0f);
		locations["SF"] = ViewLocation(37.7858,-122.401, 0.0, 0.0, 1781.0);
		locations["Kyoto"] = ViewLocation(34.9977166,135.755402, 0, 0.0f, 1500.0f);
		locations["Melbourne"] = ViewLocation(-37.815005, 144.964415, 0, 0.0f, 1500.0f);
		initialised = true;
	}

	AppWindow* appWindow = (AppWindow*)nativeAppWindowPtr;
	MyApp& myApp = appWindow->GetAppOnMap();

	//just assume the string from the Java is correct... in reality would have error handling etc
	const char* chars = jenv->GetStringUTFChars(location, 0);
	ViewLocation& result = locations[chars];
	jenv->ReleaseStringUTFChars(location, chars);

	//go to the location
	myApp.JumpTo(
			result.location.GetLatitudeInDegrees(),
			result.location.GetLongitudeInDegrees(),
			result.location.GetAltitude(),
			result.heading,
			result.distance);
}
