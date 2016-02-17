/*
 * invode_utils.c
 *
 *  Created on: Oct 16, 2014
 *      Author: jevey
 */
#include <stddef.h>
#include <string.h>
#include "invoke.h"


//#define DEBUG_STRING(s) LOGW("%s", s)
#define DEBUG_STRING(s)

static bool get_field_object(JNIEnv* env, jobject *result, const char *className,
		jobject obj, const char *filedName, const char *filedSig);
static bool get_field_static_object(JNIEnv* env, jobject *result,
		const char *className, const char *filedName, const char *filedSig);
static bool get_field_static_int(JNIEnv* env, jint *result, const char *className,
		const char *filedName);
static bool get_field_int(JNIEnv* env, jint *result, const char *className,jobject obj,
			const char *filedName) ;
static bool get_field_long(JNIEnv* env, jlong *result, const char *className,jobject obj,
			const char *filedName) ;
static bool set_field_object(JNIEnv* env, const char *className, const char *filedName,
		const char *filedSig, jobject jobj, jobject jfiled_vaule);
static bool set_field_boolean(JNIEnv* env, const char *className, const char *filedName,
		jobject jobj, jboolean jfiled_vaule);
static bool set_field_int(JNIEnv* env, const char *className, const char *filedName,
		jobject jobj, jint jfiled_vaule);
static bool set_field_long(JNIEnv* env, const char *className, const char *filedName,
		jobject jobj, jlong jfiled_vaule);
static bool set_field_static_ojbect(JNIEnv* env, const char *className,
		const char *filedName, const char *filedSig, jobject jfiled_vaule);
static bool invoke_static_int_method(JNIEnv* env, jint *result, const char *className,
		const char *methodSig, const char *methodName, ...);
static bool invoke_static_boolean_method(JNIEnv* env, jboolean *result, const char *className,
		const char *methodSig, const char *methodName, ...);
static bool invoke_static_void_method(JNIEnv* env, const char *className,
		const char *methodSig, const char *methodName, ...);
static bool invoke_static_object_method(JNIEnv* env, jobject *result,
		const char *className, const char *methodSig, const char *methodName,
		...);
static bool invoke_object_method(JNIEnv* env, jobject *result, const char *className,
		jobject jobj, const char *methodSig, const char *methodName, ...);
static bool invoke_int_method(JNIEnv* env, jint *result, const char *className,
		jobject jobj, const char *methodSig, const char *methodName, ...);
static bool invoke_boolean_method(JNIEnv* env, jboolean *result, const char *className,
		jobject jobj, const char *methodSig, const char *methodName, ...);
static bool invoke_void_method(JNIEnv* env, const char *className, jobject jobj,
		const char *methodSig, const char *methodName, ...);
static jobject new_instance(JNIEnv* env, const char *className, const char *methodSig,
		...);
static jstring get_jstring_from_cstr(JNIEnv* env, const char* pStr);
static char *get_cstr_from_jstring(JNIEnv* env, jstring jstr);

static inline int exception_check_and_clear(JNIEnv *env) {
	if ((*env)->ExceptionCheck(env)) {
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		return -1;
	}
	return 0;
}

bool get_field_object(JNIEnv* env, jobject *result, const char *className,
		jobject obj, const char *filedName, const char *filedSig) {

	DEBUG_STRING(className);
	DEBUG_STRING(filedName);
	DEBUG_STRING(filedSig);

	jclass clazz = (*env)->FindClass(env, className);
	exception_check_and_clear(env);

	if (!clazz) {
		goto err;
	}

	jfieldID fieldId = (*env)->GetFieldID(env, clazz, filedName, filedSig);
	exception_check_and_clear(env);
	if (!fieldId) {
		(*env)->DeleteLocalRef(env, clazz);
		goto err;
	}

	*result = (*env)->GetObjectField(env, obj, fieldId);
	exception_check_and_clear(env);

	(*env)->DeleteLocalRef(env, clazz);

	return true;
	err:
	LOGE("get object feild: %s->%s type:%s error！", className, filedName, filedSig);
	return false;

}

