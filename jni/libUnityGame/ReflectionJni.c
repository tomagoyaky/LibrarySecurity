#include <jni.h>
#include <assert.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <stdlib.h>
#include <stdio.h>

#include "jni_common.h"
#include "invoke.h"
#include "sm4.h"
#include "./crypto/signer.h"
#define CHECK_SIGN 0 //是否进行防二次打包校验
#define CHECK_SO 0

#if CHECK_SIGN
static  jobject jContextWrapper_getApplicationInfo(JNIEnv *env, jobject jContextWrapper) {

	jobject jResult = NULL;
	bool okey = false;

	okey =invoke_func()->callObjectMethod(env, &jResult, "android/content/ContextWrapper",
			jContextWrapper, "()Landroid/content/pm/ApplicationInfo;",
			"getApplicationInfo");
	if (!okey || !jResult) {
		LOGE("jContextWrapper_getApplicationInfo is null");
	}
	return jResult;
}
static  jstring jContextWrapper_getPackageSource(JNIEnv *env) {
	jstring jResult = NULL;
	bool okey = false;


	jobject jCurrentActivityThread = NULL;
	okey = invoke_func()->callStaticObjectMethod(env, &jCurrentActivityThread,
			"android/app/ActivityThread", "()Landroid/app/ActivityThread;",
			"currentActivityThread");
	if (!okey || !jCurrentActivityThread) {
		LOGE("getCurrentActivityThread null！");
		return -1;
	}

	jobject japplication = NULL;
	invoke_func()->callObjectMethod(env, &japplication,
			"android/app/ActivityThread", jCurrentActivityThread,
			"()Landroid/app/Application;", "getApplication");

	if (!japplication) {
		LOGE("japplication NULL");
		return -1;
	}



	jobject jApplicationInfo = jContextWrapper_getApplicationInfo(env, japplication);
	if (jApplicationInfo) {
		okey =  invoke_func()->getObject(env, &jResult,
				"android/content/pm/ApplicationInfo", jApplicationInfo,
				"sourceDir", "Ljava/lang/String;");
	}

	if (!okey || !jResult) {
		LOGE("jContextWrapper_getPackageSource is null");
	}

	(*env)->DeleteLocalRef(env,jCurrentActivityThread);
	(*env)->DeleteLocalRef(env,japplication);
	(*env)->DeleteLocalRef(env,jApplicationInfo);
	return jResult;

}

static  jstring jContextWrapper_getPackageName(JNIEnv *env) {
	jstring jResult = NULL;
	bool okey = false;


	jobject jCurrentActivityThread = NULL;
	okey = invoke_func()->callStaticObjectMethod(env, &jCurrentActivityThread,
			"android/app/ActivityThread", "()Landroid/app/ActivityThread;",
			"currentActivityThread");
	if (!okey || !jCurrentActivityThread) {
		LOGE("getCurrentActivityThread null！");
		return -1;
	}

	jobject japplication = NULL;
	invoke_func()->callObjectMethod(env, &japplication,
			"android/app/ActivityThread", jCurrentActivityThread,
			"()Landroid/app/Application;", "getApplication");

	if (!japplication) {
		LOGE("japplication NULL");
		return -1;
	}



	okey =invoke_func()->callObjectMethod(env, &jResult, "android/content/ContextWrapper",
			japplication, "()Ljava/lang/String;",
				"getPackageName");

	if (!okey || !jResult) {
		LOGE("jContextWrapper_getPackageName is null");
	}

	(*env)->DeleteLocalRef(env,jCurrentActivityThread);
	(*env)->DeleteLocalRef(env,japplication);
	return jResult;

}
#endif



