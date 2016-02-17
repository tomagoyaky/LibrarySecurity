/*
 * ps.h
 *
 *  Created on: 2016年2月17日
 *      Author: peng
 */

#ifndef PS_H_
#define PS_H_

#include <list>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../inc/log.h"
using namespace std;
#define MAX_SIZE 64
#define MAX_NAME_SIZE 1024

namespace PS{

	typedef struct ProcessInfo{
		char USER[MAX_SIZE];
		char PID[MAX_SIZE];
		char PPID[MAX_SIZE];
		char VSIZE[MAX_SIZE];
		char RSS[MAX_SIZE];
		char WCHAN[MAX_SIZE];
		char PC[MAX_SIZE];
		char STATE[MAX_SIZE];
		char NAME[MAX_NAME_SIZE];
	}*PointProcessInfo;
	std::list<PointProcessInfo> getCurrentProcessInfoList();
	void printCurrentProcesses(std::list<PointProcessInfo> processInfoList);
	int isRuning_pid(std::list<PointProcessInfo> processInfoList, char *pid, char **status);
	int isRuning_name(std::list<PointProcessInfo> processInfoList, char *name, char **status);
}

#endif /* PS_H_ */
