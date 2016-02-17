/*
 * anti.h
 *
 *  Created on: 2016年1月29日
 *      Author: peng
 */

#ifndef LIBANTI_ANTI_H_
#define LIBANTI_ANTI_H_

#include <pthread.h>
#include <stdlib.h>
#include <sys/cdefs.h>
#include <malloc.h>
#include "CheckTrace.h"
#include "CheckMemory.h"
#include "CheckCheater.h"
#include "AntiParams.h"
#include "../inc/log.h"

namespace ANTI{

	extern int start(int pid);
}

#endif /* LIBANTI_ANTI_H_ */
