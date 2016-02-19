/*
 * CheckCheater.h
 *
 *  Created on: 2016年2月15日
 *      Author: peng
 */

#ifndef CHECKCHEATER_H_
#define CHECKCHEATER_H_

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <list>
#include <string>
#include <time.h>
#include "AntiParams.h"
#include "../inc/log.h"

using namespace std;

namespace CHEAT{

	enum TacticsType {
		TypeLocalVarBuff,
		TypeLocalFile,
		TypeLocalNetWork
	};

	int MonitorHook();
	int MonitorCheater(ANTIPARAMS::PointAntiParams antiParams);
	void *start(void* _antiParams);
	int monitor_access(const char *pathname, int mode);
}

#endif /* CHECKCHEATER_H_ */
