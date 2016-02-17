/*
 * aes.h
 *
 *  Created on: 2015年3月31日
 *      Author: Jevey
 */

#ifndef AES_H_
#define AES_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <openssl/pem.h>
#include <openssl/aes.h>

bool aes_encode(char *strPassword,size_t sizePassword,
		char *strData, size_t sizeData, char *strOutData, size_t *pOutSize) {
	AES_KEY aes_key;
	unsigned int i = 0;
	if (AES_set_encrypt_key(strPassword,
			sizePassword * 8, &aes_key) < 0) {
		//assert(false);
		return false;
	}
	int block_count = sizeData / AES_BLOCK_SIZE;
	int remaining = sizeData % AES_BLOCK_SIZE;
	if (remaining != 0) {
		block_count += 1;
	}
	*pOutSize  =0 ;
	for (i = 0; i < block_count; i++) {
		AES_encrypt(&strData[i * AES_BLOCK_SIZE], &strOutData[i * AES_BLOCK_SIZE], &aes_key);
		*pOutSize +=AES_BLOCK_SIZE;
	}

	if (remaining != 0) {
		unsigned char in[AES_BLOCK_SIZE];
		strncpy(in, strData + block_count * AES_BLOCK_SIZE,
				AES_BLOCK_SIZE);
		AES_encrypt((const unsigned char*) in,  &strOutData[block_count * AES_BLOCK_SIZE], &aes_key);
		*pOutSize +=AES_BLOCK_SIZE;
	}
	return true;
}

bool aes_decode(char* key,size_t key_len,
		char* strData, size_t sizeData, char *p_outbuf, int *p_outlen) {
	AES_KEY aes_key;
	unsigned int i = 0;
	if (AES_set_decrypt_key(key,
			key_len * 8, &aes_key) < 0) {
		//	assert(false);
		LOGW("aes set decrypt key failed!");
		return false;
	}

	for (i=0; i < sizeData / AES_BLOCK_SIZE; i++) {
		AES_decrypt(&strData[i*AES_BLOCK_SIZE], &p_outbuf[i * AES_BLOCK_SIZE], &aes_key);
	}
	*p_outlen = sizeData;
	return true;
}

#endif /* AES_H_ */
