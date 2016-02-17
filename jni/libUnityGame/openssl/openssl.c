#include <openssl/aes.h>
#include "dlopenssl.h"

void *handle;
#if 0
bool load() {
	bool okey = false;
	handle = dlopen(LibraryPath, RTLD_NOW);

	if (handle) {
		do {

			CHK(BN_free = (F_BN_free) dlsym(handle, "BN_free"));
			CHK(BN_set_bit= (F_BN_set_bit) dlsym(handle, "BN_set_bit"));
			CHK(BN_bin2bn= (F_BN_bin2bn) dlsym(handle, "BN_bin2bn"));
			CHK(BN_bn2bin= (F_BN_bn2bin) dlsym(handle, "BN_bn2bin"));

			CHK(RSA_new= (F_RSA_new) dlsym(handle, "RSA_new"));
			CHK(RSA_free= (F_RSA_free) dlsym(handle, "RSA_free"));
			CHK(
					RSA_generate_key_ex= (F_RSA_generate_key_ex) dlsym(handle, "RSA_generate_key_ex"));
			CHK(RSA_size= (F_RSA_size) dlsym(handle, "RSA_size"));
			CHK(AES_encrypt= (F_AES_encrypt) dlsym(handle, "AES_encrypt"));
			CHK(AES_decrypt= (F_AES_decrypt) dlsym(handle, "AES_decrypt"));
			CHK(
					AES_set_encrypt_key= (F_AES_set_encrypt_key) dlsym(handle, "AES_set_encrypt_key"));
			CHK(
					AES_set_decrypt_key= (F_AES_set_decrypt_key) dlsym(handle, "AES_set_decrypt_key"));
			CHK(EVP_PKEY_new= (F_EVP_PKEY_new) dlsym(handle, "EVP_PKEY_new"));
			CHK(
					EVP_PKEY_set1_RSA= (F_EVP_PKEY_set1_RSA) dlsym(handle, "EVP_PKEY_set1_RSA"));
			CHK(
					EVP_MD_CTX_init= (F_EVP_MD_CTX_init) dlsym(handle, "EVP_MD_CTX_init"));
			CHK(EVP_md5= (F_EVP_md5) dlsym(handle, "EVP_md5"));
			CHK(
					EVP_DigestInit_ex= (F_EVP_DigestInit_ex) dlsym(handle, "EVP_DigestInit_ex"));
			CHK(
					EVP_SignFinal= (F_EVP_SignFinal) dlsym(handle, "EVP_SignFinal"));
			CHK(
					EVP_DigestUpdate= (F_EVP_DigestUpdate) dlsym(handle, "EVP_DigestUpdate"));
			CHK(
					EVP_VerifyFinal= (F_EVP_VerifyFinal) dlsym(handle, "EVP_VerifyFinal"));
			okey = true;
			LOGW("load libcrypto.so ok!");
			LOGW("BN_new %p!", BN_new);
			LOGW("BN_free %p!", BN_free);
			LOGW("BN_set_bit %p!", BN_set_bit);
			LOGW("BN_bin2bn %p!", BN_bin2bn);
			LOGW("BN_bn2bin %p!", BN_bn2bin);

			LOGW("RSA_new %p!", RSA_new);
			LOGW("RSA_free %p!", RSA_free);
			LOGW("RSA_generate_key_ex %p!", RSA_generate_key_ex);
			LOGW("RSA_size %p!", RSA_size);
			LOGW("EVP_PKEY_new %p!", EVP_PKEY_new);
			LOGW("EVP_PKEY_set1_RSA %p!", EVP_PKEY_set1_RSA);
			LOGW("EVP_MD_CTX_init %p!", EVP_MD_CTX_init);
			LOGW("EVP_md5 %p!", EVP_md5);
			LOGW("EVP_DigestInit_ex %p!", EVP_DigestInit_ex);
			LOGW("EVP_SignFinal %p!", EVP_SignFinal);
			LOGW("EVP_DigestUpdate %p!", EVP_DigestUpdate);
			LOGW("EVP_VerifyFinal %p!", EVP_VerifyFinal);

		} while (0);

	} else {

		LOGW("load libcrypto.so 失败!");
		return false;
	}

	return okey;
}
#endif
#define checkLoadLibrary() do{if(!handle) \
								handle = dlopen(LibraryPath, RTLD_NOW);\
							}while(0)

