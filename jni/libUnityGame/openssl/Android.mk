

LOCAL_PATH:= $(call my-dir)
 

include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
		openssl.c
		
LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/../ \
	$(LOCAL_PATH)/../libcutils/include \
		$(LOCAL_PATH)/../openssl/include \
	$(JNI_H_INCLUDE) \
	external/zlib \
	$(LOCAL_PATH)/../safe-iop/include
	
LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS += -fexceptions -fvisibility=hidden   -Wno-write-strings  

LOCAL_MODULE := libopenssl

#LOCAL_LDLIBS := -llog   -lz   -fPIC 


include $(BUILD_STATIC_LIBRARY)