#if 0
jobject get_field_static_object(JNIEnv* env, const char *className,
		const char *filedName, const char *filedSig) {
	//return getFieldObject(env, className, NULL, filedName, filedSig);
	jclass clazz = (*env)->FindClass(env,className);
	exception_check_and_clear(env);

	if (!clazz) {
		return false;
	}
	jfieldID jfieldId_ = (*env)->GetStaticFieldID(env, clazz, filedName, filedSig);
	exception_check_and_clear(env);

	if (!jfieldId_) {
		(*env)->DeleteLocalRef(env, clazz);
		return false;
	}
	jobject resutl = (*env)->GetStaticObjectField(clazz, jfieldId_);
	exception_check_and_clear(env);

	return resutl;
}
#endif
bool get_field_static_object(JNIEnv* env, jobject *result,
		const char *className, const char *filedName, const char *filedSig) {

	DEBUG_STRING(className);
	DEBUG_STRING(filedName);
	DEBUG_STRING(filedSig);

	jclass clazz = (*env)->FindClass(env, className);
	exception_check_and_clear(env);

	if (!clazz) {
		goto err;
	}
	jfieldID jfieldId_ = (*env)->GetStaticFieldID(env, clazz, filedName,
			filedSig);
	exception_check_and_clear(env);

	if (!jfieldId_) {
		(*env)->DeleteLocalRef(env, clazz);
		goto err;
	}
	*result = (*env)->GetStaticObjectField(env, clazz, jfieldId_);
	exception_check_and_clear(env);

	return true;
	err:
	LOGE(
			"set static object feild: %s->%s type:%s error！", className, filedName, filedSig);
	return false;
}

bool get_field_static_int(JNIEnv* env, jint *result, const char *className,
		const char *filedName) {

	DEBUG_STRING(className);
	DEBUG_STRING(filedName);

	jclass clazz = (*env)->FindClass(env, className);
	exception_check_and_clear(env);

	if (!clazz) {
		goto err;
	}
	jfieldID jfieldId_ = (*env)->GetStaticFieldID(env, clazz, filedName, "I");
	exception_check_and_clear(env);

	if (!jfieldId_) {
		(*env)->DeleteLocalRef(env, clazz);
		goto err;
	}
	*result = (*env)->GetStaticIntField(env, clazz, jfieldId_);
	exception_check_and_clear(env);

	return true;
	err:
	LOGE("get static int feild: %s->%s type:%s error！", className, filedName, "I");
	return false;
}

bool get_field_int(JNIEnv* env, jint *result, const char *className, jobject jobj,
		const char *filedName) {
	DEBUG_STRING(className);
	DEBUG_STRING(filedName);

	jclass clazz = (*env)->FindClass(env, className);
	exception_check_and_clear(env);

	if (!clazz) {
		goto err;
	}
	jfieldID jfieldId_ = (*env)->GetFieldID(env, clazz, filedName, "I");
	exception_check_and_clear(env);

	if (!jfieldId_) {
		(*env)->DeleteLocalRef(env, clazz);
		goto err;
	}
	*result = (*env)->GetIntField(env, jobj, jfieldId_);
	exception_check_and_clear(env);

	return true;
	err:
	LOGE("get int feild: %s->%s type:%s error！", className, filedName, "I");
	return false;
}

