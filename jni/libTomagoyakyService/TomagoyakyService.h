/*
 * TomagoyakyService.h
 *
 *  Created on: 2016年2月19日
 *      Author: peng
 */

#ifndef TomagoyakyService_H_
#define TomagoyakyService_H_

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <binder/Binder.h>
#include "dialog.h"
#include "log.h"

using namespace android;

namespace TomagoyakyService {
	class TomagoyakyService: public BBinder {
	private:
		//mutable Mutex m_Lock;
		//int32_t m_NextConnId;
	public:
		static int Instance();
		TomagoyakyService();
		virtual ~TomagoyakyService();
		virtual status_t onTransact(uint32_t, const Parcel&, Parcel*, uint32_t);
		int registerService();
	};
}

#endif /* TomagoyakyService_H_ */
