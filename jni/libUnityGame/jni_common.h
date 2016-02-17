#ifndef JNI_COMMON_H
#define JNI_COMMON_H

#include <android/log.h>

#define DEBUG 0

#ifndef LOG_TAG
#define LOG_TAG "ajm_Hello"
#endif

#if DEBUG
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG ,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG ,__VA_ARGS__)
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG ,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG ,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG ,__VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,LOG_TAG ,__VA_ARGS__)
#else
#define LOGD(...) while(0){}
#define LOGI(...) while(0){}
#define LOGV(...) while(0){}
#define LOGW(...) while(0){}
#define LOGE(...) while(0){}
#define LOGW(...) while(0){}
#endif

#define bool unsigned char
#define false 0
#define true 1


static void DebugMem(void * buf, int len, unsigned char more)
{
	unsigned char * curaddr;
	int i;
	int left;
	int sector = 0;
	left = len;
	char msgbuf[1024];
	memset(msgbuf, 0, sizeof(msgbuf));
	curaddr = (unsigned char*)buf;

	while (left > 0)
	{

		if (left < 16)
		{
			int cnt = sprintf(msgbuf, "\n%08Xh  ", (unsigned int) curaddr);

			for (i = 0; i < left; i++)
			{
				cnt += sprintf(msgbuf + cnt, "%02X ", *(curaddr + i));
			}

			for (i = left; i < 16; i++)
			cnt += sprintf(msgbuf + cnt, "   ");

			for (i = 0; i < left; i++)
			{
				if ((*(curaddr + i) >= '!') && (*(curaddr + i) <= '~'))
				cnt += sprintf(msgbuf + cnt, "%c", *(curaddr + i));
				else
				cnt += sprintf(msgbuf + cnt, ".");
			}

			left -= 16;

			LOGD("%s", msgbuf);
		}
		else
		{

			int cnt = sprintf(msgbuf, "\n%08Xh  ", (unsigned int) curaddr);

			for (i = 0; i < 16; i++)
			{
				cnt += sprintf(msgbuf + cnt, "%02X ", *(curaddr + i));
			}

			for (i = 0; i < 16; i++)
			{
				if ((*(curaddr + i) >= '!') && (*(curaddr + i) <= '~'))
				cnt += sprintf(msgbuf + cnt, "%c", *(curaddr + i));
				else
				cnt += sprintf(msgbuf + cnt, ".");
			}

			LOGD("%s", msgbuf);

		}

		left -= 16;

		sector += 16;
		curaddr += 16;

		if ((sector % 512) == 0)
		{
			LOGD(" \n");
		}
	}

	if (more)
	LOGD("  . . . . . . ");

	LOGD("\n");
}


#endif
