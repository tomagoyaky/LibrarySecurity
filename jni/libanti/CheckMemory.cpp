/*
 * CheckMemory.cpp
 *
 *  Created on: 2016年1月29日
 *      Author: peng
 */

#include "CheckMemory.h"

#define EVENT_SIZE  (sizeof (struct inotify_event))
#define BUF_LEN     (1024 * (EVENT_SIZE + 16))

void *MEMORY::start(void* _antiParams){
	ANTIPARAMS::PointAntiParams antiParams = (ANTIPARAMS::PointAntiParams)_antiParams;
	do{
		while(true){

			if(MonitorMemory(antiParams->pid) < 0){
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

int MEMORY::RegisterInotify(PointInotifyParams *inotifyParams){

	return 0;
}
int MEMORY::MonitorMemory(int pid){

	int fd, len = 0;
	char *ptr;
	static char buf[BUF_LEN] __attribute__ ((aligned(__alignof__(struct inotify_event))));
//	static char buf[BUF_LEN] = { 0 };
	//struct inotify_event *event;

	fd = inotify_init();
	if (fd < 0) {
		LOGD("error inotify_init");
	}

	LOGW("0x02 Register inotify event.");

	char path[48];
	DIR *d;
	struct dirent *de;
	memset(path, 0, sizeof(path));
	sprintf(path, "/proc/%d/task", getpid());
	LOGI("%s", path);

	d = opendir(path);
	if (d == 0){
		LOGE("Failed to open /proc (%s)\n", strerror(errno));
	}

	while ((de = readdir(d)) != 0) {
		if (isdigit(de->d_name[0])) {
			int tid = atoi(de->d_name);
			int wd;
			memset(path, 0, sizeof(path));
			sprintf(path, "/proc/%d/mem", tid);
			if (-1 != access(path, F_OK)) {
				LOGD("%s", path);
				wd = inotify_add_watch(fd, path, IN_ALL_EVENTS);
			}
			memset(path, 0, sizeof(path));
			sprintf(path, "/proc/self/task/%d/mem", tid);
			if (-1 != access(path, F_OK)) {
				LOGD("%s", path);
				wd = inotify_add_watch(fd, path, IN_ALL_EVENTS);
			}
		}
	}
	closedir(d);

	LOGW("0x03 Start scaner with block read.");
	while (1) {
		memset(buf, 0, sizeof(buf));
		int i = 0;
		len = read(fd, buf, sizeof(buf));
		buf[BUF_LEN - 1] = '\0';
		if (len <= 0) {
			LOGD("read() from inotify fd returned");
			LOGD("%s", strerror(errno));
			//break;
			sleep(3);
			continue;
		}

		while (i < len) {
			static struct inotify_event *event;
			event = (struct inotify_event *) &buf[i];

			LOGW("wd=%d mask=%u cookie=%u len=%u\n", event->wd, event->mask,
					event->cookie, event->len);

			if (event->mask & IN_OPEN) {
				//
			}
			i += EVENT_SIZE + event->len;
		}
	}
	close(fd);
	return 0;
}



