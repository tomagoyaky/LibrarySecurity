/*
 * ApkUtils.h
 *
 *  Created on: Aug 8, 2013
 *      Author: jevey
 */

#ifndef APKUTILS_H_
#define APKUTILS_H_
#include "jni_common.h"

#define DEX_ENC_FILE "assets/ijiami.dat"

typedef struct ApkUtils{
	bool (*getEncryptDexData)(const char *pApkPath,unsigned char **ppdata, size_t *unsize) ;
	bool (*getDexMethodData)(const char *pApkPath,unsigned char **ppdata, size_t *unsize) ;
	bool (*getZipEntry)(const char *pApkPath,const char * zipentry,unsigned char **ppdata, size_t *unsize);
}ApkUtils;

#endif /* APKUTILS_H_ */
