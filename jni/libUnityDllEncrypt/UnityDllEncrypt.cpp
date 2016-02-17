/*
 * UnityDllEncrypt.cpp
 *
 *      Author: peng
 */
#include <stdlib.h>
#include "MonoImageOpenStatus.h"
#include "../inc/hook.h"
#include "../inc/log.h"

static void decMap(char *str, int length) {
	int i;
	int l, c;
	for (i = 0; i < length; i++) {
		l = (str[i] & 0xF0) >> 0x04;
		c = str[i] & 0x0F;
		str[i] = invsBox[l * 16 + c] & 0X00FF;
	}
}

void *(*orig_mono_image_open)(char *data, unsigned int data_len, int need_copy,
		MonoImageOpenStatus *status, int refonly,const char *name);

void *my_mono_image_open_from_data_full(char *data, unsigned int data_len,
		int need_copy, MonoImageOpenStatus *status, int refonly, const char *name){

	if(name != NULL){
		char *tmp = strstr(name,".dll");
		if (tmp != NULL && strcmp(tmp,".dll")== 0){
			if(data[0] != 'M' && data[1] != 'Z'){
				decMap(data, data_len);
			}
		}
	}

	void *res = orig_mono_image_open(data, data_len, need_copy, status, refonly, name);
	return res;
}

static void my_init(void){
	elfHook("libmono.so", "mono_image_open_from_data_with_name", (void*)my_mono_image_open_from_data_full, (void **)&orig_mono_image_open);
}
