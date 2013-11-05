/*
 * PositionJavaPinButton.cpp
 *
 *  Created on: Oct 31, 2013
 *      Author: kimbleoperations
 */

#include <jni.h>
#include "VectorMath.h"
#include "LatLongAltitude.h"
#include "PositionJavaPinButtonExample.h"

namespace Examples
{
	PositionJavaPinButtonExample::PositionJavaPinButtonExample(
			AndroidNativeState& nativeState,
			Eegeo::Rendering::RenderContext& renderContext,
			Eegeo::Camera::CameraModel& cameraModel)
	:m_nativeState(nativeState)
	,m_renderContext(renderContext)
	,m_cameraModel(cameraModel)
    {
    }

    void PositionJavaPinButtonExample::Start()
    {
    	JNIEnv* env = NULL;
    	JavaVM& vm = *m_nativeState.vm;

    	jint result = vm.AttachCurrentThread(&env, NULL);
		{
	    	Eegeo_ASSERT(result == 0);

			jmethodID addJavaPinButton = env->GetMethodID(m_nativeState.activityClass, "addJavaPinButton", "()I");
			m_buttonID = env->CallIntMethod(m_nativeState.activity, addJavaPinButton);
			m_updateLocationMethodId = env->GetMethodID(m_nativeState.activityClass, "updatePinButtonScreenLocation", "(IFF)V");
		}
    }

    void PositionJavaPinButtonExample::Draw()
    {
    	JNIEnv* env = NULL;
    	JavaVM& vm = *m_nativeState.vm;

    	const Eegeo::Space::LatLongAltitude pinLocation(37.795118f,-122.402797f,0.f, Eegeo::Space::LatLongUnits::Degrees);
    	Eegeo::v3 screenPosition;
    	Project(pinLocation, screenPosition);

    	jint result = vm.AttachCurrentThread(&env, NULL);
		{
			env->CallVoidMethod(
				m_nativeState.activity,
				m_updateLocationMethodId,
				m_buttonID,
				screenPosition.GetX(),
				screenPosition.GetY()
			);
		}
    }

    void PositionJavaPinButtonExample::Project (const Eegeo::Space::LatLongAltitude& location, Eegeo::v3& screenPosition)
	{
		float viewport[] = {0, 0, m_renderContext.GetScreenWidth(), m_renderContext.GetScreenHeight()};
		Eegeo::m44 finalMatrix;

		Eegeo::m44::Mul (finalMatrix,
				m_renderContext.GetProjectionMatrix(),
				m_renderContext.GetViewMatrix());

		Eegeo::v3 local = (location.ToECEF() - m_cameraModel.GetWorldPosition()).ToSingle();
		Eegeo::v4 inVector(local, 1.0f);

		Eegeo::v4 outVector = Eegeo::v4::Mul(inVector, finalMatrix);

		screenPosition.SetX((outVector.GetX()/outVector.GetW()));
		screenPosition.SetY((outVector.GetY()/outVector.GetW()));
		screenPosition.SetZ((outVector.GetZ()/outVector.GetW()));

		screenPosition.SetX((screenPosition.GetX() + 1.0f) * 0.5f);
		screenPosition.SetY(1.0f - ((screenPosition.GetY() + 1.0f) * 0.5f));

		screenPosition.SetX((screenPosition.GetX()*(viewport[2]-viewport[0])) + viewport[0]);
		screenPosition.SetY((screenPosition.GetY()*(viewport[3]-viewport[1])) + viewport[1]);
	}
}
