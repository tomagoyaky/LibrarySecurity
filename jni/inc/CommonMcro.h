/*
 * CommonMcro.h
 *
 *  Created on: 2016年1月20日
 *      Author: peng
 */

#ifndef COMMONMCRO_H_
#define COMMONMCRO_H_

#include <stdlib.h>

#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#define NAME(obj)	#obj
// 关闭文件描述符
#define CLOSEFP(fp){ \
	if(fp){ \
		fclose(fp); \
	} \
}
// free掉内存
#define FREE(mem){ \
	if(mem){ \
		free(mem); \
	} \
}

#define ISNULL(obj) ((obj) == NULL: true : false)
#endif /* COMMONMCRO_H_ */
