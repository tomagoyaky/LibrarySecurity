
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES	:=	$(LOCAL_PATH)/include \
						$(LOCAL_PATH)/../ 


LOCAL_SRC_FILES := SHook.cpp
#  	Hooker.cpp Debug.cpp PosixMemory.cpp hook.cpp

LOCAL_CFLAGS :=  

ifeq ($(TARGET_ARCH),x86)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/hde64c/include
LOCAL_CFLAGS +=
LOCAL_SRC_FILES += hde64c/src/hde64.c
endif


LOCAL_MODULE := libshook
LOCAL_MODULE_TAGS := optional
 
LOCAL_LDLIBS := -llog    -fPIC  
#include $(BUILD_STATIC_EXECUTABLE)
#include $(BUILD_EXECUTABLE)
include $(BUILD_STATIC_LIBRARY)
