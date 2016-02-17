/*
 * anti.cpp
 *
 *  Created on: 2016年1月25日
 *      Author: peng
 */


#include <jni.h>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <sys/ptrace.h>
#include <asm/ptrace.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <ctype.h>
#include <libgen.h>

#include <pthread.h>
#include <sys/inotify.h>
#include <dirent.h>
#include <android/log.h>
#include <dlfcn.h>
#include <string.h>

#include "../inc/hook.h"
#include "mapinfo.h"
#include "../inc/log.h"
#include "gameDefend.h"

#define PTRACE_SELF_FILTER 1
#define MAX_PATH_LENGTH 256
#define PACKAGE_NAME_LENGTH 100

#define EVENT_SIZE  (sizeof (struct inotify_event))
#define BUF_LEN     (1024 * (EVENT_SIZE + 16))

static void __attribute__ ((constructor)) my_init(void);
static int linux_proc_get_int(pid_t lwpid, const char *field);
static void *checkStatusThread(void* arg);
static int makethread(void *(*fn)(void *), void *arg);
static void allWay_exit(const char* where);
static void *memMonitorThread(void* arg);
static void helloXX(void);
static int timeCheckInit(void);
static int checkRTCTime(void);
static void dbg_trap(int signo);
static void updateWatch(void);
static void checkOpen(void);
static int getProcName(char* procName);
static void* checkPtrace(void* arg);
static int check_miui_version(void);

static void *
timeCheckThread(void *arg);

static mapinfo *milist;
static int time_check_t = 0;
static int time_check_c = 0;
static int gMemfd = -1;

struct timeval start_time;
static int reset_timer = 0;

static char gProName[PACKAGE_NAME_LENGTH];


static volatile sig_atomic_t being_debugged = 0;
static void int3_handler(int signo) {
	being_debugged = 1;
}

void collocateDefend(void) {

	//ptrace(0, 0, 0, 0);
	makethread(&checkStatusThread, NULL);
	makethread(&timeCheckThread, NULL);
	makethread(&memMonitorThread, NULL);
	//signal(SIGCONT, dbg_trap);
	signal(SIGTRAP, int3_handler);
	makethread(&checkPtrace, NULL);
	//helloXX();
	//checkOpen();

}

static void* checkPtrace(void* arg) {
	int inotifyFd, wd;
	char buf[4096] __attribute__ ((aligned(8)));
	ssize_t numRead;
	char *p;
	struct inotify_event *event;

	inotifyFd = inotify_init(); /* Create inotify instance */
	if (inotifyFd == -1)
		LOGE("inotify_init");

	wd = inotify_add_watch(inotifyFd, "/dev/null", IN_DELETE);
	if (wd == -1)
		LOGE("inotify_add_watch");

	int isMIUI = check_miui_version();

	while (1) { /* Read events forever */
		LOGD("ptrace Thread:%d", gettid());
		LOGD("wait -- ptrace--");
		numRead = read(inotifyFd, buf, sizeof(buf));
		if (numRead == -1 && errno == EINTR) {
			LOGE("read");
			if (isMIUI) {
				continue;
			}
			allWay_exit("ptraced");
		}
	}
}

static int getProcName(char* procName) {
	FILE * pFile = NULL;
	char szPath[MAX_PATH_LENGTH], szData[MAX_PATH_LENGTH];
	sprintf(szPath, "/proc/%d/cmdline", getpid());
	pFile = fopen(szPath, "rb");

	if (!pFile) {
		LOGD("open %s failed", szPath);
		return -1;
	}

	memset(szData, 0, sizeof(szData));
	fgets(szData, sizeof(szData) - 1, pFile);
	char *p = strchr(szData, ':');
	if (NULL != p) {
		LOGD("MutiThread :%s", p);
		memset(p, '\0', 1);
	}

	szData[MAX_PATH_LENGTH - 1] = '\0';
	LOGD("proc name:%s", szData);

	memcpy(procName, szData, strlen(szData));
	fclose(pFile);

	return 0;
}

void *(*old_dlopen)(const char *filename, int flag);
static void *my_dlopen(const char *filename, int flag) {
	LOGD("----hook in dlopen------%s", filename);
	return old_dlopen(filename, flag);
}

