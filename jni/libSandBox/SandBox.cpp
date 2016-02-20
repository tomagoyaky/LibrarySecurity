/*
 * SandBox.cpp
 *
 *  Created on: 2016年2月19日
 *      Author: peng
 */

#include "SandBox.h"

#include <sys/ptrace.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "ptraceaux.h"
#include "ThreadUtil.h"
#include "log.h"

int SandBox::start(pid_t pid){

	LOGI("pid: %d", pid);
	return 0;
}
