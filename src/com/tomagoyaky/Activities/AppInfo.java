package com.tomagoyaky.Activities;

import android.graphics.drawable.Drawable;
import android.util.Log;

public class AppInfo{
	
	public String appName = "";
	public String packageName = "";
	public String versionName = "";
	public int versionCode = 0;
	public Drawable appIcon = null;
	public long firstInstallTime;
	public int pid = -1;
	public int isSystemApp = -1;

	public void print() {
		Log.v("app", "Name:" + appName + " PID:" + pid);
		Log.v("app", "Name:" + appName + " PKG:" + packageName);
		Log.v("app", "Name:" + appName + " VN:" + versionName);
		Log.v("app", "Name:" + appName + " VC:" + versionCode);
		Log.v("app", "Name:" + appName + " FIT:" + firstInstallTime);
	}
}