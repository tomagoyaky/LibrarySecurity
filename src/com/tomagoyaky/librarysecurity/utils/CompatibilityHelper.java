package com.tomagoyaky.librarysecurity.utils;

/**
 * 兼容性助手
 * */
public class CompatibilityHelper {

	// 检查是否兼容
	public static boolean isMatch(){
		return true;
	}
	
	public static String getBuildMODEL(){
		return android.os.Build.MODEL;
	}
	
	public static int getBuildSDK(){
		return android.os.Build.VERSION.SDK_INT;
	}
}
