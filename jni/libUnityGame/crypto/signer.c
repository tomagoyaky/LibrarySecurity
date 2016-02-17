/*
 * sign.h
 *
 *      Author: Jevey
 */

#include <pthread.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <openssl/des.h>
#include <openssl/rand.h>
#include "../jni_common.h"

#include "aes.h"
#include "strtok_r.h"
#include "md5.c"
#include "signer.h"
#include "../apkutils.c"

#define DEBUG_FILE 0

// #define  SIGNEDFILES_DATA "META-INF/af.bin"  //签名文件列表
// #define  ENCRYPTO_DATA "META-INF/sdata.bin"  //公钥PUBKEY，AES加密后数捿
// #define  SIGNED_DATA "META-INF/signed.bin"	 //签名数据
#define  SIGNEDFILES_DATA "assets/af.bin"  //签名文件列表
#define  ENCRYPTO_DATA "assets/sdata.bin"  //公钥PUBKEY，AES加密后数捿
#define  SIGNED_DATA "assets/signed.bin"	 //签名数据

#define  RESULT_CHECK(b) do{if(!b)return false;}while(0)

static EVP_MD_CTX mMdContext; //摘要算法上下文变釿
static EVP_PKEY* pEvpKey; //EVP KEY结构体变釿
static RSA* pRSA;

inline static bool VerifyInit(unsigned char *rsaPublicKey, size_t size);
inline static bool VerifyUpdate(char *buffer, size_t len);
inline static bool VerifyFinal(char *signVaule, size_t size);

bool VerifyInit(unsigned char *rsaPublicKey, size_t size) {
	
	LOGW("\n 正在初始化签名证乿...\n");
	//LOGW("rsaPublicKey:%s", rsaPublicKey);
	BIO *bio = NULL;  
	bio = BIO_new_mem_buf((void*)rsaPublicKey, -1);
	pRSA = PEM_read_bio_RSAPublicKey(bio,NULL,NULL,NULL);
	BIO_free(bio);

	pEvpKey = EVP_PKEY_new(); //新建丿个EVP_PKEY变量
	if (pEvpKey == NULL) {
		return false;
	}
	LOGW("\n 正在设置密钥信息...\n");

	if (EVP_PKEY_set1_RSA(pEvpKey, pRSA) != 1) //保存RSA结构体到EVP_PKEY结构使
			{
		LOGE("EVP_PKEY_set1_RSA error\n");
		return false;
	}

	LOGW("\n 正在初始化摘要上下文...\n");

	//以下是验证签名的代码
	EVP_MD_CTX_init(&mMdContext); //初始化摘要上下文
	if (!EVP_VerifyInit_ex(&mMdContext, EVP_md5(), NULL)) //验证初始化，设置摘要算法，一定要和签名一臿
			{
		LOGE("EVP_VerifyInit_ex error\n");
		return false;
	}
	return true;
}
#pragma clang obfuscate on
bool VerifyUpdate(char *buffer, size_t len) {
	if (!EVP_VerifyUpdate(&mMdContext, buffer, len)) //验证签名（摘要）Update
			{
		LOGE("VerifyUpdate error\n");
		return false;
	}
	return true;

}
#pragma clang obfuscate off
bool VerifyFinal(char *signVaule, size_t size) {
	if (!EVP_VerifyFinal(&mMdContext, signVaule, size, pEvpKey)) {
		LOGE("VerifyFinal error\n");
		return false;
	} else {
		LOGW("验证签名正确.\n");
	}
	return true;
}
#define IS_SPACE(c)  ((c == ' ') || (c == '\t') || (c =='\n') || (c =='\r'))

char *strtrim(char *s) {

	char *Tail, *Head;
	if (!s) {
		return NULL;
	}

	for (Tail = s + strlen(s) - 1; Tail >= s; Tail--)
		if (!IS_SPACE(*Tail))
			break;
	Tail[1] = 0;
	for (Head = s; Head <= Tail; Head++)
		if (!IS_SPACE(*Head))
			break;
	if (Head != s)
		memcpy(s, Head, (Tail - Head + 2) * sizeof(char));
	return s;

}

