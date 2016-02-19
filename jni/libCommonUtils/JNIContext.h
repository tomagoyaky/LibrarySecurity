/*
 * JNIContext.h
 *
 *  Created on: 2016年2月19日
 *      Author: peng
 */

#ifndef JNICONTEXT_H_
#define JNICONTEXT_H_

#include "jni.h"
#include <stdlib.h>
#include "log.h"

JNIEnv* GetJNIEnv(JavaVM* vm) {
	JNIEnv* env;
	vm->AttachCurrentThread(&env, NULL);
	return env;
}

bool checkException(JNIEnv* env) {
	if (env->ExceptionCheck() != 0) {
		LOGE("*** Uncaught exception returned from Java call!\n");
		env->ExceptionDescribe();
		return true;
	}
	return false;
}

void DetachFromVM(JavaVM* vm) {
	vm->DetachCurrentThread();
}

#endif /* JNICONTEXT_H_ */
