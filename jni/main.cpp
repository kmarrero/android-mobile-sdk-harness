#include <jni.h>
#include "AppWindow.hpp"
#include "main.h"
#include "TouchEventWrapper.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

using namespace Eegeo::Android;
using namespace Eegeo::Android::Input;

AppWindow* g_pAppWindow;
AndroidNativeState g_nativeState;
PersistentAppState g_persistentAppState;
bool firstTime = true;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* pvt)
{
	g_nativeState.vm = vm;
    return JNI_VERSION_1_2;
}

JNIEXPORT void JNICALL Java_com_eegeo_MainActivity_startNativeCode(JNIEnv* jenv, jobject obj, jobject activity, jobject assetManager)
{
	g_nativeState.env = jenv;
	g_nativeState.activity = activity;
	g_nativeState.assetManager = AAssetManager_fromJava(jenv, assetManager);
	PersistentAppState* pPersistentAppState = firstTime ? NULL : &g_persistentAppState;
	g_pAppWindow = new AppWindow(&g_nativeState, pPersistentAppState);
	firstTime = false;
}

JNIEXPORT void JNICALL Java_com_eegeo_MainActivity_stopNativeCode(JNIEnv* jenv, jobject obj)
{
	delete g_pAppWindow;
	g_pAppWindow = NULL;
}

JNIEXPORT void JNICALL Java_com_eegeo_MainActivity_pauseNativeCode(JNIEnv* jenv, jobject obj)
{
	g_pAppWindow->Pause(&g_persistentAppState);
}

JNIEXPORT void JNICALL Java_com_eegeo_MainActivity_resumeNativeCode(JNIEnv* jenv, jobject obj)
{
	g_pAppWindow->Resume();
}

JNIEXPORT void JNICALL Java_com_eegeo_MainActivity_setNativeSurface(JNIEnv* jenv, jobject obj, jobject surface)
{
    if (surface != NULL)
    {
    	g_nativeState.window = ANativeWindow_fromSurface(jenv, surface);
    	g_pAppWindow->ActivateSurface();
    }
    else
    {
        ANativeWindow_release(g_nativeState.window);
        g_nativeState.window = NULL;
    }
}

JNIEXPORT void JNICALL Java_com_eegeo_MainActivity_processNativePointerDown(JNIEnv* jenv, jobject obj,
		jint primaryActionIndex,
		jint primaryActionIdentifier,
		jint numPointers,
		jfloatArray x,
		jfloatArray y,
		jintArray pointerIdentity,
		jintArray pointerIndex)
{
	TouchInputEvent e(false, true, primaryActionIndex, primaryActionIdentifier);

	jfloat* xBuffer = jenv->GetFloatArrayElements(x, 0);
	jfloat* yBuffer = jenv->GetFloatArrayElements(y, 0);
	jint* identityBuffer = jenv->GetIntArrayElements(pointerIdentity, 0);
	jint* indexBuffer = jenv->GetIntArrayElements(pointerIndex, 0);

	for(int i = 0; i < numPointers; ++ i)
	{
		TouchInputPointerEvent p(xBuffer[i], yBuffer[i], identityBuffer[i], indexBuffer[i]);
		e.pointerEvents.push_back(p);
	}

	jenv->ReleaseFloatArrayElements(x, xBuffer, 0);
	jenv->ReleaseFloatArrayElements(y, yBuffer, 0);
	jenv->ReleaseIntArrayElements(pointerIdentity, identityBuffer, 0);
	jenv->ReleaseIntArrayElements(pointerIndex, indexBuffer, 0);

	g_pAppWindow->EnqueuePointerDown(e);
}

JNIEXPORT void JNICALL Java_com_eegeo_MainActivity_processNativePointerUp(JNIEnv* jenv, jobject obj,
		jint primaryActionIndex,
		jint primaryActionIdentifier,
		jint numPointers,
		jfloatArray x,
		jfloatArray y,
		jintArray pointerIdentity,
		jintArray pointerIndex)
{
	TouchInputEvent e(true, false, primaryActionIndex, primaryActionIdentifier);

	jfloat* xBuffer = jenv->GetFloatArrayElements(x, 0);
	jfloat* yBuffer = jenv->GetFloatArrayElements(y, 0);
	jint* identityBuffer = jenv->GetIntArrayElements(pointerIdentity, 0);
	jint* indexBuffer = jenv->GetIntArrayElements(pointerIndex, 0);

	for(int i = 0; i < numPointers; ++ i)
	{
		TouchInputPointerEvent p(xBuffer[i], yBuffer[i], identityBuffer[i], indexBuffer[i]);
		e.pointerEvents.push_back(p);
	}

	jenv->ReleaseFloatArrayElements(x, xBuffer, 0);
	jenv->ReleaseFloatArrayElements(y, yBuffer, 0);
	jenv->ReleaseIntArrayElements(pointerIdentity, identityBuffer, 0);
	jenv->ReleaseIntArrayElements(pointerIndex, indexBuffer, 0);

	g_pAppWindow->EnqueuePointerUp(e);
}

JNIEXPORT void JNICALL Java_com_eegeo_MainActivity_processNativePointerMove(JNIEnv* jenv, jobject obj,
		jint primaryActionIndex,
		jint primaryActionIdentifier,
		jint numPointers,
		jfloatArray x,
		jfloatArray y,
		jintArray pointerIdentity,
		jintArray pointerIndex)
{
	TouchInputEvent e(false, false, primaryActionIndex, primaryActionIdentifier);

	jfloat* xBuffer = jenv->GetFloatArrayElements(x, 0);
	jfloat* yBuffer = jenv->GetFloatArrayElements(y, 0);
	jint* identityBuffer = jenv->GetIntArrayElements(pointerIdentity, 0);
	jint* indexBuffer = jenv->GetIntArrayElements(pointerIndex, 0);

	for(int i = 0; i < numPointers; ++ i)
	{
		TouchInputPointerEvent p(xBuffer[i], yBuffer[i], identityBuffer[i], indexBuffer[i]);
		e.pointerEvents.push_back(p);
	}

	jenv->ReleaseFloatArrayElements(x, xBuffer, 0);
	jenv->ReleaseFloatArrayElements(y, yBuffer, 0);
	jenv->ReleaseIntArrayElements(pointerIdentity, identityBuffer, 0);
	jenv->ReleaseIntArrayElements(pointerIndex, indexBuffer, 0);

	g_pAppWindow->EnqueuePointerMove(e);
}
