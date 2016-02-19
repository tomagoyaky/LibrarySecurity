LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_CPPFLAGS += -std=c++11
LOCAL_LDLIBS 	:= -llog -fPIC
LOCAL_STATIC_LIBRARIES := E:\Android\android-ndk-r10\sources\cxx-stl\stlport\libs\armeabi\libstlport_static.a
#	/Users/admin/Documents/android-ndk-r10b/sources/cxx-stl/stlport/libs/arm64-v8a/libstlport_static.a
LOCAL_C_INCLUDES :=  libunhook \
	E:\Android\android-ndk-r10\sources\cxx-stl\stlport\stlport
#	 /Users/admin/Documents/android-ndk-r10b/sources/cxx-stl/stlport/stlport
LOCAL_MODULE    := CommonUtils
LOCAL_SRC_FILES := processinfo.cpp \
	ThreadUtil.cpp
LOCAL_EXPORT_C_INCLUDES := processinfo.h \
	ThreadUtil.h

include $(BUILD_STATIC_LIBRARY)
#include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_EXECUTABLE)