BIO *BIO_new_mem_buf(void *buf, int len){
	static F_BIO_new_mem_buf BIO_new_mem_buf_fnPtr = NULL;
	if (!BIO_new_mem_buf_fnPtr) {
		checkLoadLibrary();
		BIO_new_mem_buf_fnPtr = (F_BIO_new_mem_buf) dlsym(handle, "BIO_new_mem_buf");
	}
	return BIO_new_mem_buf_fnPtr(buf,len);
}
int BIO_free(BIO *bio){
	static F_BIO_free BIO_free_fnPtr = NULL;
	if (!BIO_free_fnPtr) {
		checkLoadLibrary();
		BIO_free_fnPtr = (F_BIO_free) dlsym(handle, "BIO_free");
	}
	return BIO_free_fnPtr(bio);
}

RSA *PEM_read_bio_RSAPublicKey(BIO *bp, RSA **x, pem_password_cb *cb, void *u)
{
	static F_PEM_read_bio_RSAPublicKey PEM_read_bio_RSAPublicKey_fnPtr = NULL;
	if (!PEM_read_bio_RSAPublicKey_fnPtr) {
		checkLoadLibrary();
		PEM_read_bio_RSAPublicKey_fnPtr = (F_PEM_read_bio_RSAPublicKey) dlsym(handle, "PEM_read_bio_RSAPublicKey");
	}
	return PEM_read_bio_RSAPublicKey_fnPtr(bp,x,cb,u);
}

BIGNUM *BN_new(void) {
	static F_BN_new BN_new_fnPtr = NULL;
	if (!BN_new_fnPtr) {
		checkLoadLibrary();
		BN_new_fnPtr = (F_BN_new) dlsym(handle, "BN_new");
	}
	return BN_new_fnPtr();;
}
void BN_free(BIGNUM *a) {
	static F_BN_free BN_free_fnPtr = NULL;
	if (!BN_free_fnPtr) {
		checkLoadLibrary();
		BN_free_fnPtr = (F_BN_free) dlsym(handle, "BN_free");
	}
	return BN_free_fnPtr(a);
}
int BN_set_bit(BIGNUM *a, int n) {
	static F_BN_set_bit BN_set_bit_fnPtr = NULL;
	if (!BN_set_bit_fnPtr) {
		checkLoadLibrary();
		BN_set_bit_fnPtr = (F_BN_set_bit) dlsym(handle, "BN_set_bit");
	}
	return BN_set_bit_fnPtr(a, n);
}

BIGNUM *BN_bin2bn(const unsigned char *s, int len, BIGNUM *ret) {
	static F_BN_bin2bn BN_bin2bn_fnPtr = NULL;

	if (!BN_bin2bn_fnPtr) {
		checkLoadLibrary();
		BN_bin2bn_fnPtr = (F_BN_bin2bn) dlsym(handle, "BN_bin2bn");
	}
	return BN_bin2bn_fnPtr(s, len, ret);
} 
int BN_bn2bin(const BIGNUM *a, unsigned char *to) {
	static F_BN_bn2bin BN_bn2bin_fnptr = NULL;

	if (!BN_bn2bin_fnptr) {
		checkLoadLibrary();
		BN_bn2bin_fnptr = (F_BN_bn2bin) dlsym(handle, "BN_bn2bin");
	}

	return BN_bn2bin_fnptr(a, to);
}