bool get_field_long(JNIEnv* env, jlong *result, const char *className, jobject jobj,
		const char *filedName) {
	DEBUG_STRING(className);
	DEBUG_STRING(filedName);

	jclass clazz = (*env)->FindClass(env, className);
	exception_check_and_clear(env);

	if (!clazz) {
		goto err;
	}
	jfieldID jfieldId_ = (*env)->GetFieldID(env, clazz, filedName, "J");
	exception_check_and_clear(env);

	if (!jfieldId_) {
		(*env)->DeleteLocalRef(env, clazz);
		goto err;
	}
	*result = (*env)->GetLongField(env, jobj, jfieldId_);
	exception_check_and_clear(env);

	return true;
	err:
	LOGE("get long feild: %s->%s type:%s error！", className, filedName, "J");
	return false;
}
bool set_field_object(JNIEnv* env, const char *className, const char *filedName,
		const char *filedSig, jobject jobj, jobject jfiled_vaule) {

	DEBUG_STRING(className);
	DEBUG_STRING(filedName);
	DEBUG_STRING(filedSig);

	jclass clazz = (*env)->FindClass(env, className);
	exception_check_and_clear(env);

	if (!clazz) {
		goto err;
	}

	jfieldID jfieldId_ = (*env)->GetFieldID(env, clazz, filedName, filedSig);
	exception_check_and_clear(env);

	if (!jfieldId_) {
		(*env)->DeleteLocalRef(env, clazz);
		goto err;
	}
	(*env)->SetObjectField(env, jobj, jfieldId_, jfiled_vaule);
	exception_check_and_clear(env);

	(*env)->DeleteLocalRef(env, clazz);
	return true;
	err:
	LOGE(
			"set object feild: %s->%s type:%s error！", className, filedName, filedSig);
	return false;
}
bool set_field_boolean(JNIEnv* env, const char *className, const char *filedName,
		jobject jobj, jboolean jfiled_vaule) {
	DEBUG_STRING(className);
	DEBUG_STRING(filedName);

	char *filedSig = "Z";

	jclass clazz = (*env)->FindClass(env, className);

	exception_check_and_clear(env);

	if (!clazz) {
		goto err;
	}

	jfieldID jfieldId_ = (*env)->GetFieldID(env, clazz, filedName, filedSig);
	exception_check_and_clear(env);

	if (!jfieldId_) {
		(*env)->DeleteLocalRef(env, clazz);
		goto err;
	}
	(*env)->SetBooleanField(env, jobj, jfieldId_, jfiled_vaule);
	exception_check_and_clear(env);

	(*env)->DeleteLocalRef(env, clazz);
	return true;
	err:
	LOGE(
			"set boolean feild: %s->%s type:%s error！", className, filedName,filedSig );
	return false;
}

bool set_field_int(JNIEnv* env, const char *className, const char *filedName,
		jobject jobj, jint jfiled_vaule) {
	DEBUG_STRING(className);
	DEBUG_STRING(filedName);
	char *filedSig = "I";

	jclass clazz = (*env)->FindClass(env, className);

	exception_check_and_clear(env);

	if (!clazz) {
		goto err;
	}

	jfieldID jfieldId_ = (*env)->GetFieldID(env, clazz, filedName, filedSig);
	exception_check_and_clear(env);

	if (!jfieldId_) {
		(*env)->DeleteLocalRef(env, clazz);
		goto err;
	}
	(*env)->SetIntField(env, jobj, jfieldId_, jfiled_vaule);
	exception_check_and_clear(env);

	(*env)->DeleteLocalRef(env, clazz);
	return true;
	err:
	LOGE(
			"set int feild: %s->%s type:%s error！", className, filedName,filedSig );
	return false;
}

bool set_field_long(JNIEnv* env, const char *className, const char *filedName,
		jobject jobj, jlong jfiled_vaule) {
	DEBUG_STRING(className);
	DEBUG_STRING(filedName);
	char *filedSig = "J";
	jclass clazz = (*env)->FindClass(env, className);
	exception_check_and_clear(env);

	if (!clazz) {
		goto err;
	}

	jfieldID jfieldId_ = (*env)->GetFieldID(env, clazz, filedName, filedSig);
	exception_check_and_clear(env);

	if (!jfieldId_) {
		(*env)->DeleteLocalRef(env, clazz);
		goto err;
	}
	(*env)->SetLongField(env, jobj, jfieldId_, jfiled_vaule);
	exception_check_and_clear(env);

	(*env)->DeleteLocalRef(env, clazz);
	return true;
	err:
	LOGE(
			"set long feild: %s->%s type:%s error！", className, filedName, filedSig);
	return false;
}

