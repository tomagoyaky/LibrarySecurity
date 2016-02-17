/*
 * MD5.h
 *
 *  Created on: Sep 1, 2012
 *      Author: jevey
 */

#ifndef MD5_H_
#define MD5_H_

#include <stdint.h>

 

#define uint32_t unsigned int


typedef  struct MD5
{

	void (*MD5_Calculate)(const unsigned char* source, uint32_t len,char m_sHash[32]);

}MD5;

#endif
 
