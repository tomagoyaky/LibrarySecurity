/*
 * Security.cpp
 *
 *  Created on: 2016年2月16日
 *      Author: peng
 */

#include <jni.h>
#include <stdlib.h>
#include <unistd.h>
#include "log.h"
#include "Security.h"
#include "anti.h"

void init() __attribute ((constructor));
void init(){
//	HookInit();
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	LOGD("JNI_OnLoad");
	JNIEnv* env = NULL;
	jint result = -1;

	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		LOGE("ERROR: env is null");
		return result;
	}
//	if((result = NativeBridge::registerNatives(env)) != 0){
//		LOGE("ERROR: native methods register fail.");
//		return result;
//	}
	if((result = ANTI::start(getpid())) != 0){
		LOGE("ERROR: anti is failure, ERRORNUM=%d", result);
	}

	return JNI_VERSION_1_4;
}