FILE *(*old_fopen)(const char *path, const char *mode);
static FILE *my_fopen(const char *path, const char *mode) {
	LOGD("----hook in fopen------%s", path);
	return old_fopen(path, mode);
}

int (*old_open)(const char *pathname, int flags, mode_t mode);

static int my_open(const char *pathname, int flags, mode_t mode) {
	LOGD("----hook in open------%s", pathname);
	if (NULL != pathname) {
		if (strstr(pathname, "/data/data/") != NULL) //for old version huluxia
				{
			char packageName[PACKAGE_NAME_LENGTH] = { 0 };
			strncpy(packageName, pathname + strlen("/data/data/"),
					strlen(gProName));
			packageName[PACKAGE_NAME_LENGTH - 1] = '\0';

			LOGD("get /data/data/%s!", packageName);

			if (strcmp(packageName, gProName) != 0) {
				return -1;
			}
		}
	}
	return old_open(pathname, flags, mode);
}

int (*old_access)(const char *pathname, int mode);

static int my_access(const char *pathname, int mode) {

	//LOGD("my_access---%s", pathname);

	if (NULL != pathname) {
		if (strstr(pathname, "/data/data/") != NULL) //for old version huluxia
				{
			if (strstr(pathname, "/data/data/com.huluxia") != NULL) {
				return -1;
			}
			/*char packageName[PACKAGE_NAME_LENGTH] = { 0 };
			 strncpy(packageName, pathname + strlen("/data/data/"),
			 strlen(gProName));
			 packageName[PACKAGE_NAME_LENGTH - 1] = '\0';

			 LOGD("get /data/data/%s!", packageName);

			 if (strcmp(packageName, gProName) != 0) {
			 return -1;
			 }*/
		}
	}

	return old_access(pathname, mode);
}

/*
 * check paths which cheater open
 */
static void checkOpen(void) {
	if (0 != getProcName(gProName)) {
		LOGD("getProcName Fail");
	}

	//elfHook("libc.so", "fopen", (void *) my_fopen, (void**) &old_fopen);
	int ret = elfHook("libc.so", "access", (void *) my_access,
			(void**) &old_access);
	if (0 != ret) {
		LOGE("access");
	}
	//elfHook("libc.so", "open", (void*) my_open, (void*) &old_open); //fail on vivo Y33, kongjia k25
}

/*
 * check gdb etc debug in time
 * bug: Coolpad devices will trigger
 * reason: no found
 */
static void dbg_trap(int signo) {
	allWay_exit("traped");
}

static int (*ori_do_speed_hook)();
static int xx_do_speed_hook() {

	LOGD("-----hook in xx_do_speed_hook-----");
	return 0;
}

static int (*ori_MSHookFunction)(int result, int a2, int a3);
static int xx_MSHookFunction(int result, int a2, int a3) {
	LOGD("-----hook in xx_MSHookFunction-------");
	return 0;
}

static int (*ori_XXJavaHookClassLoad)(int a1, int a2, int a3, int a4);
static int xx_XXJavaHookClassLoad(int a1, int a2, int a3, int a4) {
	LOGD("-----hook in XXJavaHookClassLoad-------");
	return 0;
}

static int (*ori_OOQueryInterface)(int a1, int *a2, int a3, int a4);
static int oo_OOQueryInterface(int a1, int *a2, int a3, int a4) {
	LOGD("-----hook in oo_OOQueryInterface-------");
	return 0;
}

static int (*ori_gettimeofday)(struct timeval *tv, struct timezone *tz);
static int my_gettimeofday(struct timeval *tv, struct timezone *tz) {
	//LOGD("-----hook in gettimeofday-------");
	time_check_t = time_check_t + 1;
	return ori_gettimeofday(tv, tz);

}

int (*ori_clock_gettime)(clockid_t clk_id, struct timespec *tp);

static int my_clock_gettime(clockid_t clk_id, struct timespec *tp) {
	time_check_c = time_check_c + 1;
	return ori_clock_gettime(clk_id, tp);
}

/*
 * clash with com.huang.hl
 */
