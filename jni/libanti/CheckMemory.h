/*
 * CheckMemory.h
 *
 *  Created on: 2016年1月29日
 *      Author: peng
 */

#include <sys/inotify.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <ctype.h>
#include "../inc/log.h"
#include "AntiParams.h"

#ifndef LIBANTI_CHECKMEMORY_H_
#define LIBANTI_CHECKMEMORY_H_

namespace MEMORY{

	typedef struct InotifyParams{
		int inotify_fd;

	}*PointInotifyParams;

	void *start(void* _antiParams);
	int MonitorMemory(int pid);
	int RegisterInotify(PointInotifyParams *inotifyParams);
}

#endif /* LIBANTI_CHECKMEMORY_H_ */
