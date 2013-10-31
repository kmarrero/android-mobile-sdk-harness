/*
 * LocationVisitNativeAPI.h
 *
 *  Created on: Oct 30, 2013
 *      Author: kimbleoperations
 */

#ifndef LOCATIONVISITNATIVEAPI_H_
#define LOCATIONVISITNATIVEAPI_H_

#include <jni.h>

extern "C"
{
    JNIEXPORT void JNICALL Java_com_eegeo_MainActivity_visitLocation(JNIEnv* jenv, jobject obj, jlong nativeAppWindowPtr, jstring location);
}

#endif /* LOCATIONVISITNATIVEAPI_H_ */
