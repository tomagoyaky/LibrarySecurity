LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_ARM_MODE	:= thumb
LOCAL_LDLIBS	:= -llog \
	-fPIC \
#	 -landroid_runtime -lutils -lcutils -lart -ldvm
NDK_PATCH_BASEDIR := ndk-patch-master
NDK_PATCH_INCLUDES := $(NDK_PATCH_BASEDIR)/include
NDK_PATCH_LIBS := $(NDK_PATCH_BASEDIR)/lib
#=============================================
include $(CLEAR_VARS)
LOCAL_MODULE := android_runtime
LOCAL_SRC_FILES := $(NDK_PATCH_LIBS)/libandroid_runtime.so
include $(PREBUILT_SHARED_LIBRARY)
#=============================================
include $(CLEAR_VARS)
LOCAL_MODULE := utils
LOCAL_SRC_FILES := $(NDK_PATCH_LIBS)/libutils.so
include $(PREBUILT_SHARED_LIBRARY)
#=============================================
include $(CLEAR_VARS)
LOCAL_MODULE := cutils
LOCAL_SRC_FILES := $(NDK_PATCH_LIBS)/libcutils.so
include $(PREBUILT_SHARED_LIBRARY)
#=============================================
include $(CLEAR_VARS)
LOCAL_MODULE := art
LOCAL_SRC_FILES := $(NDK_PATCH_LIBS)/libart.so
include $(PREBUILT_SHARED_LIBRARY)
#=============================================
include $(CLEAR_VARS)
LOCAL_MODULE := dvm
#LOCAL_SRC_FILES := $(NDK_PATCH_LIBS)/libdvm.2.so
LOCAL_SRC_FILES := $(NDK_PATCH_LIBS)/libdvm.4.so
include $(PREBUILT_SHARED_LIBRARY)
#=============================================
LOCAL_CFLAGS	:= -std=gnu++11 -fpermissive -DDEBUG -O0
LOCAL_MODULE    := hook
LOCAL_C_INCLUDES :=  \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/../inc \
	$(LOCAL_PATH)/$(NDK_PATCH_INCLUDES)
ElfHook_SRC_FILES := \
	 ElfHook/elfhook.cpp \
	 ElfHook/elfio.cpp \
	 ElfHook/elfutils.cpp
	
JavaHook_SRC_FILES := \
	 JavaHook/DalvikMethodHook.cpp \
	 JavaHook/ArtMethodHook.cpp
	 
ifeq "$(TARGET_ARCH)" "arm"
	JavaHook_SRC_FILES := JavaHook/art_quick_proxy.S
else ifeq "$(TARGET_ARCH)" "x86"
	JavaHook_SRC_FILES := JavaHook/art_quick_proxy.asm
endif
	
LOCAL_SRC_FILES := \
	$(ElfHook_SRC_FILES) \
	$(JavaHook_SRC_FILES)
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
	
include $(BUILD_STATIC_LIBRARY)
#include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_EXECUTABLE)
