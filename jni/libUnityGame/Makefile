LOCAL_ARM_MODE := arm

all:
	ndk-build V=1  NDK_APPLICATION_MK=Application.mk   NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=Android.mk  

install:
	adb push libs/armeabi/libhook.so  /data/local/tmp/

clean:
	rm -rf libs/
	rm -rf obj/
#	rm *~
