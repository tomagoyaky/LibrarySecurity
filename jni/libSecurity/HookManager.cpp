/*
 * HookManager.cpp
 *
 *      Author: peng
 */

#include <jni.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/system_properties.h>
#include <pthread.h>

//#include "libdex/DexFile.h"
//#include "vm/DvmDex.h"
#include "hook.h"
#include "log.h"
//
//int (*org_socket)(int domain, int socket_type, int protocol);
//int (*org_connect)(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
//ssize_t (*org_send)(int sockfd, const void *buf, size_t len, int flags);
//ssize_t (*org_sendto)(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
//ssize_t (*org_recv)(int sockfd, void *buf, size_t len, int flags);
//ssize_t (*org_recvfrom)(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
//
//int my_socket(int domain, int socket_type, int protocol);
//int my_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
//ssize_t my_send(int sockfd, const void *buf, size_t len, int flags);
//ssize_t my_sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
//ssize_t my_recv(int sockfd, void *buf, size_t len, int flags);
//ssize_t my_recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
//
//int get_sdk_version(void);
//int (*org_getEntryInfo)(int obj, int entry, int *a3, unsigned int *a4, unsigned int *a5, int *a6, int *a7, int *a8);
//int (*org_getEntryName)(int obj, int entry, char *a3, int a4);
//int (*org_creatFileMap)(int obj, const char *a2, int a3, int a4, long long a5, unsigned int a6, bool a7);
//int (*org_creatFileMap_23)(int obj, const char *a2, int a3, int a4, unsigned int a5, bool a6);
//
//int (*org_ioctl)(int d, int request, ...);
//int my_ioctl(int d, int request, ...);
//
//void *(*org_malloc)(size_t size);
//void *my_malloc(size_t size);
//void (*org_free)(void *ptr);
//void my_free(void *ptr);
//
//int (*org_dvmDexFileOpenPartial)(const void* addr, int len, DvmDex** ppDvmDex);
//int my_dvmDexFileOpenPartial(const void* addr, int len, DvmDex** ppDvmDex);
//
//jint (*org_RegisterNatives)(JNIEnv* env, jclass jclazz, const JNINativeMethod* methods, jint nMethods);
//jint my_RegisterNatives(JNIEnv* env, jclass jclazz, const JNINativeMethod* methods, jint nMethods);
//
//int (*org_pthread_create)(pthread_t *thread, void *arr,void* (*start_routine)(void *), void *arg);
//int my_pthread_create(pthread_t *thread, void *arr,void* (*start_routine)(void *), void *arg);
//
///**
// * opengl
// * */
////EGLDisplay eglGetDisplay(NativeDisplayType display); 	// ��ȡ��ʾ��
////EGLBoolean eglInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor); // �ɻ�ȡ�汾��
////EGLSurface eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config,		// ����һ����ʵ����ʾ��EGLSurface��ʵ���Ͼ���һ��FrameBuffer
////			  NativeWindowType win, const EGLint *attrib_list);
//// hash
//unsigned long long hash64(void* key, size_t keySize);
///*=======================================================================*/
//
//
//int my_socket(int domain, int socket_type, int protocol){
//
//	LOGD("domain:%d, socket_type:%d, protocol:%d", domain, socket_type, protocol);
//	return org_socket(domain, socket_type, protocol);
//}
//
//int my_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen){
//
//	LOGD("sockfd:%d, addr:%p, addrlen:%d", sockfd, addr, addrlen);
//	return org_connect(sockfd, addr, addrlen);
//}
//
//ssize_t my_send(int sockfd, const void *buf, size_t len, int flags){
//
//	unsigned long long hashValue = hash64((void*)buf, len);
//	LOGD("hashValue:%llu, sockfd:%d, buf:%p, len:%d, flags:%d", hashValue, sockfd, buf, len, flags);
//	DebugMem((void*)buf, len, true);
//	return org_send(sockfd, buf, len, flags);
//}
//
//ssize_t my_sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen){
//
//	unsigned long long hashValue = hash64((void*)buf, len);
//	LOGD("hashValue:%llu, sockfd:%d, buf:%p, len:%d, flags:%d, dest_addr:%p, addrlen:%d", hashValue, sockfd, buf, len, flags, dest_addr, addrlen);
//	DebugMem((void*)buf, len, true);
//	return org_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
//}
//
//ssize_t my_recv(int sockfd, void *buf, size_t len, int flags){
//
//	unsigned long long hashValue = hash64(buf, len);
//	LOGD("hashValue:%llu, sockfd:%d, buf:%p, len:%d, flags:%d", hashValue, sockfd, buf, len, flags);
//	DebugMem(buf, len, true);
//	int target_pid = getpid();
////	if(ptrace_attach(target_pid) == -1){
////		LOGE("");
////	}
//	return org_recv(sockfd, buf, len, flags);
//}
//
//ssize_t my_recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen){
//
//	unsigned long long hashValue = hash64(buf, len);
//	LOGD("hashValue:%llu, sockfd:%d, buf:%p, len:%d, flags:%d, src_addr:%p, addrlen:%p", hashValue, sockfd, buf, len, flags, src_addr, addrlen);
//	DebugMem(buf, len, true);
//	int target_pid = getpid();
////	if(ptrace_attach(target_pid) == -1){
////
////	}
//	return org_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
//}
//
//
//static char mResFileName[NAME_MAX] = { 0 };
//int get_sdk_version(void){
//	char SDKVersion[10] = { 0 };
//	__system_property_get("ro.build.version.sdk", SDKVersion);
//
//	LOGD("SDK version :%s", SDKVersion);
//	return atoi(SDKVersion);
//}
//
//int my_getEntryName(int obj, int entry, char *a3, int a4) {
//
//	return org_getEntryName(obj, entry, a3, a4);
//}
//
//int my_getEntryInfo(int obj, int entry, int *a3, unsigned int *a4, unsigned int *a5, int *a6, int *a7, int *a8) {
//
//	char resName[100] = { 0 };
//	org_getEntryName(obj, entry, resName, sizeof(resName));
//	memset(mResFileName, 0, sizeof(mResFileName));
//	memcpy(mResFileName, resName, sizeof(resName));
//	LOGD("mResFileName : %s", mResFileName);
//	return org_getEntryInfo(obj, entry, a3, a4, a5, a6, a7, a8);
//}
//
//int my_creatFileMap_23(int obj, const char *a2, int a3, int a4, unsigned int a5, bool a6) {
//
//	return org_creatFileMap_23(obj, a2, a3, a4, a5, a6);
//}
//
//int my_creatFileMap(int obj, const char *a2, int a3, int a4, long long a5, unsigned int a6, bool a7) {
//
//	return org_creatFileMap(obj, a2, a3, a4, a5, a6, a7);
//}
//
//int my_ioctl(int d, int request, ...){
//
////	return org_ioctl(d, request, ...);
//	return 0;
//}
//
//void *my_malloc(size_t size){
//
//	void *buffer = org_malloc(size);
////	LOGW("buffer:%p, size:%d", buffer, size);
//	return buffer;
//}
//void my_free(void *ptr){
//
//	if(ptr != NULL){
////		LOGW("buffer:%p", ptr);
////		DebugMem(ptr, 0x30, true);
//	}else{
////		LOGE("buffer is null");
//	}
//}
//
//int my_dvmDexFileOpenPartial(const void* addr, int len, DvmDex** ppDvmDex){
//
//	LOGW("addr:%p, len:%d, ppDvmDex:%p", addr, len, ppDvmDex);
//	int ret = org_dvmDexFileOpenPartial(addr, len, ppDvmDex);
//	return ret;
//}
//
//jint my_RegisterNatives(JNIEnv* env, jclass jclazz, const JNINativeMethod* methods, jint nMethods){
//
//	LOGW("registerMethodName:%s", methods->name);
//	jint ret = org_RegisterNatives(env, jclazz, methods, nMethods);
//	return ret;
//}
//
//int my_pthread_create(pthread_t *thread, void *arr,void* (*start_routine)(void *), void *arg){
//
//	LOGW("start_routine:%p, thread_pid:%ld", start_routine, (long)thread);
//	return org_pthread_create(thread, arr, start_routine, arg);
//}
//
//unsigned long long hash64(void* key, size_t keySize) {
//
//	unsigned long long h = keySize;
//	char* data = (char*) key;
//	size_t i;
//	for (i = 0; i < keySize; i++) {
//		h = h * 31 + *data;
//		data++;
//	}
//	return h;
//}
//
//extern JavaVM* mDalvikVm;
//
//void HookInit(){
//
//	int ret = 0;
//
//	ret = elfHook("libc.so", "socket", (void *) my_socket, (void**) &org_socket);
//	ret = elfHook("libc.so", "connect", (void *) my_connect, (void**) &org_connect);
//	ret = elfHook("libc.so", "send", (void *) my_send, (void**) &org_send);
//	ret = elfHook("libc.so", "sendto", (void *) my_sendto, (void**) &org_sendto);
//	ret = elfHook("libc.so", "recv", (void *) my_recv, (void**) &org_recv);
//	ret = elfHook("libc.so", "recvfrom", (void *) my_recvfrom, (void**) &org_recvfrom);
////	ret = elfHook("libbinder.so", "ioctl", (void *) my_ioctl, (void**) &org_ioctl);
//	ret = elfHook("libhwui.so", "ioctl", (void *) my_ioctl, (void**) &org_ioctl);
//
//	// ZipEntry����
//
//	ret = 0;
//	int sdk_version = get_sdk_version();
//	if (sdk_version < 14) {
//
//		ret = elfHook("libutils.so",
//				"_ZNK7android9ZipFileRO16getEntryFileNameEPvPci",
//				(void *) my_getEntryName, (void**) &org_getEntryName);
//
//		ret = elfHook("libutils.so",
//				"_ZNK7android9ZipFileRO12getEntryInfoEPvPiPjS3_PlS4_S4_",
//				(void *) my_getEntryInfo, (void**) &org_getEntryInfo);
//
//		ret = elfHook("libutils.so", "_ZN7android7FileMap6createEPKciljb",
//				(void *) my_creatFileMap_23, (void**) &org_creatFileMap_23);
//	} else if (sdk_version >= 14 && sdk_version < 19) {
//		ret = elfHook("libutils.so",
//				"_ZNK7android9ZipFileRO16getEntryFileNameEPvPci",
//				(void *) my_getEntryName, (void**) &org_getEntryName);
//
//		ret = elfHook("libutils.so",
//				"_ZNK7android9ZipFileRO12getEntryInfoEPvPiPjS3_PxPlS5_",
//				(void *) my_getEntryInfo, (void**) &org_getEntryInfo);
//
//		ret = elfHook("libutils.so", "_ZN7android7FileMap6createEPKcixjb",
//				(void *) my_creatFileMap, (void**) &org_creatFileMap);
//
//	} else if (sdk_version >= 19) {
//		ret = elfHook("libandroidfw.so",
//				"_ZNK7android9ZipFileRO16getEntryFileNameEPvPci",
//				(void *) my_getEntryName, (void**) &org_getEntryName);
//
//		ret = elfHook("libandroidfw.so",
//				"_ZNK7android9ZipFileRO12getEntryInfoEPvPiPjS3_PxPlS5_",
//				(void *) my_getEntryInfo, (void**) &org_getEntryInfo);
//
//		ret = elfHook("libutils.so", "_ZN7android7FileMap6createEPKcixjb",
//				(void *) my_creatFileMap, (void**) &org_creatFileMap);
//	}
//
//	ret = elfHook("libc.so", "malloc", (void *) my_malloc, (void**) &org_malloc);
//	ret = elfHook("libc.so", "free", (void *) my_free, (void**) &org_free);
//
//	/*
//	 * libEGL_android.so��libGLESV1_cm.so��libGLESV2.so
//	 * */
////	ret = elfHook("libGLES_android.so", "free", (void *) my_free, (void**) &org_free);
//
//	// libnetworkbase.so
////	ret = elfHook("libnetworkbase.so", "free", (void *) my_free, (void**) &org_free);
//
//	// DexFile
//	ret = elfHook("libdvm.so", "_Z21dvmDexFileOpenPartialPKviPP6DvmDex",
//			(void *) my_dvmDexFileOpenPartial, (void**) &org_dvmDexFileOpenPartial); // dvmDexFileOpenPartial
//
//	ret = elfHook("libdvm.so", "RegisterNatives",
//			(void *) my_RegisterNatives, (void**) &org_RegisterNatives); // RegisterNatives
//}
