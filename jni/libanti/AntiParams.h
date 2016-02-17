/*
 * AntiParams.h
 *
 *  Created on: 2016年1月29日
 *      Author: peng
 */

#ifndef LIBANTI_ANTIPARAMS_H_
#define LIBANTI_ANTIPARAMS_H_

#include <limits.h>

namespace ANTIPARAMS{
	enum Tactics { Flag_Exit, Flag_ConsolePrint, Flag_FileLog};
	typedef struct AntiParams{
		const char *name;
		int tactics;
		int pid;

		// for unhook
		const char libName[NAME_MAX];
		const char libPath[NAME_MAX];
		const char libNamePath[NAME_MAX];
		void *func;

		//
		int sleepTime;

	}*PointAntiParams;
	int ResultProcess(PointAntiParams antiParams);
}
#endif /* LIBANTI_ANTIPARAMS_H_ */