static int timeCheckInit(void) {

	time_check_t = 0;
	time_check_c = 0;
	int ret = 0;

	ret = elfHook("libc.so", "gettimeofday", (void *) my_gettimeofday,
			(void**) &ori_gettimeofday);

	if (ret != 0) {
		time_check_t = -1;
	}

	ret = elfHook("libc.so", "clock_gettime", (void *) my_clock_gettime,
			(void**) &ori_clock_gettime);

	if (ret != 0) {
		time_check_c = -1;
	}

	if (time_check_t == -1 || time_check_c == -1)
		return -1;
	else
		return 0;
}

/*
 * xxAssistant
 */
static void helloXX(void) {
	milist = init_mapinfo(getpid());
	static int xxed = 0;
	char xxLibName[48];
	mapinfo *tmp = milist;

	while (tmp) {

		if (strstr(tmp->name, "libsubstrate.so") != NULL) {
			elfHook("libsubstrate.so", "MSHookFunction",
					(void *) xx_MSHookFunction, (void**) &ori_MSHookFunction);
		}

		if (strstr(tmp->name, "libxxdvm.so") != NULL) {
			elfHook("libxxdvm.so", "XXJavaHookClassLoad",
					(void *) xx_XXJavaHookClassLoad,
					(void**) &ori_XXJavaHookClassLoad);
		}

		if (strstr(tmp->name, "libxxspeedmanager.so") != NULL) {
			elfHook("libxxspeedmanager.so", "do_speed_hook",
					(void *) xx_do_speed_hook, (void**) &ori_do_speed_hook);
		}

		if (strstr(tmp->name, "libooSpeeder.so") != NULL) {

			elfHook("libooSpeeder.so", "OOQueryInterface",
					(void *) oo_OOQueryInterface,
					(void**) &ori_OOQueryInterface);
		}

		if (strstr(tmp->name, "/data/data/com.xxAssistant/app_plugin") != NULL
				&& strstr(tmp->name, ".so") != NULL) {
			char *bname = basename(tmp->name);
			memset(xxLibName, 0, sizeof(xxLibName));
			memcpy(xxLibName, bname, strlen(bname));
			LOGD("maps so :%s", xxLibName);
			xxed = 1;

		}

		tmp = tmp->next;
	}

	deinit_mapinfo(milist);
	if (xxed)
		allWay_exit("xxAssistant");
}

/*
 * function : monitor proc mem file
 *
 * aim:     : main to defend game cheater
 *
 * effect   : org.sbtools.gamehack, com.huluxia.gametools, com.huang.hl,
 * 			 com.touch18.tools will be checked, when search proc mem
 *
 * bug      : no found
 *
 */
static void *
memMonitorThread(void* arg) {
	//sleep(15);
	//helloXX();//for map test;

	int fd, len = 0;
	char *ptr;
	static char buf[BUF_LEN] __attribute__ ((aligned(__alignof__(struct inotify_event))));
	//struct inotify_event *event;

	fd = inotify_init();
	gMemfd = fd;
	if (fd < 0) {
		LOGD("error inotify_init");
	}

	char path[48];
	DIR *d;
	struct dirent *de;
	memset(path, 0, sizeof(path));
	sprintf(path, "/proc/%d/task", getpid());
	d = opendir(path);
	if (d == 0)
		LOGD("Failed to open /proc (%s)\n", strerror(errno));

	while ((de = readdir(d)) != 0) {
		if (isdigit(de->d_name[0])) {
			int tid = atoi(de->d_name);
			int wd;
			memset(path, 0, sizeof(path));
			sprintf(path, "/proc/%d/mem", tid); //com.huang.hl
			if (-1 != access(path, F_OK)) {
				wd = inotify_add_watch(gMemfd, path, IN_ACCESS);
			}
			memset(path, 0, sizeof(path));
			sprintf(path, "/proc/self/task/%d/mem", tid); //com.huluxia.gametools
			if (-1 != access(path, F_OK)) {
				wd = inotify_add_watch(gMemfd, path, IN_ALL_EVENTS);
			}
		}
	}
	closedir(d);

	static int memEv = 0;
	while (1) {
		LOGD("mem Thread:%d", gettid());
		memset(buf, 0, sizeof(buf));
		int i = 0;
		//LOGD("start block read----");
		len = read(gMemfd, buf, sizeof(buf));
		buf[BUF_LEN - 1] = '\0';
		if (len <= 0) { //mi4 screen off google breakpad
			LOGD("read() from inotify fd returned");
			LOGD("%s", strerror(errno));
			//break;
			sleep(3);
			continue;
		}

		/* Loop over all events in the buffer */

		while (i < len) {
			static struct inotify_event *event;
			event = (struct inotify_event *) &buf[i];

			LOGW("wd=%d mask=%u cookie=%u len=%u\n", event->wd, event->mask,
					event->cookie, event->len);

			if (event->mask & IN_ACCESS) {
				memEv = 1;
				break; //out for loop
			}

			i += EVENT_SIZE + event->len;
		}

		if (memEv == 1) {
			memEv = 0;
			break; //out while loop
		}

	}
	//inotify_rm_watch(fd, wd);
	close(fd);
	allWay_exit("mem monitor!");

	return NULL;
}

