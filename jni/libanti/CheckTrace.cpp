/*
 * CheckTrace.cpp
 *
 *  Created on: 2016年1月29日
 *      Author: peng
 */
#include "CheckTrace.h"

void *TRACE::start(void* _antiParams){
	ANTIPARAMS::PointAntiParams antiParams = (ANTIPARAMS::PointAntiParams)_antiParams;
	do{
		while(true){
			if(CheckTraceID(antiParams->pid) > 0){
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
}

int TRACE::CheckIsGDBAttach(int pid){

	return 0;
}

/**
 * 检测pid进程是否被ptrace附加
 * 是则返回追踪者的pid,否返回0,失败返回-1
 * */
long TRACE::CheckTraceID(int pid){

	const char *fieldName = "TracerPid";
	int fieldLength = strlen(fieldName);
	FILE *fd_status = NULL;
	char mapPath[NAME_MAX] = {0};
	char buf[PATH_MAX] = {0};
	long tracerPid = 0;

	do{
		snprintf(mapPath, NAME_MAX, "/proc/%d/status", pid);
		fd_status = fopen(buf, "r");
		if (fd_status == NULL) {
			return -1;
		}
		while (fgets(buf, sizeof(buf), fd_status)){
			if (strncmp(buf, fieldName, fieldLength) == 0 && buf[fieldLength] == ':') {
				tracerPid = strtol(&buf[fieldLength + 1], NULL, 10);
				break;
			}
		}

		return tracerPid;
	}while(0);
	return 0;
}




