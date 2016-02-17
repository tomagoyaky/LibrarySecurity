/*
 * CheckTrace.h
 *
 *  Created on: 2016年1月29日
 *      Author: peng
 */

#include <linux/limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "AntiParams.h"

#ifndef LIBANTI_CHECKTRACE_H_
#define LIBANTI_CHECKTRACE_H_

namespace TRACE{

	void *start(void* _antiParams);
	long CheckTraceID(int pid);
	int CheckIsGDBAttach(int pid);
}

#endif /* LIBANTI_CHECKTRACE_H_ */
