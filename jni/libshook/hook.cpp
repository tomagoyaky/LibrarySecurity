#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <dlfcn.h>
#include <unistd.h>
#include "util.h"
#include "Log.hpp"

#include "CydiaSubstrate.h"
#include "hook.h"

int elfHook(const char *soname, const char *symbol, void *replace_func,
		void **old_func) {
	int ret = -1;
#if 0
	LOGW("dlopen %s", soname);
	void *handle = dlopen(soname, RTLD_NOW);

	if (!handle) {
		LOGW("open %s error, %s!", soname , dlerror());
		return ret;
	}
	void *addr = dlsym(handle, symbol);
	if (!addr) {
		LOGW("No found %s error!", symbol);
		return ret;
	}
#else
	void *addr = NULL;
	if (find_name(getpid(), symbol, soname, (unsigned long *)&addr) < 0) {
		LOGW("Not find: %s\n", symbol);
		return -1;
	}
#endif
	MSHookFunction(addr, replace_func, old_func);
	ret = 0;
//	LOGD("hook  %s %s addr:%p ret:%d!, old_func:%p", soname, symbol, addr, ret, *old_func);
	return ret;
}

int elfHookDirect(unsigned int addr, void *replace_func,
		void **old_func){
		if(addr == 0){
			LOGW("hook direct addr:%p  error!", (void*)addr);
			return -1;
		}
		MSHookFunction((void*)addr, replace_func, old_func);
//		LOGD("hook direct addr:%p  old_func:%p", (void*)addr,   *old_func);
		return 0;
}