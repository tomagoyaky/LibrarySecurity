/*
 * ThreadUtil.cpp
 *
 *  Created on: 2016年2月17日
 *      Author: peng
 */

#include "ThreadUtil.h"


int ThreadUtil::makethread(void *(*fn)(void *), void *arg) {
	int err;
	pthread_t tid;
	pthread_attr_t attr;

	err = pthread_attr_init(&attr);
	if (err != 0)
		return err;
	err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (err == 0) {
		err = pthread_create(&tid, &attr, fn, arg);
	}

	pthread_attr_destroy(&attr);
	return err;

}