/*
 * function : proc traced status check
 *
 * aim:     : easy to defend beginner
 *
 * effect   : cn.mm.gk, gdb or ida will be checked, when main process
 *
 * bug      : due to cost, ignore threads; refer to memMonitorThread
 * 			 few app do ptrace work,need filter macro PTRACE_SELF_FILTER set
 *
 */

static void *
checkStatusThread(void* arg) {

	//for exp update
	unsigned int count = 0;
	unsigned int exp_count = 0;

	pid_t tracerPid;

	while (1) {
		LOGD("status Thread:%d", gettid());

		if (being_debugged) {
			being_debugged = 0;
			allWay_exit("trapped");
		}

		tracerPid = linux_proc_get_int(getpid(), "TracerPid");
		if (tracerPid != 0) {
			//proc be traced
			LOGD("PID %d traced by %d", getpid(), tracerPid);
#ifdef PTRACE_SELF_FILTER
			if (tracerPid != getppid()) {
				allWay_exit("checkStatus");
			}
#else
			allWay_exit("checkStatus");
#endif
		}

		sleep(3);

		if (count == (0x01u << exp_count)) {
			updateWatch();
			exp_count++;
		}

		if (exp_count < 0x05u) {
			count++;
		} else {
			count = 0;
			exp_count = 0;
		}

	}
	return NULL;
}

/*
 * function : time API check
 *
 * aim:     : check whether time API hooked
 *
 * effect   : speed function for cheater from hook time API
 *
 * bug      : need hook success to support, hook OK need to test
 *
 */

static void *timeCheckThread(void *arg) {

	checkOpen(); //check old huluxia verison
	helloXX(); // check xx

	int hookTimeOk = 0;
	if (0 != timeCheckInit()) {
		LOGD("time init error!"); //MX5 morefun game
		//return NULL;
	} else {
		hookTimeOk = 1;
	}

	while (1) {
		LOGD("time Thread:%d", gettid());
		time_check_t = 0;
		struct timeval now_time;
		gettimeofday(&now_time, 0);
		//LOGD("time_check_t = %d", time_check_t);

		time_check_c = 0;
		struct timespec now;
		clock_gettime(CLOCK_MONOTONIC, &now);
		//LOGD("time_check_c = %d", time_check_c);

		if ((time_check_t == 0 || time_check_c == 0) && (hookTimeOk != 0)) {
			allWay_exit("timespeed");
		}

		sleep(2);
		if (checkRTCTime() > 1) {
			allWay_exit("timespeed 2");
		}
		//sleep(50);
		//updateWatch();

	}

	return NULL;
}

