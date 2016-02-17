/*
 * NativeBridge.cpp
 *
 *  Created on: 2016��1��23��
 *      Author: peng
 */

namespace NativeBridge{

	int RegisterNativeMethod(){

	}
	/*
	 * NativeBridge.cpp
	 *
	 *      Author: peng
	 */
	#include <jni.h>
	#include "log.h"
	#include "CommonMcro.h"

	#ifdef __cplusplus
	extern "C" {
	#endif

	static inline void get_cstr_from_jstring(JNIEnv* env, jstring jstr, char **out) {
		jboolean iscopy = JNI_TRUE;
		const char *cstr = env->GetStringUTFChars(jstr, &iscopy);
		*out = strdup(cstr);
		env->ReleaseStringUTFChars(jstr, cstr);
	}

	JNIEXPORT void JNICALL Java_com_tomagoyaky_librarysecurity_NativeBridge_SayHello(JNIEnv *env, jobject clazz){
		LOGD("native_SayHello");
	}

	JNIEXPORT jboolean JNICALL Java_com_tomagoyaky_Protector_CheckSignature_judgeNative(JNIEnv *env, jobject clazz, jstring pkgName, jstring org_signature){
		LOGD("native_judgeNative");


		return JNI_TRUE;
	}

	JNIEXPORT jint JNICALL Java_com_tomagoyaky_librarysecurity_utils_hookMethodNative(JNIEnv *env, jobject clazz, jstring cls, jstring methodname, jstring methodsig, jboolean isstatic){
	//
	//	HookInfo *info = (HookInfo *)malloc(sizeof(HookInfo));
	//
	//	get_cstr_from_jstring(env, cls, &info->classDesc);
	//	get_cstr_from_jstring(env, methodname, &info->methodName);
	//	get_cstr_from_jstring(env, methodsig, &info->methodSig);
	//
	//	info->isStaticMethod = isstatic == JNI_TRUE;
	//	return java_method_hook(env, info);
		return 0;
	}

	JNIEXPORT jstring JNICALL Java_com_tomagoyaky_Protector_CheckUSBState_getInfo(JNIEnv *env, jobject clazz){
		LOGD("native_getInfo");


		return env->NewStringUTF("getInfo");
	}

	JNIEXPORT void JNICALL Java_com_tomagoyaky_Protector_CheckJDWP_DisableJdwp(JNIEnv *env, jobject clazz){
		LOGD("native_DisableJdwp");

//		if(CheckJDWP::isHaveJDWPConnect()){
//		}

	}

	#ifdef __cplusplus
	}
	#endif
	//namespace NativeBridge{
	//
	//	#define JNI_REG_CLASS "com/tomagoyaky/librarysecurity/NativeBridge"
	//	#define NCHECK_SIGNATURE "com/tomagoyaky/Protector/CheckSignatures"
	//	#define REG(cls, table) (NativeBridge::registerNativeMethods(env, cls, table, NELEM(table)) == 0)
	//
	//	void native_judgeNative(JNIEnv* env, jobject thiz, jstring pkgName, jstring org_signature){
	//		LOGD("native_judgeNative");
	//
	//	}
	//
	//	void native_sayHello(JNIEnv* env, jobject thiz){
	//		LOGD("Hello NativeBridge.");
	//	}
	//
	//	static JNINativeMethod jni_bridge_methods_table[] = {
	//		{"SayHello", "()V", (void*) native_sayHello}
	//	};
	//
	//	static JNINativeMethod check_signature_methods_table[] = {
	//		{"judgeNative2", "(Ljava/lang/String;Ljava/lang/String;)V", (void*) native_judgeNative}
	//	};
	//
	//	static int registerNativeMethods(JNIEnv* env, const char* className, JNINativeMethod* gMethods, int numMethods) {
	//		jclass clazz;
	//		clazz = env->FindClass(className);
	//		if (clazz == NULL) {
	//			return JNI_FALSE;
	//		}
	//		if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
	//			return JNI_FALSE;
	//		}
	//
	//		return JNI_TRUE;
	//	}
	//
	//	static int registerNatives(JNIEnv* env) {
	//		if (REG(JNI_REG_CLASS, NativeBridge::jni_bridge_methods_table)
	//				&& REG(NCHECK_SIGNATURE, NativeBridge::check_signature_methods_table)) {
	//			LOGE("ERROR: native method registration failed");
	//			return JNI_ERR;
	//		}
	//		return JNI_OK;
	//	}
	//}
}


