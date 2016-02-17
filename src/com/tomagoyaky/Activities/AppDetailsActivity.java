package com.tomagoyaky.Activities;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import android.app.Activity;
import android.app.ActivityManager;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.os.Bundle;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;
import android.widget.ScrollView;
import android.widget.TextView;
import cn.trinea.android.common.util.ShellUtils;
import cn.trinea.android.common.util.ShellUtils.CommandResult;

public class AppDetailsActivity extends Activity{
	
	private Context context;
	private String pkgName;
	private String name;
	private String isSystemApp;
	
	protected static final String TAG = "tomagoyaky";

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		context = this;
		Intent intent = this.getIntent(); 
		pkgName = intent.getStringExtra("pkg");
		name = intent.getStringExtra("name");
		isSystemApp = intent.getStringExtra("isSystemApp");
		this.setTitle(name);

		final TextView textView = new TextView(context);
		textView.setLayoutParams(new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
		final StringBuilder sb = new StringBuilder();
		sb.append("AppName: " + name);
		sb.append("\npkgName: " + pkgName);
		// 应用信息
		ArrayList<AppInfo> appList = new ArrayList<AppInfo>();
		List<PackageInfo> packages = getPackageManager().getInstalledPackages(0);

		for (int i = 0; i < packages.size(); i++) {
			PackageInfo packageInfo = packages.get(i);
			if(packageInfo.packageName.equals(pkgName)) {
				SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
				sb.append("\nfirstInstallTime: " + format.format(new Date(packageInfo.firstInstallTime)));
				sb.append("\nlastUpdateTime: " + format.format(new Date(packageInfo.lastUpdateTime)));
				sb.append("\napplication: " + packageInfo.applicationInfo.className);
				sb.append("\ndataDir: " + packageInfo.applicationInfo.dataDir);
				sb.append("\nnativeLibraryDir: " + packageInfo.applicationInfo.nativeLibraryDir);
			}
		}
        sb.append("\n-----------------------------------------------------------------------");
        // 系统信息
        TelephonyManager tm = (TelephonyManager) getSystemService(Context.TELEPHONY_SERVICE);
        sb.append("\nDeviceId(IMEI) = " + tm.getDeviceId());
        sb.append("\nDeviceSoftwareVersion = " + tm.getDeviceSoftwareVersion());

        switch (tm.getNetworkType()) {
		case TelephonyManager.NETWORK_TYPE_UNKNOWN:
	        sb.append("\nNetworkType = unknow NetWork");
			break;
		case TelephonyManager.NETWORK_TYPE_GPRS:
	        sb.append("\nNetworkType = GPRS NetWork");
			break;
		case TelephonyManager.NETWORK_TYPE_EDGE:
	        sb.append("\nNetworkType = EDGE NetWork");
			break;
		case TelephonyManager.NETWORK_TYPE_UMTS:
	        sb.append("\nNetworkType = UMTS NetWork");
			break;
		case TelephonyManager.NETWORK_TYPE_HSDPA:
	        sb.append("\nNetworkType = HSDPA NetWork");
			break;
		case TelephonyManager.NETWORK_TYPE_HSUPA:
	        sb.append("\nNetworkType = HSUPA NetWork");
			break;
		case TelephonyManager.NETWORK_TYPE_HSPA:
	        sb.append("\nNetworkType = HSPA NetWork");
			break;
		case TelephonyManager.NETWORK_TYPE_CDMA:
	        sb.append("\nNetworkType = CDMA NetWork,IS95A 或 IS95B.");
			break;
		case TelephonyManager.NETWORK_TYPE_EVDO_0:
	        sb.append("\nNetworkType = EVDO NetWork, revision 0.");
			break;
		case TelephonyManager.NETWORK_TYPE_EVDO_A:
	        sb.append("\nNetworkType = EVDO NetWork, revision A.");
			break;
		case TelephonyManager.NETWORK_TYPE_1xRTT:
	        sb.append("\nNetworkType = 1xRTT NetWork");
			break;

		default:
			break;
		}
        
        sb.append("\nPhoneType = " + tm.getPhoneType());
        sb.append("\nSimState = " + tm.getSimState());
        sb.append("\nSubscriberId(IMSI) = " + tm.getSubscriberId());
		
        DisplayMetrics dm = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(dm);
        sb.append("\ndpi=(" + dm.widthPixels + "*" + dm.heightPixels + ")");

        // 获取so表
        List<String> commnandList1 = new ArrayList<String>();
        if(Integer.parseInt(isSystemApp) == 1)
        	commnandList1.add("ls -l /data/app-lib/");
        else
        	commnandList1.add("ls -l /data/data/" + pkgName + "/lib/");
		CommandResult result1 = ShellUtils.execCommand(commnandList1, true);

		ArrayList<String> soList = new ArrayList<String>();
		String[] soLineArray = result1.successMsg.split("\n");
		if(soLineArray.length > 0 && soLineArray[0] != ""){
			for (String line : soLineArray) {
				String[] item = line.split(" ");
				soList.add(item[item.length - 1]);
			}
			
			if(soList.size() > 0){
		        sb.append("\n-----------------------------------------------------------------------");
				for (String soFileName : soList) {
					sb.append("\n[+]" + soFileName);
				}
			}
			if(result1.result != 0)
				sb.append("\n[-]" + result1.errorMsg);
		}

        // 运行信息
		ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        List<RunningAppProcessInfo> run = am.getRunningAppProcesses();
        for (RunningAppProcessInfo runningProcess : run) {
            if ((runningProcess.processName != null) && runningProcess.processName.equals(pkgName)) {
                sb.append("\n-----------------------------------------------------------------------");
                sb.append("\n>>> uid: " + runningProcess.uid);
                sb.append("\n>>> pid: " + runningProcess.pid);
                sb.append("\n>>> processName: " + runningProcess.processName);

                sb.append("\n-----------------------------------------------------------------------");
				List<String> commnandList = new ArrayList<String>();
				int pid = android.os.Process.myPid();
				commnandList.add("cat /proc/" + pid + "/status");
				CommandResult result = ShellUtils.execCommand(commnandList, true);

//				SearchMemory(data, android.os.Process.myPid())
				if(result.result == 0)
					sb.append("\n>>>" + result.successMsg);
				else
					sb.append("\n>>>" + result.errorMsg);

				textView.setText(sb.toString());
                break;
            }
        }
        
		textView.setText(sb.toString());
		LinearLayout frame = new LinearLayout(context);
		ScrollView scrollView = new ScrollView(context);
		scrollView.addView(textView);
		frame.addView(scrollView);
		this.setContentView(frame);
	}
}
