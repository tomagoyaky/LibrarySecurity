LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_LDLIBS 	:= -llog -fPIC
LOCAL_MODULE    := hook
LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/../inc \
	$(LOCAL_PATH)/ElfHook \
	$(LOCAL_PATH)/JavaHook
	
	
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
	
include $(BUILD_STATIC_LIBRARY)
#include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_EXECUTABLE)