static int JNI_readAsset(JNIEnv* env, char* pkgName, char* local_pkgName) {

	jobject jCurrentActivityThread = NULL;
	bool okey = false;
	okey = invoke_func()->callStaticObjectMethod(env, &jCurrentActivityThread,
			"android/app/ActivityThread", "()Landroid/app/ActivityThread;",
			"currentActivityThread");
	if (!okey || !jCurrentActivityThread) {
		LOGE("getCurrentActivityThread null！");
		return -1;
	}

	jobject japplication = NULL;
	invoke_func()->callObjectMethod(env, &japplication,
			"android/app/ActivityThread", jCurrentActivityThread,
			"()Landroid/app/Application;", "getApplication");

	if (!japplication) {
		LOGE("japplication NULL");
		return -1;
	}

//get pkg name
	jstring jpkgName = NULL;
	invoke_func()->callObjectMethod(env, &jpkgName,
			"android/content/ContextWrapper", japplication,
			"()Ljava/lang/String;", "getPackageName");

	if (!jpkgName) {
		LOGE("jpkgName NULL");
		return -1;
	}

	const char *mpkgName = (*env)->GetStringUTFChars(env, jpkgName, 0);
	memcpy(pkgName, mpkgName, strlen(mpkgName));

	(*env)->ReleaseStringUTFChars(env, jpkgName, mpkgName);

//read local pkg name
	jobject jassetmanager = NULL;
	invoke_func()->callObjectMethod(env, &jassetmanager,
			"android/content/ContextWrapper", japplication,
			"()Landroid/content/res/AssetManager;", "getAssets");
	if (!jassetmanager) {
		LOGE("jassetmanager NULL");
		return -1;
	}

	jstring filename = (*env)->NewStringUTF(env, "ajmforg.png");
	jboolean iscopy;
	const char *mfile = (*env)->GetStringUTFChars(env, filename, &iscopy);
	LOGD("file name :%s", mfile);

	AAssetManager* mgr = AAssetManager_fromJava(env, jassetmanager);
	AAsset* asset = AAssetManager_open(mgr, mfile, AASSET_MODE_UNKNOWN);
	(*env)->ReleaseStringUTFChars(env, filename, mfile);
	if (asset == NULL) {
		LOGI(" %s", "asset==NULL");
		return -1;
	}
	//get file size
	off_t bufferSize = AAsset_getLength(asset);
	LOGI("file size: %d\n", bufferSize);
	char *buffer = (char *) malloc(bufferSize + 1);
	buffer[bufferSize] = 0;
	int numBytesRead = AAsset_read(asset, buffer, bufferSize);

	unsigned char key[17] = { 0x22, 0x98, 0x56, 0x48, 0x89, 0x0e, 0xcd, 0xef,
			0xfe, 0xd1, 0xbc, 0x23, 0x76, 0x54, 0x32, 0x18, 0x00 };

	sm4_context ctx;
	sm4_setkey_dec(&ctx, key);
	sm4_crypt_ecb(&ctx, 0, numBytesRead, buffer, buffer);
	LOGI("content:%s", buffer);
	memcpy(local_pkgName, buffer, bufferSize);
	free(buffer);
	AAsset_close(asset);

	(*env)->DeleteLocalRef(env, jCurrentActivityThread);
	(*env)->DeleteLocalRef(env, japplication);
	return 0;

}


static int check4so(char* pkgName, char* local_pkgName) {
	if (pkgName != NULL && local_pkgName != NULL) {
		LOGD("%s vs %s", pkgName, local_pkgName);
		if (0 != strcmp(pkgName, local_pkgName)) {
			LOGD("not match app");
			exit(0);
		}
	} else {
		LOGD("read info error");
		exit(0);
	}
	return 0;
}


#if CHECK_SIGN
static void sign_callback(bool verify) {
	LOGW("signer verify result : %s", verify ? "true" : "false");
	if (!verify)
		exit(0);
}
#endif

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	JNIEnv* env = NULL;
	jint result = -1;

	/*
	 if (vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
	 return result;
	 }
	 */
	if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		return result;
	}

#if CHECK_SO
	char pkg_name[300] = { 0 };
	char local_pkg_name[300] = { 0 };
	JNI_readAsset(env, pkg_name, local_pkg_name);
	check4so(pkg_name, local_pkg_name);
#endif	

#if CHECK_SIGN
	jstring jApkPath = jContextWrapper_getPackageSource(env);
	jstring jPackageName = jContextWrapper_getPackageName(env);
	char * apkpath = (*env)->GetStringUTFChars(env,jApkPath,0);
	char * packageName = (*env)->GetStringUTFChars(env,jPackageName,0);
	//verify the signature
	SignThreadData *std = malloc(sizeof(SignThreadData));
	std->on_callback = sign_callback;
	std->packagename = strdup(packageName);
	std->apkpath = strdup(apkpath);

	LOGE("apkpath:%s",apkpath);
	LOGE("pkg_name:%s",packageName);

	(*env)->ReleaseStringUTFChars(env,jApkPath,apkpath);
	(*env)->ReleaseStringUTFChars(env,jPackageName,packageName);
	signer_verify_async(std);
#endif

	return JNI_VERSION_1_4;
}
