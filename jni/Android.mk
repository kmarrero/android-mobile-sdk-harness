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

LOCAL_C_INCLUDES += ./jni/Examples 

LOCAL_C_INCLUDES += ./libs/png
LOCAL_C_INCLUDES += ./libs/curl 
LOCAL_C_INCLUDES += ./libs/jpeg  

LOCAL_C_INCLUDES += ./libs/platform
LOCAL_C_INCLUDES += ./libs/platform/Android
LOCAL_C_INCLUDES += ./libs/platform/Android/Input/
LOCAL_C_INCLUDES += ./libs/platform/Android/WebRequestCacheTasks/
LOCAL_C_INCLUDES += ./libs/platform/Android/KtxTextures/
LOCAL_C_INCLUDES += ./libs/platform/Base
LOCAL_C_INCLUDES += ./libs/platform/Base/Model
LOCAL_C_INCLUDES += ./libs/platform/Camera
LOCAL_C_INCLUDES += ./libs/platform/Culling
LOCAL_C_INCLUDES += ./libs/platform/DataStructures
LOCAL_C_INCLUDES += ./libs/platform/Debug
LOCAL_C_INCLUDES += ./libs/platform/Geometry
LOCAL_C_INCLUDES += ./libs/platform/Helpers
LOCAL_C_INCLUDES += ./libs/platform/Io
LOCAL_C_INCLUDES += ./libs/platform/Io/ChunkedFileFormat
LOCAL_C_INCLUDES += ./libs/platform/Io/POD
LOCAL_C_INCLUDES += ./libs/platform/Lighting
LOCAL_C_INCLUDES += ./libs/platform/Location
LOCAL_C_INCLUDES += ./libs/platform/Rendering
LOCAL_C_INCLUDES += ./libs/platform/Rendering/Materials
LOCAL_C_INCLUDES += ./libs/platform/Resources
LOCAL_C_INCLUDES += ./libs/platform/Resources/Buildings
LOCAL_C_INCLUDES += ./libs/platform/Resources/CityThemes
LOCAL_C_INCLUDES += ./libs/platform/Resources/CustomTextures
LOCAL_C_INCLUDES += ./libs/platform/Resources/Landmarks
LOCAL_C_INCLUDES += ./libs/platform/Resources/Midoki
LOCAL_C_INCLUDES += ./libs/platform/Resources/PlaceNames
LOCAL_C_INCLUDES += ./libs/platform/Resources/Roads
LOCAL_C_INCLUDES += ./libs/platform/Resources/Roads/Navigation
LOCAL_C_INCLUDES += ./libs/platform/Resources/Shadows
LOCAL_C_INCLUDES += ./libs/platform/Resources/Terrain
LOCAL_C_INCLUDES += ./libs/platform/Resources/Terrain/Heights
LOCAL_C_INCLUDES += ./libs/platform/Resources/Terrain/Lcm
LOCAL_C_INCLUDES += ./libs/platform/Resources/Terrain/Placeholder
LOCAL_C_INCLUDES += ./libs/platform/Resources/Terrain/Raster
LOCAL_C_INCLUDES += ./libs/platform/Search
LOCAL_C_INCLUDES += ./libs/platform/Search/Service
LOCAL_C_INCLUDES += ./libs/platform/Space
LOCAL_C_INCLUDES += ./libs/platform/Streaming
LOCAL_C_INCLUDES += ./libs/platform/Streaming/CoverageTrees
LOCAL_C_INCLUDES += ./libs/platform/Streaming/Loaders
LOCAL_C_INCLUDES += ./libs/platform/Streaming/Payload
LOCAL_C_INCLUDES += ./libs/platform/Streaming/Prioritisers
LOCAL_C_INCLUDES += ./libs/platform/Streaming/Quadtree
LOCAL_C_INCLUDES += ./libs/platform/Streaming/SceneGraph
LOCAL_C_INCLUDES += ./libs/platform/Streaming/Stream
LOCAL_C_INCLUDES += ./libs/platform/Streaming/Volume
LOCAL_C_INCLUDES += ./libs/platform/Traffic
LOCAL_C_INCLUDES += ./libs/platform/Web
LOCAL_C_INCLUDES += ./libs/platform/Weather
include $(BUILD_SHARED_LIBRARY)


$(call import-module,android/native_app_glue)