static int checkRTCTime(void) {
	FILE *rtc_file;
	char buf[20] = { 0 };
	long rtctime0, rtctime1;

	struct timeval tv;
	struct timespec ts;

	time_t nowtime_tv0, nowtime_tv1;
	time_t nowtime_ts0, nowtime_ts1;

	struct tm *nowtm;
	char tmbuf[64] = { 0 };

	rtc_file = fopen("/sys/class/rtc/rtc0/since_epoch", "r");
	if (rtc_file == NULL) {
		LOGD("unable to open rtc file0!");
		return -1;
	}
	gettimeofday(&tv, NULL);
	fgets(buf, sizeof(buf), rtc_file);
	rtctime0 = atol(buf);
	//LOGD("RTC time0: %ld", rtctime0);
	fclose(rtc_file);
	nowtime_tv0 = tv.tv_sec;
	//LOGD("tv  time0:%ld", nowtime_tv0);

	clock_gettime(CLOCK_REALTIME, &ts);
	nowtime_ts0 = ts.tv_sec;
	//LOGD("ts  time0:%ld", nowtime_ts0);

	sleep(2);

	rtc_file = fopen("/sys/class/rtc/rtc0/since_epoch", "r");
	if (rtc_file == NULL) {
		LOGD("unable to open rtc file1!");
		return -1;
	}
	gettimeofday(&tv, NULL);
	fgets(buf, sizeof(buf), rtc_file);
	rtctime1 = atol(buf);
	//LOGD("RTC time1: %ld", rtctime1);
	fclose(rtc_file);

	nowtime_tv1 = tv.tv_sec;
	//LOGD("tv  time1 :%ld", nowtime_tv1);

	clock_gettime(CLOCK_REALTIME, &ts);
	nowtime_ts1 = ts.tv_sec;
	//LOGD("ts  time0:%ld", nowtime_ts1);

	LOGD("tv:  %ld", nowtime_tv1 - nowtime_tv0);
	LOGD("ts:  %ld", nowtime_ts1 - nowtime_ts0);
	LOGD("rtc: %ld", rtctime1 - rtctime0);

	int ret = abs(abs(nowtime_tv1 - nowtime_tv0) - abs(rtctime1 - rtctime0));

	return ret;
}

/* Return the TGID of LWPID from /proc/pid/status.  Returns -1 if not
 found.  */
static int linux_proc_get_int(pid_t lwpid, const char *field) {
	size_t field_len = strlen(field);
	FILE *status_file;
	char buf[100];
	int retval = -1;

	snprintf(buf, sizeof(buf), "/proc/%d/status", (int) lwpid);
	status_file = fopen(buf, "r");
	if (status_file == NULL) {
		LOGD("unable to open /proc file '%s'", buf);
		return -1;
	}

	while (fgets(buf, sizeof(buf), status_file))
		if (strncmp(buf, field, field_len) == 0 && buf[field_len] == ':') {
			retval = strtol(&buf[field_len + 1], NULL, 10);
			break;
		}

	fclose(status_file);
	return retval;
}

static void allWay_exit(const char* where) {
	LOGE("proc exit from : %s", where);
	sleep(1);
	exit(0);
}

static int makethread(void *(*fn)(void *), void *arg) {
	int err;
	pthread_t tid;
	pthread_attr_t attr;

	err = pthread_attr_init(&attr);
	if (err != 0)
		return err;
	err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (err == 0) {
		err = pthread_create(&tid, &attr, fn, arg);
	}

	pthread_attr_destroy(&attr);
	return err;

}

static void updateWatch(void) {

	LOGD("updateWatch_ _ _");
	if (gMemfd < 0) {
		return;
	}
	char path[48];
	DIR *d;
	struct dirent *de;
	memset(path, 0, sizeof(path));
	sprintf(path, "/proc/%d/task", getpid());
	d = opendir(path);
	if (d == 0)
		LOGD("Failed to open /proc (%s)\n", strerror(errno));

	while ((de = readdir(d)) != 0) {
		if (isdigit(de->d_name[0])) {
			int tid = atoi(de->d_name);
			int wd;
			memset(path, 0, sizeof(path));
			sprintf(path, "/proc/%d/mem", tid);

			if (-1 != access(path, F_OK)) {
				wd = inotify_add_watch(gMemfd, path, IN_ACCESS);
			}
			memset(path, 0, sizeof(path));
			sprintf(path, "/proc/self/task/%d/mem", tid); //com.huluxia.gametools
			if (-1 != access(path, F_OK)) {
				wd = inotify_add_watch(gMemfd, path, IN_ALL_EVENTS);
			}
		}
	}
	closedir(d);
}

static int check_miui_version(void) {

	char propValue[92] = { 0 };
	int code = 0;
	int name = 0;
	code = __system_property_get("ro.miui.ui.version.name", propValue);
	name = __system_property_get("ro.miui.ui.version.code", propValue);

	if (code == 0 && name == 0) {
		return 0;
	}
	LOGD("MIUI");
	return 1;
}