bool set_field_static_ojbect(JNIEnv* env, const char *className,
		const char *filedName, const char *filedSig, jobject jfiled_vaule) {
	return set_field_object(env, className, filedName, filedSig, NULL,
			jfiled_vaule);
}

bool invoke_static_int_method(JNIEnv* env, jint *result, const char *className,
		const char *methodSig, const char *methodName, ...) {
	DEBUG_STRING(className);
	DEBUG_STRING(methodSig);

	jclass clazz = (*env)->FindClass(env, className);
	exception_check_and_clear(env);

	if (!clazz) {
		goto err;
	}
	jmethodID jmethodId_ = (*env)->GetStaticMethodID(env, clazz, methodName,
			methodSig);
	exception_check_and_clear(env);

	if (!jmethodId_) {
		(*env)->DeleteLocalRef(env, clazz);
		goto err;
	}
	va_list arg_ptr;

	va_start(arg_ptr, methodName);
	*result = (*env)->CallStaticIntMethodV(env, clazz, jmethodId_, arg_ptr);
	exception_check_and_clear(env);
	va_end(arg_ptr);
	(*env)->DeleteLocalRef(env, clazz);

	return true;
	err:
	LOGE(
			"invoke static int method: %s->%s type:%s error！", className, methodName, methodSig);
	return false;
}
bool invoke_static_boolean_method(JNIEnv* env, jboolean *result, const char *className,
		const char *methodSig, const char *methodName, ...) {
	DEBUG_STRING(className);
	DEBUG_STRING(methodSig);
	DEBUG_STRING(methodName);

	jclass clazz = (*env)->FindClass(env, className);
	exception_check_and_clear(env);

	if (!clazz) {
		goto err;
	}
	jmethodID jmethodId_ = (*env)->GetStaticMethodID(env, clazz, methodName,
			methodSig);
	exception_check_and_clear(env);

	if (!jmethodId_) {
		(*env)->DeleteLocalRef(env, clazz);
		goto err;
	}
	va_list arg_ptr;

	va_start(arg_ptr, methodName);
	*result = (*env)->CallStaticBooleanMethodV(env, clazz, jmethodId_, arg_ptr);
	exception_check_and_clear(env);
	va_end(arg_ptr);
	(*env)->DeleteLocalRef(env, clazz);

	return true;
	err:
	LOGE(
			"invoke static boolean method: %s->%s type:%s error！", className, methodName, methodSig);
	return false;
}

bool invoke_static_void_method(JNIEnv* env, const char *className,
		const char *methodSig, const char *methodName, ...) {
	DEBUG_STRING(className);
	DEBUG_STRING(methodSig);
	DEBUG_STRING(methodName);

	jclass clazz = (*env)->FindClass(env, className);
	exception_check_and_clear(env);
	if (!clazz) {
		goto err;
	}
	jmethodID jmethodId_ = (*env)->GetStaticMethodID(env, clazz, methodName,
			methodSig);
	exception_check_and_clear(env);

	if (!jmethodId_) {
		(*env)->DeleteLocalRef(env, clazz);
		goto err;
	}
	va_list arg_ptr;

	va_start(arg_ptr, methodName);
	(*env)->CallStaticVoidMethodV(env, clazz, jmethodId_, arg_ptr);
	exception_check_and_clear(env);
	va_end(arg_ptr);
	(*env)->DeleteLocalRef(env, clazz);

	return true;
	err:
	LOGE(
			"invoke static void method: %s->%s type:%s error！", className, methodName, methodSig);
	return false;
}

