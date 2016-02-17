#ifndef CRYPTO_H_
#define CRYPTO_H_

#include "3des.h"

#define DES3_KEY_LEN 24
//#define DES3_KEY "LOVEIJMTEST.."
static const char DES3_KEY[]=  {0x20,0x01,0x02,0x08,0x88,0x10,0x79,0x58,0x26,0x32,0x52,0x50,0x11,0x00};

#define CRYPTO_MODE_DEFAULT 1

typedef struct String{
		char *c_str;
		unsigned int size;
}String;

typedef struct Crypto {
	int (*encrypt)(const unsigned char *In, unsigned char *Out,
			unsigned long datalen, const  char *Key, char bInsertType,int encryptMode);
	int (*decrypt)(const unsigned char *In, unsigned char *Out,
			unsigned long datalen, const char *Key,  int nKeyLen, char bInsertType,int encryptMode);
	//void (*getKey)(const String key, unsigned char Des3key[DES3_KEY_LEN],int encryptMode);

}Crypto;

#endif
