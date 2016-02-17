/*
 * dlopenssl.h
 *
 *  Created on: Dec 30, 2013
 *      Author: jevey
 */

#ifndef DLOPENSSL_H_
#define DLOPENSSL_H_
#include <dlfcn.h>

#include <openssl/pem.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#include "../jni_common.h"

#define LibraryPath "/system/lib/libcrypto.so"
#define null NULL

#define func(X) do{\
	X=(_#X)dlsym(handle,#X);\
	}while(0)
#define CHK(X) if((X)==NULL)break;


typedef BIO *(*F_BIO_new_mem_buf)(void *buf, int len);
typedef int (*F_BIO_free)(BIO *bio);
typedef RSA *(*F_PEM_read_bio_RSAPublicKey)(BIO *bp, RSA **x,
                                       pem_password_cb *cb, void *u);

typedef BIGNUM *(*F_BN_new)(void);
typedef void (*F_BN_free)(BIGNUM *a);
typedef int (*F_BN_set_bit)(BIGNUM *a, int n);
typedef BIGNUM *(*F_BN_bin2bn)(const unsigned char *s, int len, BIGNUM *ret);
typedef int (*F_BN_bn2bin)(const BIGNUM *a, unsigned char *to);

typedef RSA *(*F_RSA_new)(void);
typedef void (*F_RSA_free)(RSA *r);
typedef int (*F_RSA_generate_key_ex)(RSA *rsa, int bits, BIGNUM *e_value,
		BN_GENCB *cb); 
typedef int (*F_RSA_size)(const RSA *r);
typedef void (*F_AES_encrypt)(const unsigned char *in, unsigned char *out,
		const AES_KEY *key);
typedef void (*F_AES_decrypt)(const unsigned char *in, unsigned char *out,
		const AES_KEY *key);
typedef int (*F_AES_set_encrypt_key)(const unsigned char *userKey,
		const int bits, AES_KEY *key);
typedef int (*F_AES_set_decrypt_key)(const unsigned char *userKey,
		const int bits, AES_KEY *key);

typedef EVP_PKEY *(*F_EVP_PKEY_new)(void);
typedef int (*F_EVP_PKEY_set1_RSA)(EVP_PKEY *pkey, RSA *key);
typedef void (*F_EVP_MD_CTX_init)(EVP_MD_CTX *ctx);
typedef const EVP_MD *(*F_EVP_md5)(void);
typedef int (*F_EVP_DigestInit_ex)(EVP_MD_CTX *ctx, const EVP_MD *type,
		ENGINE *impl);
typedef int (*F_EVP_SignFinal)(EVP_MD_CTX *ctx, unsigned char *md,
		unsigned int *s, EVP_PKEY *pkey);
typedef int (*F_EVP_DigestUpdate)(EVP_MD_CTX *ctx, const void *data,
		size_t count);
typedef int (*F_EVP_VerifyFinal)(EVP_MD_CTX *ctx, const unsigned char *sigbuf,
		unsigned int siglen, EVP_PKEY *pkey);


typedef RSA *(*F_RSA_private_key_from_bytes)(const uint8_t *in, size_t in_len);

#endif /* DLOPENSSL_H_ */
