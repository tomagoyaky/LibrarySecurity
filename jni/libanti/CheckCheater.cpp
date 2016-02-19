/*
 * CheckCheater.cpp
 *
 *  Created on: 2016年2月15日
 *      Author: peng
 */

#include "CheckCheater.h"
#include "ANTIPARAMS.h"
#include "processinfo.h"
#include "hook.h"

using namespace PS;

void *CHEAT::start(void* _antiParams){
	int ret = 0;
	ANTIPARAMS::PointAntiParams antiParams;
	do{
		antiParams = (ANTIPARAMS::PointAntiParams)_antiParams;
		while(true){
			if((ret = MonitorHook()) < 0){
				LOGE("set hook point fail, error:%d", ret);
				break;
			}
			if(MonitorCheater(antiParams) < 0){
				break;
			}
			if(ANTIPARAMS::ResultProcess(antiParams) < 0){
				break;
			}

			if(antiParams->sleepTime == NoSleep_Break){
				break;
			}else{
				sleep(antiParams->sleepTime);
			}
		}
	}while(0);
	return NULL;
}

int (*org_access)(const char *pathname, int mode);
int CHEAT::monitor_access(const char *pathname, int mode){

	LOGI(":::: pathname='%s', mode=%d", pathname, mode);
	return org_access(pathname, mode);
}
static int error_num = 0;
int CHEAT::MonitorHook(){

	int ret = 0;
	do {
		if((ret = elfHook("libc.so", "access", (void *) monitor_access, (void**) &org_access)) != 0){
			ret = -1 * (error_num++);
			break;
		}
	} while (0);
	return ret;
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
					const char *_blacklist[] = {
						"/data/data/com.touch18.tools/files/memcheck",
						"com.touch18.tools",
						"com.touch18.tools:systemservice",
						"/data/data/cn.mm.gk/files/isq",
						"cn.mm.gk"
					};
					blacklist = (char**)_blacklist;
					blacklistnum = sizeof(_blacklist) / sizeof(char*);
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
			LOGD(">>> OK, not found cheater");
		}else{
			LOGD("Warnning!!! found cheater number of '%d'", flag_found);
		}
	
		time_t t = time(0);
		char tmp[64];
		strftime( tmp, sizeof(tmp), "%Y/%m/%d %H %X %A 本年第%j天 %z",localtime(&t) );
		LOGD(">>> %s", tmp);

	} while (0);
	
//	return UNHOOK::undo(antiParams->libNamePath, antiParams->func);
	return 0;
}

