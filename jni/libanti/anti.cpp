/*
 * anti.cpp
 *
 *  Created on: 2016年1月29日
 *      Author: peng
 */

#include "anti.h"
#include "ThreadUtil.h"

using namespace ANTIPARAMS;

int ANTI::start(int pid){

	int retval = 0;
	PointAntiParams antiParams;
	do{
		if((antiParams = (PointAntiParams)malloc(sizeof(struct AntiParams))) == NULL){
			retval = -1;
			break;
		}
		// 反调试
		antiParams->name = "ANTI-TRACE-DEBUG";
		antiParams->pid = pid;
		antiParams->sleepTime = 30;
		antiParams->tactics = Flag_Exit;
		if((ThreadUtil::makethread(&TRACE::start, antiParams) != 0)){
			retval = -2;
			break;
		}
		LOGD(">>> %s. ", antiParams->name);

		// 反内存修改
		memset(antiParams, 0, sizeof(struct AntiParams));
		antiParams->name = "ANTI-MEMORY-MODIFY";
		antiParams->pid = pid;
		antiParams->sleepTime = 30;
		antiParams->tactics = Flag_Exit;
		if((ThreadUtil::makethread(&MEMORY::start, antiParams) != 0)){
			retval = -3;
			break;
		}
		LOGD(">>> %s. ", antiParams->name);

		// 反作弊器
		memset(antiParams, 0, sizeof(struct AntiParams));
		antiParams->name = "ANTI-CHEAT-TOOL";
		antiParams->pid = pid;
		antiParams->sleepTime = NoSleep_Break;
		antiParams->tactics = Flag_Exit;
		if((ThreadUtil::makethread(&CHEAT::start, antiParams) != 0)){
			retval = -4;
			break;
		}
		LOGD(">>> %s. ", antiParams->name);

	}while(0);
	if(antiParams){
		free(antiParams);
	}
	return retval;
}


