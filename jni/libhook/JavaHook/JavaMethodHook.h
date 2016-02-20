/*
 * JavaMethodHook.h
 *
 *  Created on: 2014-9-18
 *      Author: boyliang
 */

#ifndef __JAVA_METHOD_HOOK__H__
#define __JAVA_METHOD_HOOK__H__

#include <jni.h>
#include <stddef.h>
#include <elf.h>

namespace JavaHook{
	struct HookInfo {
		char *classDesc;
		char *methodName;
		char *methodSig;

		// for dalvik jvm
		bool isStaticMethod;
		void *originalMethod;
		void *returnType;
		void *paramTypes;

		// for art jvm
		const void *nativecode;
		const void *entrypoint;
	};
	extern int dalvikHook(JNIEnv*, HookInfo *);
	extern int artHook(JNIEnv*, HookInfo*);
}

#endif //end of __JAVA_METHOD_HOOK__H__
