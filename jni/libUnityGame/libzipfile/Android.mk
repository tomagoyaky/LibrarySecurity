LOCAL_PATH:= $(call my-dir)

 
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	centraldir.c \
	zipfile.c

LOCAL_STATIC_LIBRARIES := \
	libunz

LOCAL_CFLAGS :=-fvisibility=hidden  -O3

LOCAL_MODULE:= libzipfile

LOCAL_C_INCLUDES += 

#LOCAL_LDLIBS := -llog   -lz   

include $(BUILD_STATIC_LIBRARY)
