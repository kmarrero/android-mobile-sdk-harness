#ifndef PERSISTENTAPPSTATE_H_
#define PERSISTENTAPPSTATE_H_

#include "Types.h"

struct PersistentAppState
{
	PersistentAppState(PersistentAppState* pValues)
	:lastGlobeCameraLatLong(0,0,0)
	{
		valid = pValues!=NULL;
		if(valid)
		{
			lastGlobeCameraLatLong = pValues->lastGlobeCameraLatLong;
			lastGlobeCameraHeadingDegrees = pValues->lastGlobeCameraHeadingDegrees;
			lastGlobeCameraDistanceToInterest = pValues->lastGlobeCameraDistanceToInterest;
		}
	}

	bool valid;
	Eegeo::Space::LatLongAltitude lastGlobeCameraLatLong;
	float lastGlobeCameraHeadingDegrees;
	float lastGlobeCameraDistanceToInterest;
};

#endif
