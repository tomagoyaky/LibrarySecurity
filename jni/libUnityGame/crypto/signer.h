/*
 * signer.h
 *
 *  Created on: 2015年3月31日
 *      Author: Jevey
 */

#ifndef SIGNER_H_
#define SIGNER_H_

#define bool unsigned char

typedef void (*signer_callback)(bool);
typedef struct {
	signer_callback on_callback;
	//thread count
	int thread_num;
	char * packagename;
	char * apkpath;

} SignThreadData;

typedef struct Signer {
	bool (*verify)();
	//依赖g->packagename
	bool (*verify_async)(SignThreadData *data);

} Signer;
bool signer_verify_async(SignThreadData *data);
#endif /* SIGNER_H_ */
