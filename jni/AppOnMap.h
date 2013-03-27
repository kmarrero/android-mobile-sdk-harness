/*
 * AppOnMap.h
 *
 *  Created on: Mar 26, 2013
 *      Author: eegeo
 */

#ifndef APPONMAP_H_
#define APPONMAP_H_

#include "IAppOnMap.h"
#include "EegeoWorld.h"
#include "RenderContext.h"
#include "NewGlobeCamera.h"

class AppOnMap : public Eegeo::IAppOnMap
{
public:
    virtual ~AppOnMap() { }

    void OnStart ()
    {
    	Eegeo::Camera::NewGlobeCamera& controller = ((Eegeo::Camera::NewGlobeCamera&)World().GetCameraController());

    	controller.SetInterestHeadingDistance(Eegeo::Space::LatLongAltitude(51.506172,-0.118915, 0, Eegeo::Space::LatLongUnits::Degrees),
                                                        351.0f,
                                                        2731.0f);
    }

    void Update (float dt)
    {
        World().Update(dt);
    }

    void Draw (float dt)
    {
        World().Draw(dt);
    }

    void Event_TouchRotate 			(const AppInterface::RotateData& data) { World().GetCameraController().Event_TouchRotate(data); }
    void Event_TouchRotate_Start	(const AppInterface::RotateData& data) { World().GetCameraController().Event_TouchRotate_Start(data); }
    void Event_TouchRotate_End 		(const AppInterface::RotateData& data) { World().GetCameraController().Event_TouchRotate_End(data); }

    void Event_TouchPinch 			(const AppInterface::PinchData& data) { World().GetCameraController().Event_TouchPinch(data); }
    void Event_TouchPinch_Start 	(const AppInterface::PinchData& data) { World().GetCameraController().Event_TouchPinch_Start(data); }
    void Event_TouchPinch_End 		(const AppInterface::PinchData& data) { World().GetCameraController().Event_TouchPinch_End(data); }

    void Event_TouchPan				(const AppInterface::PanData& data) { World().GetCameraController().Event_TouchPan(data); }
    void Event_TouchPan_Start		(const AppInterface::PanData& data) { World().GetCameraController().Event_TouchPan_Start(data); }
    void Event_TouchPan_End 		(const AppInterface::PanData& data) { World().GetCameraController().Event_TouchPan_End(data); }

    void Event_TouchTap 			(const AppInterface::TapData& data) { World().GetCameraController().Event_TouchTap(data); }
    void Event_TouchDoubleTap		(const AppInterface::TapData& data) { World().GetCameraController().Event_TouchDoubleTap(data); }

    void Event_TouchDown 			(const AppInterface::TouchData& data) { World().GetCameraController().Event_TouchDown(data); }
    void Event_TouchMove 			(const AppInterface::TouchData& data) { World().GetCameraController().Event_TouchMove(data); }
    void Event_TouchUp 				(const AppInterface::TouchData& data) { World().GetCameraController().Event_TouchUp(data); }
};


#endif /* APPONMAP_H_ */
