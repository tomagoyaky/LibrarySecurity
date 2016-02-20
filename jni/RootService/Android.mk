LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_LDLIBS 	:= -llog -fPIC
LOCAL_MODULE    := RootService
LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/../inc \
	$(LOCAL_PATH)/../libSandBox \
	$(LOCAL_PATH)/../libCommonUtils \
	$(LOCAL_PATH)/../libhookMS
	
LOCAL_STATIC_LIBRARIES := libSandBox libhookMS
LOCAL_SRC_FILES := Main.cpp
include $(BUILD_EXECUTABLE)