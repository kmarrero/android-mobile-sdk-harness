#include <jni.h>
#include "AppWindow.hpp"
#include "main.h"
#include "TouchEventWrapper.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <pthread.h>

using namespace Eegeo::Android;
using namespace Eegeo::Android::Input;

AppWindow* g_pAppWindow;
AndroidNativeState g_nativeState;
PersistentAppState g_persistentAppState;
bool firstTime = true;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* pvt)
{
	g_nativeState.vm = vm;
    return JNI_VERSION_1_6;
}

//lifecycle
JNIEXPORT long JNICALL Java_com_eegeo_MainActivity_startNativeCode(JNIEnv* jenv, jobject obj, jobject activity, jobject assetManager)
{
	g_nativeState.javaMainThread = pthread_self();
	g_nativeState.mainThreadEnv = jenv;
	g_nativeState.activity = jenv->NewGlobalRef(activity);
	g_nativeState.activityClass = (jclass)jenv->NewGlobalRef(jenv->FindClass("com/eegeo/MainActivity"));

	jmethodID getClassLoader = jenv->GetMethodID(g_nativeState.activityClass,"getClassLoader", "()Ljava/lang/ClassLoader;");
	g_nativeState.classLoader = jenv->NewGlobalRef(jenv->CallObjectMethod(activity, getClassLoader));
	g_nativeState.classLoaderClass  = (jclass)jenv->NewGlobalRef(jenv->FindClass("java/lang/ClassLoader"));
	g_nativeState.classLoaderLoadClassMethod = jenv->GetMethodID(g_nativeState.classLoaderClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");

	jthrowable exc;
	exc = jenv->ExceptionOccurred();

	if (exc)
	{
		jenv->ExceptionDescribe();
		jenv->ExceptionClear();
		return 0;
	}

	g_nativeState.assetManagerGlobalRef = jenv->NewGlobalRef(assetManager);
	g_nativeState.assetManager = AAssetManager_fromJava(jenv, g_nativeState.assetManagerGlobalRef);
	PersistentAppState* pPersistentAppState = firstTime ? NULL : &g_persistentAppState;
	g_pAppWindow = new AppWindow(&g_nativeState, pPersistentAppState);
	firstTime = false;

	return ((long)g_pAppWindow);
}

JNIEXPORT void JNICALL Java_com_eegeo_MainActivity_stopNativeCode(JNIEnv* jenv, jobject obj)
{
	jenv->DeleteGlobalRef(g_nativeState.assetManagerGlobalRef);
	jenv->DeleteGlobalRef(g_nativeState.activity);
	jenv->DeleteGlobalRef(g_nativeState.activityClass);
	jenv->DeleteGlobalRef(g_nativeState.classLoaderClass);
	jenv->DeleteGlobalRef(g_nativeState.classLoader);
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

//input
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

	jenv->ReleaseFloatArrayElements(x, xBuffer, JNI_ABORT);
	jenv->ReleaseFloatArrayElements(y, yBuffer, JNI_ABORT);
	jenv->ReleaseIntArrayElements(pointerIdentity, identityBuffer, JNI_ABORT);
	jenv->ReleaseIntArrayElements(pointerIndex, indexBuffer, JNI_ABORT);

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

	jenv->ReleaseFloatArrayElements(x, xBuffer, JNI_ABORT);
	jenv->ReleaseFloatArrayElements(y, yBuffer, JNI_ABORT);
	jenv->ReleaseIntArrayElements(pointerIdentity, identityBuffer, JNI_ABORT);
	jenv->ReleaseIntArrayElements(pointerIndex, indexBuffer, JNI_ABORT);

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

	jenv->ReleaseFloatArrayElements(x, xBuffer, JNI_ABORT);
	jenv->ReleaseFloatArrayElements(y, yBuffer, JNI_ABORT);
	jenv->ReleaseIntArrayElements(pointerIdentity, identityBuffer, JNI_ABORT);
	jenv->ReleaseIntArrayElements(pointerIndex, indexBuffer, JNI_ABORT);

	g_pAppWindow->EnqueuePointerMove(e);
}
