/*
 * load.h
 *
 *      Author: peng
 */

#ifndef JNI_LIBLOAD_LOAD_H_
#define JNI_LIBLOAD_LOAD_H_
#include <stdio.h>
#include <errno.h>
#include <android/log.h>
#define DEBUG 1

#ifndef DEBUG
	#define DEBUG 0
#endif

#ifndef LOG_TAG
	# define LOG_TAG "tomagoyaky_native"
#endif

#if DEBUG
	#define LOGD(fmt,...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "[%s]" fmt, __FUNCTION__,##__VA_ARGS__)
	#define LOGI(fmt,...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "[%s]" fmt, __FUNCTION__,##__VA_ARGS__)
	#define LOGV(fmt,...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "[%s]" fmt, __FUNCTION__,##__VA_ARGS__)
	#define LOGW(fmt,...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, "[%s]" fmt, __FUNCTION__,##__VA_ARGS__)
	#define LOGE(fmt,...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "[%s]" fmt, __FUNCTION__,##__VA_ARGS__)
	#define LOGF(fmt,...) __android_log_print(ANDROID_LOG_FATAL, LOG_TAG, "[%s]" fmt, __FUNCTION__,##__VA_ARGS__)
#else
	#define LOGD(...) while(0){}
	#define LOGI(...) while(0){}
	#define LOGV(...) while(0){}
	#define LOGW(...) while(0){}
	#define LOGE(...) while(0){}
	#define LOGW(...) while(0){}
#endif

/*
 * If "very verbose" logging is enabled, make it equivalent to LOGV.
 * Otherwise, make it disappear.
 *
 * Define this above the #include "Dalvik.h" to enable for only a
 * single file.
 */
/* #define VERY_VERBOSE_LOG */
#if defined(VERY_VERBOSE_LOG)
# define LOGVV      LOGV
# define IF_LOGVV() IF_LOGV()
#else
# define LOGVV(...) ((void)0)
# define IF_LOGVV() if (false)
#endif


static inline void DebugMem(void * buf, int len, unsigned char more) {
	unsigned char * curaddr;
	int i;
	int left;
	int sector = 0;

	if (len > 0x20) {
		len = 0x30;
	}
	if (buf == NULL) {
		return;
	}
	left = len;
	char msgbuf[1024] = { 0 };
	curaddr = (unsigned char*) buf;

	while (left > 0) {

		if (left < 16) {
			int cnt = sprintf(msgbuf, "%08Xh  ", (unsigned int) curaddr);

			for (i = 0; i < left; i++) {
				cnt += sprintf(msgbuf + cnt, "%02X ", *(curaddr + i));
			}

			for (i = left; i < 16; i++)
				cnt += sprintf(msgbuf + cnt, "   ");

			for (i = 0; i < left; i++) {
				if ((*(curaddr + i) >= '!') && (*(curaddr + i) <= '~'))
					cnt += sprintf(msgbuf + cnt, "%c", *(curaddr + i));
				else
					cnt += sprintf(msgbuf + cnt, ".");
			}
			left -= 16;
			LOGV("%s", msgbuf);
		} else {

			int cnt = sprintf(msgbuf, "%08Xh  ", (unsigned int) curaddr);

			for (i = 0; i < 16; i++) {
				cnt += sprintf(msgbuf + cnt, "%02X ", *(curaddr + i));
			}

			for (i = 0; i < 16; i++) {
				if ((*(curaddr + i) >= '!') && (*(curaddr + i) <= '~'))
					cnt += sprintf(msgbuf + cnt, "%c", *(curaddr + i));
				else
					cnt += sprintf(msgbuf + cnt, ".");
			}
			LOGV("%s", msgbuf);
		}

		left -= 16;
		sector += 16;
		curaddr += 16;
	}
}

#endif /* JNI_LIBLOAD_LOAD_H_ */
