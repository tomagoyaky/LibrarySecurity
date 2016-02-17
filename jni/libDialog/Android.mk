LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_LDLIBS 	:= -llog -fPIC
LOCAL_MODULE    := dialog
LOCAL_SRC_FILES := dialog.cpp
include $(BUILD_STATIC_LIBRARY)
#include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_EXECUTABLE)
