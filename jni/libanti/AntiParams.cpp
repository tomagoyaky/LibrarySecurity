/*
 * AntiParams.cpp
 *
 *  Created on: 2016年1月29日
 *      Author: peng
 */

#include "AntiParams.h"

// 对发现的结果按照策略进行处理
int ANTIPARAMS::ResultProcess(PointAntiParams antiParams){
	switch (antiParams->tactics) {
	case Flag_Exit:
		return Flag_Exit;
		break;
	case Flag_ConsolePrint:
		return Flag_ConsolePrint;
		break;
	case Flag_FileLog:
		return Flag_FileLog;

		break;
		default:
			break;
	}
	return 0;
}