bool invoke_static_object_method(JNIEnv* env, jobject *result,
		const char *className, const char *methodSig, const char *methodName,
		...) {
	DEBUG_STRING(className);
	DEBUG_STRING(methodSig);
	DEBUG_STRING(methodName);

	jclass clazz = (*env)->FindClass(env, className);
	exception_check_and_clear(env);
	if (!clazz) {
		goto err;
	}
	jmethodID jmethodId_ = (*env)->GetStaticMethodID(env, clazz, methodName,
			methodSig);
	exception_check_and_clear(env);

	if (!jmethodId_) {
		(*env)->DeleteLocalRef(env, clazz);
		goto err;
	}
	va_list arg_ptr;

	va_start(arg_ptr, methodName);
	*result = (*env)->CallStaticObjectMethodV(env, clazz, jmethodId_, arg_ptr);
	exception_check_and_clear(env);
	va_end(arg_ptr);
	(*env)->DeleteLocalRef(env, clazz);

	return true;
	err:
	LOGE(
			"invoke static object method: %s->%s type:%s error！", className, methodName, methodSig);
	return false;
}

bool invoke_object_method(JNIEnv* env, jobject *result, const char *className,
		jobject jobj, const char *methodSig, const char *methodName, ...) {
	DEBUG_STRING(className);
	DEBUG_STRING(methodSig);
	DEBUG_STRING(methodName);
	//LOGD("invoke object method: %s->%s type:%s！", className, methodName, methodSig);
	jclass clazz = (*env)->FindClass(env, className);
	exception_check_and_clear(env);

	if (!clazz || !jobj) {
		goto err;
	}

	jmethodID jmethodId_ = (*env)->GetMethodID(env, clazz, methodName,
			methodSig);
	exception_check_and_clear(env);
	if (!jmethodId_) {
		(*env)->DeleteLocalRef(env, clazz);
		goto err;
	}
	va_list arg_ptr;
	va_start(arg_ptr, methodName);
	*result = (*env)->CallObjectMethodV(env, jobj, jmethodId_, arg_ptr);
	exception_check_and_clear(env);

	va_end(arg_ptr);
	(*env)->DeleteLocalRef(env, clazz);
	return true;
	err:
	LOGE(
			"invoke object method: %s->%s type:%s error！", className, methodName, methodSig);
	return false;
}

bool invoke_int_method(JNIEnv* env, jint *result, const char *className,
		jobject jobj, const char *methodSig, const char *methodName, ...) {

	DEBUG_STRING(className);
	DEBUG_STRING(methodSig);
	DEBUG_STRING(methodName);

	jclass clazz = (*env)->FindClass(env, className);
	exception_check_and_clear(env);

	if (!clazz || !jobj) {
		goto err;
	}

	jmethodID jmethodId_ = (*env)->GetMethodID(env, clazz, methodName,
			methodSig);
	exception_check_and_clear(env);
	if (!jmethodId_) {
		(*env)->DeleteLocalRef(env, clazz);
		goto err;
	}
	va_list arg_ptr;
	va_start(arg_ptr, methodName);

	*result = (*env)->CallIntMethodV(env, jobj, jmethodId_, arg_ptr);
	exception_check_and_clear(env);
	va_end(arg_ptr);
	(*env)->DeleteLocalRef(env, clazz);

	return true;
	err:
	LOGE(
			"invoke int method: %s->%s type:%s error！", className, methodName, methodSig);
	return false;
}
bool invoke_boolean_method(JNIEnv* env, jboolean *result, const char *className,
		jobject jobj, const char *methodSig, const char *methodName, ...) {
	DEBUG_STRING(className);
	DEBUG_STRING(methodSig);
	DEBUG_STRING(methodName);

	jclass clazz = (*env)->FindClass(env, className);
	exception_check_and_clear(env);

	if (!clazz || !jobj) {
		goto err;
	}

	jmethodID jmethodId_ = (*env)->GetMethodID(env, clazz, methodName,
			methodSig);
	exception_check_and_clear(env);
	if (!jmethodId_) {
		(*env)->DeleteLocalRef(env, clazz);
		goto err;
	}
	va_list arg_ptr;
	va_start(arg_ptr, methodName);

	*result = (*env)->CallBooleanMethodV(env, jobj, jmethodId_, arg_ptr);
	exception_check_and_clear(env);
	va_end(arg_ptr);
	(*env)->DeleteLocalRef(env, clazz);

	return true;
	err:
	LOGE(
			"invoke boolean method: %s->%s type:%s error！", className, methodName, methodSig);
	return false;
}

