package com.tomagoyaky.librarysecurity;

import java.io.File;
import java.io.IOException;

import android.app.Application;
import android.content.Context;
import android.util.Log;
import android.widget.Toast;
import cn.trinea.android.common.util.AssetDatabaseOpenHelper;

public class SecurityApplication extends Application {

	private Context context;
	protected static final String TAG = "tomagoyaky";
	@Override
	protected void attachBaseContext(Context base) {
		super.attachBaseContext(base);

		System.loadLibrary("Security");
	}

	@Override
	public void onCreate() {
		super.onCreate();
		
		context = this;
		NativeBridge.SayHello();
		
		File targetFile = new File("/data/local/tmp/tomagoyaky_server");
		try {
			Log.d(TAG, "Release assets of 'tomagoyaky_server'");
			new AssetDatabaseOpenHelper(context, "tomagoyaky_server").copyDatabase(targetFile);
		} catch (Exception e) {
			Toast.makeText(context, e.getMessage(), Toast.LENGTH_LONG).show();
			e.printStackTrace();
		}
	}
}
