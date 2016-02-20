/*
 * Main.cpp
 *
 *  Created on: 2016年2月20日
 *      Author: peng
 */

#include "log.h"
#include "SandBox.h"
#include <unistd.h>
#include <stdarg.h>
#include <memory.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "hook.h"
#include "processinfo.h"

using namespace PS;

#define ddll LOGD("%d", __LINE__);
#define MySelf 0
enum CommandType {EXIT, RUNING, PTRACE, SANDBOX, HOOK};
const char *default_pkg = "com.tomagoyaky.librarysecurity";

int (*old_access)(const char *pathname, int mode);
int my_access(const char *pathname, int mode);
void parseSecondParams(char **argv, pid_t *pid, char *pkg);

int main(int argc, char **argv) {

	int i;
	for(i = 0; i < argc; i++){
		LOGD("argv[%d]=%s", i, argv[i]);
	}
	LOGD("--------------------------------------------");

	int cmdType = atoi(argv[1]);
	switch (cmdType) {
		case PTRACE:
			do {
				LOGI(">>> Ptrace running.");
				pid_t pid = MySelf ? pid = getpid() : pid = atoi(argv[1]);
			} while (0);

			break;
		case SANDBOX:
			do {
				LOGI(">>> SandBox running.");
				pid_t pid = MySelf ? pid = getpid() : pid = atoi(argv[1]);
				SandBox::start(pid);
			} while (0);
			break;
		case HOOK:
			do {
				LOGI(">>> Hook running.");
				pid_t pid = -1;
				char pkg[NAME_MAX] = {0};
				parseSecondParams(argv, &pid, pkg);

				char soName[NAME_MAX] = {0};
				char symbolName[NAME_MAX] = {0};

				LOGD("pid:%d", pid);
				LOGD("pkg:%s", pkg);
				char *symbolStr = argv[3];
				char *token = strtok(symbolStr, ",");
				while (token != NULL) {
					memset(soName, NAME_MAX, 0);
					memset(symbolName, NAME_MAX, 0);
					sscanf(symbolStr, "%s=%s", soName, symbolName);

					// hook
					LOGD("symbolStr:%s", symbolStr);
					LOGD("soName:%s", soName);
					LOGD("symbolName:%s", symbolName);
					LOGD("++++++++++++++++++++++++++++++++++++++");
					int ret = elfHook(soName, symbolName, (void *) my_access, (void**) &old_access);
					if (0 != ret) {
						LOGD("hook access");
					}else{
						LOGD("hook error");
					}

					token = strtok(NULL, ",");
				}
			} while (0);

			break;
		case RUNING:
			LOGI(">>> running.");
			break;
		default:
			LOGI(">>> [-]illegal command!.");
			break;
	}
	return 0;
}

void parseSecondParams(char **argv, pid_t *pid, char *pkg){

	char keyName[NAME_MAX] = {0};
	char value[NAME_MAX] = {0};
	sscanf(argv[2], "%s=%s", keyName, value);
	LOGD(">>> %s=%s", keyName, value);

	if(keyName == NULL | value == NULL){
		LOGE("second params format is not right.");
		return;
	}

	if(strncmp(keyName, "pid", 3)){
		*pid = MySelf ? getpid() : atoi(value);
		LOGI("pid:%s", value);
	}

	if(strncmp(keyName, "pkg", 3)){
		const char *temp_pkg = MySelf ? default_pkg : value;

		std::list<PointProcessInfo> processInfoList = getCurrentProcessInfoList();
		std::list<PointProcessInfo>::iterator iterator;
		for (iterator = processInfoList.begin(); iterator != processInfoList.end(); iterator++) {
			PointProcessInfo processInfo = *iterator;

			if(strncmp(temp_pkg, processInfo->NAME, strlen(temp_pkg)) == 0){
				*pid = atoi(processInfo->PID);
				LOGI("pid:%s", processInfo->PID);
			}
		}
		strncpy(pkg, temp_pkg, strlen(temp_pkg));
	}
}

int my_access(const char *pathname, int mode) {

	LOGD(">>> %s", pathname);

	return old_access(pathname, mode);
}

