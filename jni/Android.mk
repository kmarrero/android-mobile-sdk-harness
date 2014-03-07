LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := native-activity-lib
LOCAL_SRC_FILES := ./../libs/libnative-activity-lib.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := native-activity
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2 -L./libs/ -lpng -lz -lm -L./libs/ -lcrypto -L./libs/ -lssl -L./libs/ -lcurl -L./libs/ -lcares -L./libs/ -lsimd -L./libs/ -lmyjpeg
LOCAL_STATIC_LIBRARIES := android_native_app_glue native-activity-lib
 
LOCAL_SRC_FILES += ./main.cpp
LOCAL_SRC_FILES += ./AppWindow.cpp
LOCAL_SRC_FILES += ./AppOnMap.cpp
LOCAL_SRC_FILES += ./ExampleCameraJumpController.cpp
LOCAL_SRC_FILES += ./UiThreadToNativeThreadTaskQueue.cpp

LOCAL_SRC_FILES += ./Examples/ControlCityThemeExample.cpp
LOCAL_SRC_FILES += ./Examples/DebugSphereExample.cpp
LOCAL_SRC_FILES += ./Examples/EnvironmentFlatteningExample.cpp
LOCAL_SRC_FILES += ./Examples/EnvironmentNotifierExample.cpp
LOCAL_SRC_FILES += ./Examples/FileIOExample.cpp
LOCAL_SRC_FILES += ./Examples/LoadModelExample.cpp
LOCAL_SRC_FILES += ./Examples/ModifiedRenderingExample.cpp
LOCAL_SRC_FILES += ./Examples/NavigationGraphExample.cpp
LOCAL_SRC_FILES += ./Examples/ResourceSpatialQueryExample.cpp
LOCAL_SRC_FILES += ./Examples/ScreenUnprojectExample.cpp
LOCAL_SRC_FILES += ./Examples/SearchExample.cpp
LOCAL_SRC_FILES += ./Examples/ToggleTrafficExample.cpp
LOCAL_SRC_FILES += ./Examples/WebRequestExample.cpp
LOCAL_SRC_FILES += ./Examples/KeyboardInputExample.cpp
LOCAL_SRC_FILES += ./Examples/PODAnimationExample.cpp
LOCAL_SRC_FILES += ./Examples/Pick3DObjectExample.cpp
LOCAL_SRC_FILES += ./Examples/ShowJavaPlaceJumpUIExample.cpp
LOCAL_SRC_FILES += ./Examples/PositionJavaPinButtonExample.cpp
LOCAL_SRC_FILES += ./Examples/ScreenPickExample.cpp
LOCAL_SRC_FILES += ./Examples/DebugPrimitiveRenderingExample.cpp
LOCAL_SRC_FILES += ./Examples/RouteDrawingExample.cpp
LOCAL_SRC_FILES += ./Examples/PinsExample.cpp
LOCAL_SRC_FILES += ./Examples/RouteSimulationExample.cpp
LOCAL_SRC_FILES += ./Examples/RouteThicknessPolicyExample.cpp
LOCAL_SRC_FILES += ./Examples/JavaHudCrossThreadCommunicationExample.cpp
LOCAL_SRC_FILES += ./Examples/RouteMatchingExample.cpp
LOCAL_SRC_FILES += ./Examples/RouteSimulationAnimationExample.cpp
LOCAL_SRC_FILES += ./Examples/DynamicText3DExample.cpp
LOCAL_SRC_FILES += ./Examples/SingleCityExample.cpp
LOCAL_SRC_FILES += ./Examples/PinOverModelExample.cpp
LOCAL_SRC_FILES += ./Examples/TrafficCongestionExample.cpp

os_name:=$(shell uname -s)

ifeq ($(os_name),Darwin)
	platformincludes := $(shell find ./libs/platform -type d ! -path "*/OSX/*" ! -path "*/iOS/*")
	LOCAL_C_INCLUDES := $(platformincludes:$(LOCAL_PATH)/%=%)
else
	# assume windows if not specified for now (due to no uname)
	platformincludes := $(shell dir .\libs\platform /ad-h /s /b)
	LOCAL_C_INCLUDES := $(platformincludes:$(LOCAL_PATH)/%=%)
	LOCAL_C_INCLUDES += ./libs/platform 
endif 

LOCAL_C_INCLUDES += ./jni/Examples 

LOCAL_C_INCLUDES += ./libs/png
LOCAL_C_INCLUDES += ./libs/curl 
LOCAL_C_INCLUDES += ./libs/jpeg  

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
