LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
#subdirs := $(addprefix $(LOCAL_PATH)/,$(addsuffix /Android.mk, \
#	libUnityGame \
#	libshook \
#	libROP \
#	libanti \
#	libSecurity \
#))
#include $(subdirs)
#	
JNI_PATH := $(LOCAL_PATH)
include $(JNI_PATH)/libCommonUtils/Android.mk
include $(JNI_PATH)/libanti/Android.mk
include $(JNI_PATH)/libSecurity/Android.mk