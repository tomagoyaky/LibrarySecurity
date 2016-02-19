
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := hook.cpp
#  	Hooker.cpp Debug.cpp PosixMemory.cpp hook.cpp

LOCAL_CFLAGS :=  

ifeq ($(TARGET_ARCH),x86)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/hde64c/include
LOCAL_CFLAGS +=
LOCAL_SRC_FILES += hde64c/src/hde64.c
endif
LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/../inc 
LOCAL_EXPORT_C_INCLUDES := hook.h

LOCAL_MODULE := shook
LOCAL_MODULE_TAGS := optional
 
LOCAL_LDLIBS := -llog    -fPIC  
#include $(BUILD_STATIC_EXECUTABLE)
#include $(BUILD_EXECUTABLE)
include $(BUILD_STATIC_LIBRARY)

include $(call all-makefiles-under,$(LOCAL_PATH))