bool invoke_void_method(JNIEnv* env, const char *className, jobject jobj,
		const char *methodSig, const char *methodName, ...) {
	DEBUG_STRING(className);
	DEBUG_STRING(methodSig);
	DEBUG_STRING(methodName);

	jclass clazz = (*env)->FindClass(env, className);
	exception_check_and_clear(env);

	if (!clazz) {
		goto err;
	}
	jmethodID jmethodId_ = (*env)->GetMethodID(env, clazz, methodName,
			methodSig);
	exception_check_and_clear(env);

	if (!jmethodId_) {
		(*env)->DeleteLocalRef(env, clazz);
		goto err;
	}
	va_list arg_ptr;

	va_start(arg_ptr, methodName);
	(*env)->CallVoidMethodV(env, jobj, jmethodId_, arg_ptr);
	exception_check_and_clear(env);

	(*env)->DeleteLocalRef(env, clazz);
	va_end(arg_ptr);
	return true;
	err:
	LOGE(
			"invoke void method: %s->%s type:%s error！", className, methodName, methodSig);
	return false;
}

jobject new_instance(JNIEnv* env, const char *className, const char *methodSig,
		...) {
	DEBUG_STRING(className);
	DEBUG_STRING(methodSig);
	jclass clazz = (*env)->FindClass(env, className);
	exception_check_and_clear(env);

	const char *methodName = "<init>";
//const char *methodSig="()V";
	if (!clazz) {
		return NULL;
	}
	jmethodID jmethodId_ = (*env)->GetMethodID(env, clazz, methodName,
			methodSig);
	exception_check_and_clear(env);

	if (!jmethodId_) {
		(*env)->DeleteLocalRef(env, clazz);
		return NULL;
	}
	va_list arg_ptr;

	va_start(arg_ptr, methodSig);
	jobject jobj = (*env)->NewObjectV(env, clazz, jmethodId_, arg_ptr);

	(*env)->CallVoidMethodV(env, jobj, jmethodId_, arg_ptr);
	exception_check_and_clear(env);
	va_end(arg_ptr);
	(*env)->DeleteLocalRef(env, clazz);

	return jobj;
}
#if 0
static jstring get_jstring_from_cstr2(JNIEnv* env, const char* pStr) {
	int strLen = strlen(pStr);
	jclass jstrObj = (*env)->FindClass(env, "java/lang/String");
	jmethodID methodId = (*env)->GetMethodID(env, jstrObj, "<init>",
			"([BLjava/lang/String;)V");
	jbyteArray byteArray = (*env)->NewByteArray(env, strLen);
	jstring encode = (*env)->NewStringUTF(env, "utf-8");

	(*env)->SetByteArrayRegion(env, byteArray, 0, strLen, (jbyte*) pStr);

	jstring result = (jstring) (*env)->NewObject(jstrObj, methodId, byteArray,
			encode);
	(*env)->DeleteLocalRef(env, jstrObj);
	(*env)->DeleteLocalRef(env, byteArray);
	(*env)->DeleteLocalRef(env, encode);
	return result;
}

