/*
 * ThreadUtil.h
 *
 *  Created on: 2016年2月17日
 *      Author: peng
 */

#ifndef THREADUTIL_H_
#define THREADUTIL_H_

#include <pthread.h>

namespace ThreadUtil{

	int makethread(void *(*fn)(void *), void *arg);
}

#endif /* THREADUTIL_H_ */
