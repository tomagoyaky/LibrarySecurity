/*
 * ApkUtils.h
 *
 *  Created on: Aug 8, 2013
 *      Author: jevey
 */

#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "jni_common.h"
#include "apkutils.h"
#include "libzipfile/zipfile.h"

#define DEX_ENC_FILE "assets/ijiami.dat"
#define METHOD_CODE_FILE "assets/ijiami.ajm"
#include <pthread.h>
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static inline bool getZipEntry(const char *pApkPath, const char * zipentry,
		unsigned char **ppdata, size_t *unsize) {
	size_t size;

	void* scratch;
	static zipfile_t zip = NULL;
	zipentry_t entry;
	struct stat statbuff;
	void* buf = NULL;
	//LOGD("get entry: %s",zipentry);
	pthread_mutex_lock(&mutex);

	if (!zip) {
		int fd = -1;
		fd = open(pApkPath, O_RDONLY);
		//pthread_mutex_lock(&mutex);
		if (fd > 0) {
			if (stat(pApkPath, &statbuff) == 0) {
				size = statbuff.st_size;
				buf = (unsigned char *) mmap(NULL, size, PROT_READ, MAP_SHARED,
						fd, 0);
				zip = init_zipfile(buf, size);
			}

		}

	}

	if (zip == NULL) {
		LOGE("inti_zipfile failed\n");
		pthread_mutex_unlock(&mutex);
		return false;
	}

	entry = lookup_zipentry(zip, zipentry);
	if (entry == NULL) {
		pthread_mutex_unlock(&mutex);
//		LOGE("Not found %s\n", zipentry);
		return false;
	}

	*unsize = get_zipentry_size(entry);
	size = *unsize * 1.001;
	*ppdata = (unsigned char *) malloc(size);
	if (*ppdata == NULL) {
		pthread_mutex_unlock(&mutex);
		LOGE("low memory%s\n", zipentry);
		return false;
	}
	//printf("scratch=%p\n", *ppdata);
	int err = decompress_zipentry(entry, *ppdata, size);
	if (err != 0) {
		LOGE("error decompressing file\n");
		pthread_mutex_unlock(&mutex);
		return false;
	}

	pthread_mutex_unlock(&mutex);

//	munmap(buf, statbuff.st_size);
//	zip = NULL;
	return true;

}

static bool getEncryptDexData(const char *pApkPath, unsigned char **ppdata,
		size_t *unsize) {
	return getZipEntry(pApkPath, DEX_ENC_FILE, ppdata, unsize);
}
#if 1
static bool getDexMethodData0(const char *pApkPath, unsigned char **ppdata,
		size_t *unsize) {
	return getZipEntry(pApkPath, METHOD_CODE_FILE, ppdata, unsize);
}

static bool getDexMethodData(const char *pApkPath, unsigned char **ppdata,
		size_t *unsize) {
	bool okey = getZipEntry(pApkPath, METHOD_CODE_FILE, ppdata, unsize);

	return okey;
}

#else
static bool getDexMethodData0(const char *pApkPath, unsigned char **ppdata,
		size_t *outsize) {
	size_t unsize;
	char *pzipdata;
	LOGW("getDexMethodData!!!");
	bool okey = getZipEntry(pApkPath, METHOD_CODE_FILE, &pzipdata, &unsize);
	if (!okey) {
		return false;
	}

	LOGW("decode ok!!! , outsize: %d", outsize);

//	char *password = "010101010101010000000000";
//	okey=  aes_decode(password, strlen(password),pzipdata, unsize, *ppdata, outsize);

//	LOGW("decode %s!!! , outsize: %d", okey?"true":"false",*outsize);
	int i = 0;
	char key[] = {0x15, 0x21, 0x56, 0x89, 0x12, 0x16, 0x45, 0x52};
	for (i = 0; i < unsize; i++) {
		*ppdata[i] = key[i % 8] ^ pzipdata[i];
	}

	return true;
}
#endif
static ApkUtils apkutils_module = { getEncryptDexData, getDexMethodData,
		getZipEntry };

//REGISTER_DEF(apkutils);

