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

#define USER_SELF_PID 1

enum CommandType {EXIT, RUNING, PTRACE, SANDBOX};
int main(int argc, char **argv) {

	int i;
	for(i = 0; i < argc; i++){
		LOGD("argv[%d]=%s", i, argv[i]);
	}
	LOGD("--------------------------------------------");
	pid_t pid;
	int cmdType = atoi(argv[2]);
#if USER_SELF_PID
	pid = getpid();
#else
	pid = atoi(argv[1]);
#endif
	switch (cmdType) {
		case PTRACE:
			do {
				LOGI(">>> Ptrace running.");
			} while (0);

			break;
		case SANDBOX:
			do {
				LOGI(">>> SandBox running.");
				SandBox::start(pid);
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

