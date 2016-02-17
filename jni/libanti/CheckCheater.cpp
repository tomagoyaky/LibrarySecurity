/*
 * CheckCheater.cpp
 *
 *  Created on: 2016年2月15日
 *      Author: peng
 */

#include "CheckCheater.h"
#include "ANTIPARAMS.h"
#include "processinfo.h"

using namespace PS;

void *CHEAT::start(void* _antiParams){
	ANTIPARAMS::PointAntiParams antiParams = (ANTIPARAMS::PointAntiParams)_antiParams;
	do{
		while(true){

			if(MonitorCheater(antiParams) < 0){
				break;
			}
			if(ANTIPARAMS::ResultProcess(antiParams) < 0){
				break;
			}

			sleep(antiParams->sleepTime);
		}
	}while(0);
	return NULL;
}

int CHEAT::MonitorCheater(ANTIPARAMS::PointAntiParams antiParams){

	std::list<PointProcessInfo> processInfoList;
	std::list<PointProcessInfo>::iterator iterator;
	int Tactics = TypeLocalVarBuff;
	char **blacklist;
	int blacklistnum;
	do {
		// 0x01 get the mapinfo
		processInfoList = getCurrentProcessInfoList();
#if 0
		printCurrentProcesses(processInfoList);
#endif
		// 0x02 how to get the blacklist
		switch(Tactics){
			case TypeLocalVarBuff:
				do {
					char *_blacklist[] = {
						"/data/data/com.touch18.tools/files/memcheck",
						"com.touch18.tools",
						"com.touch18.tools:systemservice",
						"/data/data/cn.mm.gk/files/isq",
						"cn.mm.gk"
					};
					blacklist = _blacklist;
					blacklistnum = sizeof(_blacklist);
				} while (0);
				break;
			case TypeLocalFile:

				break;
			case TypeLocalNetWork:

				break;
		}

		// 0x03 check cheat is running
		int flag_found = 0;
		for (iterator = processInfoList.begin(); iterator != processInfoList.end(); iterator++) {
			PointProcessInfo processInfo = *iterator;

			int i;
			for(i = 0; i < blacklistnum; i++){
				if(strncmp(blacklist[i], processInfo->NAME, strlen(blacklist[i])) == 0){
					LOGW("warnning!!! found cheater %s:%s", processInfo->NAME, processInfo->PID);
					flag_found++;
				}
			}
		}
		if(flag_found == 0){
			LOGI(">>> OK, not found cheater");
		}else{
			LOGI("Warnning!!! found cheater number of '%d'", flag_found);
		}
	
		time_t t = time(0);
		char tmp[64];
		strftime( tmp, sizeof(tmp), "%Y/%m/%d %H %X %A 本年第%j天 %z",localtime(&t) );
		LOGD(">>> %s", tmp);

	} while (0);
	
//	return UNHOOK::undo(antiParams->libNamePath, antiParams->func);
	return 0;
}

