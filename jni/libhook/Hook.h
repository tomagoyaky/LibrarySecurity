/*
 * hook.h
 *
 *  Created on: 2016年2月20日
 *      Author: peng
 */

#ifndef HOOK_H_
#define HOOK_H_

#include "ElfHook/elfhook.h"
#include "JavaHook/JavaMethodHook.h"
using namespace ElfHook;
using namespace JavaHook;

namespace Hook{

	extern int elfHook(const char *soname, const char *symbol, void *replace_func, void **old_func);
	extern int elfHookDirect(unsigned int addr, void *replace_func,void **old_func);

	extern int dalvikHook(JNIEnv*, HookInfo *);
	extern int artHook(JNIEnv*, HookInfo*);
}
#endif /* HOOK_H_ */
