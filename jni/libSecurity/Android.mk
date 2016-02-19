LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_LDLIBS 	:= -llog -fPIC
	
LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/../inc \
	$(LOCAL_PATH)/../libanti \
	$(LOCAL_PATH)/../libshook \
	$(LOCAL_PATH)/../libCommonUtils \
	$(LOCAL_PATH)/../libCMPT
	
LOCAL_STATIC_LIBRARIES := libanti \
	libshook \
	libCommonUtils \
	libCMPT
	
LOCAL_MODULE    := Security
LOCAL_SRC_FILES := Security.cpp \
	NativeBridge.cpp \
	HookManager.cpp \
	AppalyStackOverFlow.cpp \
	CheckJDWP.cpp \
	CheckCodeSignature.cpp \
	CheckSignature.cpp
	
#include $(BUILD_STATIC_LIBRARY)
include $(BUILD_SHARED_LIBRARY)

