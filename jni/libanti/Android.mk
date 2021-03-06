LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_CPPFLAGS += -std=c++11
LOCAL_LDLIBS 	:= -llog -fPIC
LOCAL_STATIC_LIBRARIES := E:\Android\android-ndk-r10\sources\cxx-stl\stlport\libs\armeabi\libstlport_static.a
#	/Users/admin/Documents/android-ndk-r10b/sources/cxx-stl/stlport/libs/arm64-v8a/libstlport_static.a
LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/../inc \
	$(LOCAL_PATH)/../libhook \
	$(LOCAL_PATH)/../libCommonUtils \
	$(LOCAL_PATH)/../libCMPT \
	E:\Android\android-ndk-r10\sources\cxx-stl\stlport\stlport
#	 /Users/admin/Documents/android-ndk-r10b/sources/cxx-stl/stlport/stlport

LOCAL_EXPORT_C_INCLUDES := anti.h
LOCAL_STATIC_LIBRARIES := libhook \
	libCommonUtils \
	libCMPT
	
LOCAL_MODULE    := anti
LOCAL_SRC_FILES := anti.cpp \
	AntiParams.cpp \
	CheckCheater.cpp \
	CheckTrace.cpp \
	CheckMemory.cpp
	
include $(BUILD_STATIC_LIBRARY)
#include $(BUILD_SHARED_LIBRARY)
