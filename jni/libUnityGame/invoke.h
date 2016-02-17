/*
 * invode_utils.h
 *
 *  Created on: Oct 16, 2014
 *      Author: jevey
 */

#ifndef INVOKE_H_
#define INVOKE_H_
#include <jni.h>
#include "jni_common.h"

#define INLINE_ALWAYS __attribute__ ((unused, always_inline))

typedef struct {
	 bool (*getObject)(JNIEnv* env, jobject *result, const char *className,
			jobject obj, const char *filedName, const char *filedSig);
	 bool (*getStaticObject)(JNIEnv* env, jobject *result,
			const char *className, const char *filedName, const char *filedSig);
	 bool (*getStaticInt)(JNIEnv* env, jint *result, const char *className,
			const char *filedName) ;
	 bool (*getInt)(JNIEnv* env, jint *result, const char *className,jobject obj,
			const char *filedName) ;
	 bool (*getLong)(JNIEnv* env, jlong *result, const char *className,jobject obj,
			const char *filedName) ;
	 bool (*setObject)(JNIEnv* env, const char *className, const char *filedName,
			const char *filedSig, jobject jobj, jobject jfiled_vaule);
	 bool (*setBoolean)(JNIEnv* env, const char *className, const char *filedName, jobject jobj, jboolean jfiled_vaule);
	 bool (*setInt)(JNIEnv* env, const char *className, const char *filedName, jobject jobj, jint jfiled_vaule);
	 bool (*setLong)(JNIEnv* env, const char *className, const char *filedName, jobject jobj, jlong jfiled_vaule);
	 bool (*setStaticObject)(JNIEnv* env, const char *className,
			const char *filedName, const char *filedSig, jobject jfiled_vaule);
	 bool (*callStaticIntMethod)(JNIEnv* env, jint *result, const char *className,
			const char *methodSig, const char *methodName, ...);
	 bool (*callStaticBooleanMethod)(JNIEnv* env, jboolean *result, const char *className,
				const char *methodSig, const char *methodName, ...);
	 bool (*callStaticVoidMothod)(JNIEnv* env, const char *className,
			const char *methodSig, const char *methodName, ...);
	 bool (*callStaticObjectMethod)(JNIEnv* env, jobject *result,
			const char *className, const char *methodSig, const char *methodName,
			...);
	 bool (*callObjectMethod)(JNIEnv* env, jobject *result, const char *className,
			jobject jobj, const char *methodSig, const char *methodName, ...);
	 bool (*callIntMethod)(JNIEnv* env, jint *result, const char *className,
				jobject jobj, const char *methodSig, const char *methodName, ...);
	 bool (*callBooleanMethod)(JNIEnv* env, jboolean *result, const char *className,
			jobject jobj, const char *methodSig, const char *methodName, ...);
	 bool (*callVoidMethod)(JNIEnv* env, const char *className, jobject jobj,
			const char *methodSig, const char *methodName, ...);
	 jobject (*newInstance)(JNIEnv* env, const char *className, const char *methodSig,
			...);
	 jstring (*tojstring)(JNIEnv* env, const char* pStr);
	 char *(*tocstr)(JNIEnv* env, jstring jstr);
}Invoke;

Invoke* invoke_func();

#endif /* INVODE_UTILS_H_ */
