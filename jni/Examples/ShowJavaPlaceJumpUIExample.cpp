/*
 * ShowJavaPlaceJumpUIExample.cpp
 *
 *  Created on: Oct 30, 2013
 *      Author: kimbleoperations
 */

#include <jni.h>

#include "ShowJavaPlaceJumpUIExample.h"

namespace Examples
{
	ShowJavaPlaceJumpUIExample::ShowJavaPlaceJumpUIExample(AndroidNativeState& nativeState)
	:m_nativeState(nativeState)
    {
    }

    void ShowJavaPlaceJumpUIExample::Start()
    {
    	JNIEnv* env = NULL;
    	JavaVM& vm = *m_nativeState.vm;

    	jint result = vm.AttachCurrentThread(&env, NULL);
		{
	    	Eegeo_ASSERT(result == 0);
			jclass activityClass = m_nativeState.activityClass;
			jmethodID showVisitMenu = env->GetMethodID(activityClass, "showVisitMenu", "()V");
			env->CallVoidMethod(m_nativeState.activity, showVisitMenu);
		}
    }
}