static char *get_cstr_from_jstring2(JNIEnv* env, jstring jstr) {
	jboolean iscopy = JNI_TRUE;
	const char *cstr = (*env)->GetStringUTFChars(env, jstr, &iscopy);
	char *p = strdup(cstr);

	(*env)->ReleaseStringUTFChars(env, jstr, cstr);
	return p;
}
#endif
#if 1
jstring get_jstring_from_cstr(JNIEnv* env, const char* pStr) {
	if (!pStr) {
		LOGW("get_jstring_from_cstr jstring is null!");
		return NULL;
	}
	int strLen = strlen(pStr);
	jclass jstrObj = (*env)->FindClass(env, "java/lang/String");
	jmethodID methodId = (*env)->GetMethodID(env, jstrObj, "<init>",
			"([BLjava/lang/String;)V");
	jbyteArray byteArray = (*env)->NewByteArray(env, strLen);
	jstring encode = (*env)->NewStringUTF(env, "utf-8");

	(*env)->SetByteArrayRegion(env, byteArray, 0, strLen, (jbyte*) pStr);

	jstring result = (jstring) (*env)->NewObject(env, jstrObj, methodId,
			byteArray, encode);
	(*env)->DeleteLocalRef(env, jstrObj);
	(*env)->DeleteLocalRef(env, byteArray);
	(*env)->DeleteLocalRef(env, encode);
	return result;
}

char *get_cstr_from_jstring(JNIEnv* env, jstring jstr) {

	jobject result;

	jclass jclass_ = (*env)->FindClass(env, "java/lang/String");
	if (!jclass_) {
		return NULL;
	}
	jmethodID jmethodId_ = (*env)->GetMethodID(env, jclass_, "getBytes",
			"()[B");
	if (!jmethodId_) {
		(*env)->DeleteLocalRef(env, jclass_);
		return NULL;
	}

	result = (*env)->CallObjectMethod(env, jstr, jmethodId_);

	jbyteArray jdata = (jbyteArray) result;
	size_t len = (*env)->GetArrayLength(env, jdata);
	jbyte* data = (*env)->GetByteArrayElements(env, jdata, JNI_FALSE);

	char *p = malloc(len + 1);
	strncpy(p, (char*)data, len);
	p[len] = '\0';

	(*env)->ReleaseByteArrayElements(env, jdata, data, JNI_FALSE);
	(*env)->DeleteLocalRef(env, jdata);
	(*env)->DeleteLocalRef(env, jclass_);
	return p;
}
#else

static char *get_cstr_from_jstring(JNIEnv* env, jstring jstr) {
	jobject result;

	jclass jclass_ = (*env)->FindClass(env,"java/lang/String");
	exception_check_and_clear(env);

	if (!jclass_) {
		return NULL;
	}
	jmethodID jmethodId_ = (*env)->GetMethodID(env,jclass_, "getBytes", "()[B");
	exception_check_and_clear(env);

	if (!jmethodId_) {
		(*env)->DeleteLocalRef(env,jclass_);
		return NULL;
	}

	result = (*env)->CallObjectMethod(env,jstr, jmethodId_);
	exception_check_and_clear(env);

	(*env)->DeleteLocalRef(env,jclass_);

	jbyteArray jdata = (jbyteArray) result;
	size_t len = (*env)->GetArrayLength(env,jdata);
	jbyte* data = (*env)->GetByteArrayElements(env,jdata, JNI_FALSE);
	const char p = strdup(data);
	(*env)->ReleaseByteArrayElements(env, jdata, data, len);
	return p;
}
#endif

static Invoke invoke_module = {
		get_field_object,
		get_field_static_object,
		get_field_static_int,
		get_field_int,
		get_field_long,
		set_field_object,
		set_field_boolean,
		set_field_int,
		set_field_long,
		set_field_static_ojbect,
		invoke_static_int_method,
		invoke_static_boolean_method,
		invoke_static_void_method,
		invoke_static_object_method,
		invoke_object_method,
		invoke_int_method,
		invoke_boolean_method,
		invoke_void_method,
		new_instance,
		get_jstring_from_cstr,
		get_cstr_from_jstring
};

Invoke* invoke_func()
{
	return &invoke_module;
}

