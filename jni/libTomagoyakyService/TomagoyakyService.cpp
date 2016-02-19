/*
 * TomagoyakyService.cpp
 *
 *  Created on: 2016年2月19日
 *      Author: peng
 */

#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include "TomagoyakyService.h"

using namespace TomagoyakyService;
//static struct sigaction oldact;
static pthread_key_t sigbuskey;

int TomagoyakyService::Instance() {
	LOGE("TomagoyakyService Instantiate\n");
	int ret = defaultServiceManager()->addService(String16("zp.svc"),
			new TomagoyakyService());
	LOGE("TomagoyakyService ret = %d\n", ret);
	return ret;
}
TomagoyakyService::TomagoyakyService() {
	LOGV("TomagoyakyService create\n");
	//m_NextConnId = 1;
	pthread_key_create(&sigbuskey, NULL);
}
TomagoyakyService::~TomagoyakyService() {
	pthread_key_delete(sigbuskey);
	LOGV("TomagoyakyService destory\n");
}

status_t TomagoyakyService::onTransact(uint32_t code, const Parcel& data,
		Parcel* reply, uint32_t flags) {
	switch (code) {
	case 0: {
		pid_t pid = data.readInt32();
		int num = data.readInt32();
		num += 1000;
		reply->writeInt32(num);
		return NO_ERROR;
	}
		break;
	default:
		return BBinder::onTransact(code, data, reply, flags);
	}
}

int TomagoyakyService::registerService() {
	sp<ProcessState> proc(ProcessState::self());
	sp<IServiceManager> sm = defaultServiceManager();
	LOGI("server - serviceManager: %p\n", sm.get());
	int ret = TomagoyakyService::Instance();
	LOGI("server - TomagoyakyService::Instance return %d\n", ret);
	ProcessState::self()->startThreadPool();
}
