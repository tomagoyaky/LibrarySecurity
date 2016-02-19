LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_LDLIBS 	:= -llog -fPIC
LOCAL_MODULE    := SandBox
LOCAL_SRC_FILES := SandBox.cpp
LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/../inc \
	$(LOCAL_PATH)/libhook
	
LOCAL_EXPORT_C_INCLUDES := SandBox.h

LOCAL_STATIC_LIBRARIES := libhook
include $(BUILD_STATIC_LIBRARY)
#include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_EXECUTABLE)