RSA *RSA_new(void) {
	static F_RSA_new RSA_new_fnPtr = NULL;
	if (!RSA_new_fnPtr) {
		checkLoadLibrary();
		RSA_new_fnPtr = (F_RSA_new) dlsym(handle, "RSA_new");
	}

	return RSA_new_fnPtr();
}
void RSA_free(RSA *r) {
	static F_RSA_free RSA_free_fnPtr = NULL;

	if (!RSA_free_fnPtr) {
		checkLoadLibrary();
		RSA_free_fnPtr = (F_RSA_free) dlsym(handle, "RSA_free");
	}

	RSA_free_fnPtr(r);

}
int RSA_generate_key_ex(RSA *rsa, int bits, BIGNUM *e_value, BN_GENCB *cb) {
	static F_RSA_generate_key_ex RSA_generate_key_ex_fnPtr = NULL;
	LOGW("RSA_generate_key_ex+ : %p",RSA_generate_key_ex_fnPtr);

	if (!RSA_generate_key_ex_fnPtr) {
		checkLoadLibrary();
		RSA_generate_key_ex_fnPtr = (F_RSA_generate_key_ex) dlsym(handle,
				"RSA_generate_key_ex");
	}
	LOGW("RSA_generate_key_ex- : %p",RSA_generate_key_ex_fnPtr);
	return RSA_generate_key_ex_fnPtr(rsa, bits, e_value, cb);
}

int RSA_size(const RSA *r) {
	static F_RSA_size RSA_size_fnPtr = NULL;
	if (!RSA_size_fnPtr) {
		checkLoadLibrary();
		RSA_size_fnPtr = (F_RSA_size) dlsym(handle, "RSA_size");
	}

	return RSA_size_fnPtr(r);
}

void AES_encrypt(const unsigned char *in, unsigned char *out,
		const AES_KEY *key) {

	static F_AES_encrypt AES_encrypt_fnPtr = NULL;
	if (!AES_encrypt_fnPtr) {
		checkLoadLibrary();
		AES_encrypt_fnPtr = (F_AES_encrypt) dlsym(handle, "AES_encrypt");
	}

	AES_encrypt_fnPtr(in, out, key);
}
void AES_decrypt(const unsigned char *in, unsigned char *out,
		const AES_KEY *key) {
	static F_AES_decrypt AES_decrypt_fnPtr = NULL;
	if (!AES_decrypt_fnPtr) {
		checkLoadLibrary();
		AES_decrypt_fnPtr = (F_AES_decrypt) dlsym(handle, "AES_decrypt");
	}

	AES_decrypt_fnPtr(in, out, key);

}

int AES_set_encrypt_key(const unsigned char *userKey, const int bits,
		AES_KEY *key) {
	static F_AES_set_encrypt_key AES_set_encrypt_key_fnPtr = NULL;
	if (!AES_set_encrypt_key_fnPtr) {
		checkLoadLibrary();
		AES_set_encrypt_key_fnPtr = (F_AES_set_encrypt_key) dlsym(handle,
				"AES_set_encrypt_key");
	}

	return AES_set_encrypt_key_fnPtr(userKey, bits, key);
}
int AES_set_decrypt_key(const unsigned char *userKey, const int bits,
		AES_KEY *key) {
	static F_AES_set_decrypt_key AES_set_decrypt_key_fnPtr = NULL;
	if (!AES_set_decrypt_key_fnPtr) {
		checkLoadLibrary();
		AES_set_decrypt_key_fnPtr = (F_AES_set_decrypt_key) dlsym(handle,
				"AES_set_decrypt_key");
	}

	return AES_set_decrypt_key_fnPtr(userKey, bits, key);

}

EVP_PKEY *EVP_PKEY_new(void) {
	static F_EVP_PKEY_new EVP_PKEY_new_fnPtr = NULL;
	if (!EVP_PKEY_new_fnPtr) {
		checkLoadLibrary();
		EVP_PKEY_new_fnPtr = (F_EVP_PKEY_new) dlsym(handle, "EVP_PKEY_new");
	}

	return EVP_PKEY_new_fnPtr();
}

