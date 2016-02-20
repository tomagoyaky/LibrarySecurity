LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/../inc
LOCAL_LDLIBS 	:= -llog -fPIC
LOCAL_MODULE    := RootService
LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/../inc \
	$(LOCAL_PATH)/../libSandBox
	
LOCAL_STATIC_LIBRARIES := libSandBox
LOCAL_SRC_FILES := Main.cpp
include $(BUILD_EXECUTABLE)