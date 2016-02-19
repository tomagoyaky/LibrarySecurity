LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_LDLIBS 	:= -llog -fPIC
LOCAL_MODULE    := dialog
LOCAL_SRC_FILES := dialog.cpp \
	DialogServer.cpp
	
LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/../inc
LOCAL_EXPORT_C_INCLUDES := dialog.h
#LOCAL_SHARED_LIBRARIES:=libutils libbinder
LOCAL_STATIC_LIBRARIES := libutils libbinder
include $(BUILD_STATIC_LIBRARY)
#include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_EXECUTABLE)
