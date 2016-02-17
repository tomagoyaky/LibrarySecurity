LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_INCLUDE_FILES		:= $(LOCAL_PATH)/../inc
LOCAL_MODULE    := UnityDllEncrypt
LOCAL_SRC_FILES := UnityDllEncrypt.cpp
LOCAL_LDFLAGS 	:= $(LOCAL_PATH)/../hooklib/armeabi/libthook.a

include $(BUILD_SHARED_LIBRARY)
