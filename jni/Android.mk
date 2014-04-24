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
LOCAL_SRC_FILES += ./AppProxy.cpp
LOCAL_SRC_FILES += ./AppRunner.cpp
LOCAL_SRC_FILES += ./AppHost.cpp
LOCAL_SRC_FILES += ./AppOnMap.cpp
LOCAL_SRC_FILES += ./AppMessages/AppStartGameMessage.cpp
LOCAL_SRC_FILES += ./AppLifecycleMessages/AppPauseMessage.cpp
LOCAL_SRC_FILES += ./AppLifecycleMessages/AppDisplayAvailableMessage.cpp
LOCAL_SRC_FILES += ./InputMessages/TouchEventMessage.cpp
LOCAL_SRC_FILES += ./GlDisplayService.cpp
LOCAL_SRC_FILES += ./Game/DefaultGame.cpp
LOCAL_SRC_FILES += ./ExampleCameraJumpController.cpp

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
LOCAL_C_INCLUDES += ./jni/AppMessages
LOCAL_C_INCLUDES += ./jni/AppLifecycleMessages
LOCAL_C_INCLUDES += ./jni/InputMessages
LOCAL_C_INCLUDES += ./jni/Game

LOCAL_C_INCLUDES += ./libs/png
LOCAL_C_INCLUDES += ./libs/curl 
LOCAL_C_INCLUDES += ./libs/jpeg  

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
