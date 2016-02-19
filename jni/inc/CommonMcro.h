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

/*
用法:extern Dwarf_Die *dwarf_offdie (Dwarf *dbg, Dwarf_Off offset, Dwarf_Die *result) __nonnull_attribute__ (3);
 * */
#if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3)
# define __nonnull_attribute__(...) __attribute__ ((__nonnull__ (__VA_ARGS__)))
# define __deprecated_attribute__ __attribute__ ((__deprecated__))
#else
# define __nonnull_attribute__(args...)
# define __deprecated_attribute__
#endif


#endif /* COMMONMCRO_H_ */