int EVP_PKEY_set1_RSA(EVP_PKEY *pkey, RSA *key) {
	static F_EVP_PKEY_set1_RSA EVP_PKEY_set1_RSA_fnPtr = NULL;
	if (!EVP_PKEY_set1_RSA_fnPtr) {
		checkLoadLibrary();
		EVP_PKEY_set1_RSA_fnPtr = (F_EVP_PKEY_set1_RSA) dlsym(handle,
				"EVP_PKEY_set1_RSA");
	}

	return EVP_PKEY_set1_RSA_fnPtr(pkey, key);

}
void EVP_MD_CTX_init(EVP_MD_CTX *ctx) {
	static F_EVP_MD_CTX_init EVP_MD_CTX_init_fnPtr = NULL;
	if (!EVP_MD_CTX_init_fnPtr) {
		checkLoadLibrary();
		EVP_MD_CTX_init_fnPtr = (F_EVP_MD_CTX_init) dlsym(handle,
				"EVP_MD_CTX_init");
	}

	EVP_MD_CTX_init_fnPtr(ctx);
}
const EVP_MD *EVP_md5(void) {
	static F_EVP_md5 EVP_md5_fnPtr = NULL;
	if (!EVP_md5_fnPtr) {
		checkLoadLibrary();
		EVP_md5_fnPtr = (F_EVP_md5) dlsym(handle, "EVP_md5");
	}

	return EVP_md5_fnPtr();
}
int EVP_DigestInit_ex(EVP_MD_CTX *ctx, const EVP_MD *type, ENGINE *impl) {
	static F_EVP_DigestInit_ex EVP_DigestInit_ex_fnPtr = NULL;
	if (!EVP_DigestInit_ex_fnPtr) {
		checkLoadLibrary();
		EVP_DigestInit_ex_fnPtr = (F_EVP_DigestInit_ex) dlsym(handle,
				"EVP_DigestInit_ex");
	}
	return EVP_DigestInit_ex_fnPtr(ctx, type, impl);
}

int EVP_SignFinal(EVP_MD_CTX *ctx, unsigned char *md, unsigned int *s,
		EVP_PKEY *pkey) {
	static F_EVP_SignFinal EVP_SignFinal_fnPtr = NULL;
	if (!EVP_SignFinal_fnPtr) {
		checkLoadLibrary();
		EVP_SignFinal_fnPtr = (F_EVP_SignFinal) dlsym(handle, "EVP_SignFinal");
	}
	return EVP_SignFinal_fnPtr(ctx, md, s, pkey);

}
int EVP_DigestUpdate(EVP_MD_CTX *ctx, const void *data, size_t count) {
	static F_EVP_DigestUpdate EVP_DigestUpdate_fnPtr = NULL;
	if (!EVP_DigestUpdate_fnPtr) {
		checkLoadLibrary();
		EVP_DigestUpdate_fnPtr = (F_EVP_DigestUpdate) dlsym(handle,
				"EVP_DigestUpdate");
	}
	return EVP_DigestUpdate_fnPtr(ctx, data, count);
}

int EVP_VerifyFinal(EVP_MD_CTX *ctx, const unsigned char *sigbuf,
		unsigned int siglen, EVP_PKEY *pkey) {
	static F_EVP_VerifyFinal EVP_VerifyFinal_fnPtr = NULL;
	if (!EVP_VerifyFinal_fnPtr) {
		checkLoadLibrary();
		EVP_VerifyFinal_fnPtr = (F_EVP_VerifyFinal) dlsym(handle,
				"EVP_VerifyFinal");
	}
	return EVP_VerifyFinal_fnPtr(ctx, sigbuf, siglen, pkey); 
}