static bool signer_verify(char * packagename,char * apkpath) {

	LOGD("signer_verify apkpath:%s",apkpath);
	LOGD("signer_verify pkg_name:%s",packagename);

	bool bIsSucc = false;

	char *strPublicKey[1024] = { 0 };
	size_t sizePublicKey = 0;

	char *pSignedFilesBuf = NULL;
	size_t nSignedFilesBufSize = 0;

	MD5 md5;
	char key[33] = { 0 };

	char *password = packagename;
	
	/*
	 * tmp for only special key
	*/

	char szBuf[]= {
        0x03,0x44,0x46,0x17,0x7f,0x4f,0xbb,0xcc,0xf1,0x45,0x70,0x64,0x19,0xf5,
        0x23,0x54,0xc6,0x77,0x9f,0xaf,0xcb,0x0c,0x1f,0x85,0x79,0xc4,0x79,0x75,0x00};
    password = szBuf;
	//key = md5(md5(packagename))
	MD5_Calculate(password, strlen(password), key);
	password = strdup(key);
	MD5_Calculate(password, strlen(password), key);
	free(password);

	LOGW("aes key : %s", key);
	/*
	 * 获取签名文件列表mSignedFilesList
	 */
	bIsSucc = false;

	do {
		LOGW("--------Get signed files------------");
		size_t nEncDataSize;
		unsigned char *pEncData;
		LOGW("apkpath: %s", apkpath);
		if (!getZipEntry(apkpath, SIGNEDFILES_DATA, &pEncData,
				&nEncDataSize)) {
			LOGE("signed file read error!");
			break;
		}
			//LOGD("签名列表＿");
			//DebugMem(pEncData,nEncDataSize,0);
		LOGW("enc data: %p, size: %d", pEncData, nEncDataSize);
		char *mCiphertextSignedFilesStr;
		pSignedFilesBuf = malloc(nEncDataSize);

		if (!aes_decode(key, strlen(key), pEncData, nEncDataSize,
				pSignedFilesBuf, &nSignedFilesBufSize)) {
			LOGE("signed file decode error!");
			free(pEncData);
			break;
		}

		free(pEncData);

#if DEBUG_FILE
		FILE* fp = fopen("/data/local/tmp/sign.txt", "w+");
		if (fp) {
			LOGE("write /data/local/tmp/sign.txt !");
			fwrite(pSignedFilesBuf, 1, strlen(pSignedFilesBuf), fp);
			fclose(fp);
		} else {
			LOGE("open /data/local/tmp/sign.txt error!");
		}
#endif
		bIsSucc = true;
	} while (0);

	RESULT_CHECK(bIsSucc);
	bIsSucc = false;
	/*
	 * 获取公钥strPublicKey
	 */
	do {
		LOGW("--------Get public key------------");
		unsigned char *pEncData;
		unsigned int nEncDataSize;

		if (!getZipEntry(apkpath, ENCRYPTO_DATA, &pEncData,
				&nEncDataSize)) {
			LOGE("read encrypto data error!");
			break;
		}
			//LOGD("密钥＿");
			//DebugMem(pEncData,nEncDataSize,0);
		if (!aes_decode(key, strlen(key), pEncData, nEncDataSize, strPublicKey,
				&sizePublicKey)) {
			LOGE("decode pulbic key  error!");
			break;
		}

		free(pEncData);
		//LOGD("公钥＿");
		//DebugMem((unsigned char *)strPublicKey,strlen(strPublicKey),0);
		bIsSucc = true;

	} while (0);

	RESULT_CHECK(bIsSucc);
	bIsSucc = false;
	/*
	 * 签名验证
	 */
	do {
		LOGW("--------verifying-----------");

		unsigned char *pData;
		size_t nDataLen;
		size_t i = 0;
		if(!VerifyInit(strPublicKey, sizePublicKey)){
			LOGE("VerifyInit error!");
			break;
		}
		char *filename = NULL;
		char *delim = "\n";
		char *safep = NULL;
		char *pbuf = pSignedFilesBuf;

		while ((filename = strtok_r(pbuf, delim, &safep)) != NULL) {
			filename=strtrim(filename);
			if (filename[0] != '\0') {
				pData = NULL;
				if (getZipEntry(apkpath, filename, &pData,
						&nDataLen)) {
					//LOGE("get zip entry: %s", filename);
					if (pData == NULL || nDataLen == 0) {
						LOGW("-------pEntryData is null-----------");
						continue;
					}

#if DEBUG_FILE
					do {
						static FILE *fp = NULL;
						if (!fp) {
							fp = fopen("/data/local/tmp/sign_debug.dat", "w+");
						}
						if (fp) {
							fwrite(filename, 1, strlen(filename), fp);
							fwrite("\n", 1, 1, fp);
							//fwrite(&nDataLen, 1, sizeof(size_t), fp);
							//fwrite(pData, 1, nDataLen, fp);
						}
					} while (0);

#endif
					VerifyUpdate((char*) pData, nDataLen); //计算签名
					free(pData);
				}

			}

			pbuf = NULL;
		}

		//读取签名倿
		unsigned char *pSignedData;
		unsigned int nSignedLength;
		if (!getZipEntry(apkpath, SIGNED_DATA, &pSignedData,
				&nSignedLength)) {
			LOGE("read signed data error!");
			break;
		}

		if (pSignedData == NULL || nSignedLength == 0) {
			LOGE("signed data error!");
			break;
		}

//		DebugMem((unsigned char *) pSignedData, nSignedLength, 0);

		if (!VerifyFinal(pSignedData, nSignedLength)) {
			LOGE("verify error!");
			free(pSignedData);
			return false;
		}
		free(pSignedData);

		bIsSucc = true;

	} while (0);

	free(pSignedFilesBuf);
	RESULT_CHECK(bIsSucc);

	return true;
}

void* signer_verify_thread(void *data) {

	SignThreadData *std = (SignThreadData *) data;
	std->thread_num--;

	bool okey = signer_verify(std->packagename,std->apkpath);
	std->on_callback(okey);

	return NULL;
}

bool signer_verify_async(SignThreadData *data) {
#define THREAD_NUM 1
#define THREAD_COST_TIME_MS 200
	int err;
	pthread_t ntid;
	struct timeval curtime;
	long int entry_time_ms, exit_time_ms;

	gettimeofday(&curtime, 0);
	entry_time_ms = ((long) curtime.tv_sec) * 1000
			+ (long) curtime.tv_usec / 1000;

	data->thread_num = THREAD_NUM;

	err = pthread_create(&ntid, NULL, signer_verify_thread, data);
	if (err != 0) {
		LOGE("can't create thread: %s\n", strerror(err));
		return false;
	}

	do {
		gettimeofday(&curtime, 0);
		exit_time_ms = ((long) curtime.tv_sec) * 1000
				+ (long) curtime.tv_usec / 1000;
		if (exit_time_ms - entry_time_ms > THREAD_COST_TIME_MS) {
			return false;
		}
	} while (data->thread_num > 0);
	return true;
}
