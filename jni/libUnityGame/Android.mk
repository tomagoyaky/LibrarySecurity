LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)   
LOCAL_MODULE    := static  
ifeq "$(TARGET_ARCH)" "arm"
  ifeq "$(TARGET_ARCH_VARIANT)" "armv7-a"
    LOCAL_SRC_FILES := hooklib/armeabi-v7a/libthook.a
  else
    LOCAL_SRC_FILES := hooklib/armeabi/libthook.a
  endif
else ifeq "$(TARGET_ARCH)" "x86"
	LOCAL_SRC_FILES := hooklib/x86/libthook.a
endif
include $(PREBUILT_STATIC_LIBRARY)  
#======================================================================
include $(CLEAR_VARS)

LOCAL_C_INCLUDES :=	$(LOCAL_PATH)/openssl\
	$(LOCAL_PATH)/openssl/include\
	$(LOCAL_PATH)/crypto
	
#LOCAL_MODULE    := execdllgame
LOCAL_MODULE    := execdllgame

#LOCAL_LDFLAGS := libthook.a
LOCAL_STATIC_LIBRARIES := static libzipfile libz_static     libopenssl
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog -lc -lm -landroid
#LOCAL_LDLIBS := -llog  -lz  -fPIC  -Wl,-init=ijiami 

LOCAL_CFLAGS +=   -fvisibility=hidden #-Wstatic-in-inline
cmd-strip = $(TOOLCHAIN_PREFIX)strip --strip-debug -x $1

LOCAL_ARM_MODE = arm
CRYPTO_FILES:= crypto/3des.c \
			   crypto/md5.c \
			   crypto/crypto.c \
			   crypto/signer.c

LOCAL_SRC_FILES := myGetData.c gameDefend.c mapinfo.c \
				invoke.c ReflectionJni.c sm4.c apkutils.c \
				$(CRYPTO_FILES)


include $(BUILD_SHARED_LIBRARY)

subdirs := $(addprefix $(LOCAL_PATH)/,$(addsuffix /Android.mk, \
		zlib \
		libzipfile \
		openssl \
	))

include $(subdirs)

