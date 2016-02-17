/*
 * ps.cpp
 *
 *  Created on: 2016年2月17日
 *      Author: peng
 */

#include "processinfo.h"
using namespace PS;

std::list<PointProcessInfo> PS::getCurrentProcessInfoList(){

	std::list<PointProcessInfo> processInfoList;
	const char *ps_file_path = "/system/bin/ps";
	if(access(ps_file_path, X_OK) == -1){
		LOGE("not found '%s' file, could not use this API '%s'.", ps_file_path, __FUNCTION__);
		return processInfoList;
	}

	char command[] = "ps";
	char buffer[256] = {0};
	FILE *fp = popen(command, "r");

	while(fgets(buffer, sizeof(buffer), fp) != NULL){
		if(strncmp(buffer, "USER ", 5) == 0){
			continue;
		}

		PointProcessInfo processInfo = (PointProcessInfo)malloc(sizeof(struct ProcessInfo));
		const char *fmt = "%s      %s     %s     %s\t%s\t%s\t%s\t%s\t%s";
		sscanf(buffer, fmt, processInfo->USER,
				processInfo->PID,
				processInfo->PPID,
				processInfo->VSIZE,
				processInfo->RSS,
				processInfo->WCHAN,
				processInfo->PC,
				processInfo->STATE,
				processInfo->NAME);

		processInfoList.push_back(processInfo);
	}
	return processInfoList;
}

void PS::printCurrentProcesses(std::list<PointProcessInfo> processInfoList){

	std::list<PointProcessInfo>::iterator iterator;
	int i = 0;
	for (iterator = processInfoList.begin(); iterator != processInfoList.end(); iterator++) {
		i++;
		PointProcessInfo processInfo = *iterator;

		LOGD("[%02d]%s,%s,%s,%s,%s,%s,%s,%s,%s", i,
				processInfo->USER,
				processInfo->PID,
				processInfo->PPID,
				processInfo->VSIZE,
				processInfo->RSS,
				processInfo->WCHAN,
				processInfo->PC,
				processInfo->STATE,
				processInfo->NAME);
	}
}

int PS::isRuning_pid(std::list<PointProcessInfo> processInfoList, char *pid, char **status){

	int flag = 0;
	std::list<PointProcessInfo>::iterator iterator;
	for (iterator = processInfoList.begin(); iterator != processInfoList.end(); iterator++) {
		PointProcessInfo processInfo = *iterator;

		if(strncmp(pid, processInfo->PID, strlen(processInfo->PID)) == 0){
			flag = 1;
			strcpy(*status, processInfo->STATE);
			break;
		}
	}
	return flag;
}

int PS::isRuning_name(std::list<PointProcessInfo> processInfoList, char *name, char **status){

	int flag = 0;
	std::list<PointProcessInfo>::iterator iterator;
	for (iterator = processInfoList.begin(); iterator != processInfoList.end(); iterator++) {
		PointProcessInfo processInfo = *iterator;

		if(strncmp(name, processInfo->NAME, strlen(processInfo->NAME)) == 0){
			flag = 1;
			strcpy(*status, processInfo->STATE);
			break;
		}
	}
	return flag;
}
