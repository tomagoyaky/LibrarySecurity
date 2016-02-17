package com.tomagoyaky.Activities;

import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.app.ActivityManager;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnLongClickListener;
import android.view.ViewGroup;
import android.widget.AbsListView;
import android.widget.AbsListView.OnScrollListener;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import com.tomagoyaky.Protector.CheckSignature;
import com.tomagoyaky.librarysecurity.R;
public class AppInfoActivity extends Activity {

	protected static final String TAG = "tomagoyaky";

	private Context context = null;
	private ListView lv;
	private List<AppInfo> data;

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		context = this;

		setContentView(R.layout.main);
		lv = (ListView) findViewById(R.id.lv);
		lv.setOnScrollListener(new OnScrollListener(){  
		    @Override  
		    public void onScrollStateChanged(AbsListView view, int scrollState){  
		        if (scrollState == OnScrollListener.SCROLL_STATE_IDLE) {  
		            if (view.getLastVisiblePosition() == view.getCount() - 1) {  
		            	Toast.makeText(context, "到达底部了", Toast.LENGTH_SHORT).show();
		            }  
		        }  
		    }
			@Override
			public void onScroll(AbsListView view, int firstVisibleItem,
					int visibleItemCount, int totalItemCount) {
				// TODO Auto-generated method stub
				
			}  
		});

		data = getData();
		MyAdapter adapter = new MyAdapter(this);
		lv.setAdapter(adapter);
	}

	private ArrayList<AppInfo> getData() {

		ArrayList<AppInfo> appList = new ArrayList<AppInfo>();
		List<PackageInfo> packages = getPackageManager().getInstalledPackages(0);
		
		ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        List<RunningAppProcessInfo> run = am.getRunningAppProcesses();

		for (int i = 0; i < packages.size(); i++) {
			PackageInfo packageInfo = packages.get(i);
			AppInfo tmpInfo = new AppInfo();
			tmpInfo.appName = packageInfo.applicationInfo.loadLabel(getPackageManager()).toString();
	        for (RunningAppProcessInfo runningProcess : run) {
	            if ((runningProcess.processName != null) && runningProcess.processName.equals(packageInfo.packageName)) {
	    			tmpInfo.pid = runningProcess.pid;
	    			break;
	            }
	        }
			tmpInfo.isSystemApp = packageInfo.applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM;
			tmpInfo.packageName = packageInfo.packageName;
			tmpInfo.versionName = packageInfo.versionName;
			tmpInfo.versionCode = packageInfo.versionCode;
			tmpInfo.appIcon = packageInfo.applicationInfo.loadIcon(getPackageManager());
			tmpInfo.firstInstallTime = packageInfo.firstInstallTime;
			appList.add(tmpInfo);
		}
		return appList;
	}

	static class ViewHolder {
		public LinearLayout item;
		public ImageView img;
		public TextView title;
		public TextView info;
		public TextView version;
	}

	public class MyAdapter extends BaseAdapter{
		
		private LayoutInflater mInflater = null;
		private MyAdapter(Context context) {
			this.mInflater = LayoutInflater.from(context);
		}

		@Override
		public int getCount() {
			// How many items are in the data set represented by this Adapter.
			return data.size();
		}

		@Override
		public Object getItem(int position) {
			// Get the data item associated with the specified position in the
			// data set.
			return position;
		}

		@Override
		public long getItemId(int position) {
			// Get the row id associated with the specified position in the
			// list.
			return position;
		}

		// Get a View that displays the data at the specified position in the
		// data set.
		@Override
		public View getView(int position, View convertView, ViewGroup parent) {
			ViewHolder holder = null;
			if (convertView == null) {
				holder = new ViewHolder();
				convertView = mInflater.inflate(R.layout.list_item, null);
				holder.item = (LinearLayout) convertView.findViewById(R.id.item);
				holder.img = (ImageView) convertView.findViewById(R.id.img);
				holder.title = (TextView) convertView.findViewById(R.id.tv);
				holder.info = (TextView) convertView.findViewById(R.id.info);
				holder.version = (TextView) convertView.findViewById(R.id.version);
				convertView.setTag(holder);
			} else {
				holder = (ViewHolder) convertView.getTag();
			}
			final AppInfo appInfo = (AppInfo)data.get(position);
			holder.img.setBackground(appInfo.appIcon);
			if(appInfo.pid != -1){
				holder.title.setText(appInfo.appName + ":" + appInfo.pid);
			}else{
				holder.title.setText(appInfo.appName);
			}
			holder.info.setText(appInfo.packageName);
			holder.version.setText(appInfo.versionName);
			holder.item.setOnClickListener(new OnClickListener() {
				
				@Override
				public void onClick(View v) {
					Intent intent = new Intent(context, AppDetailsActivity.class);
					intent.putExtra("name", appInfo.appName);
					intent.putExtra("pkg", appInfo.packageName);
					intent.putExtra("isSystemApp", String.valueOf(appInfo.isSystemApp));
					context.startActivity(intent);
				}
			});
			holder.item.setOnLongClickListener(new OnLongClickListener() {
				
				@Override
				public boolean onLongClick(View v) {
					AlertDialog.Builder builder = new Builder(AppInfoActivity.this);
					ScrollView scrollView = new ScrollView(context);
					builder.setView(findViewById(R.layout.alert_scroll));
					CharSequence[] menuData = getResources().getStringArray(R.array.MenuItemArray);
					builder.setItems(menuData, new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface arg0, int index) {
							
							switch (index) {
							case 0:
								CheckSignature.start(context, appInfo.packageName);
								break;
							case 1:
								Toast.makeText(context, "unimplement!", Toast.LENGTH_SHORT).show();
								break;
							case 2:
								if(appInfo.pid == -1){
									Toast.makeText(context, "App not running!", Toast.LENGTH_SHORT).show();
									break;
								}
								Intent intent = new Intent(context, MemorySearchActivity.class);
								intent.putExtra("appName", appInfo.appName);
								intent.putExtra("pid", String.valueOf(appInfo.pid));
								context.startActivity(intent);
								break;

							default:
								Toast.makeText(context, "unimplement!", Toast.LENGTH_SHORT).show();
								break;
							}
							arg0.dismiss();
						}
					});
					builder.show();
					return false;
				}
			});

			return convertView;
		}
	}
	
	private long exitTime = 0;
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
	    if(keyCode == KeyEvent.KEYCODE_BACK && event.getAction() == KeyEvent.ACTION_DOWN){   
	        if((System.currentTimeMillis() - exitTime) > 2000){  
	            Toast.makeText(getApplicationContext(), "再按一次退出程序", Toast.LENGTH_SHORT).show();                                
	            exitTime = System.currentTimeMillis();   
	        } else {
	            finish();
	            System.exit(0);
	        }
	        return true;   
	    }
	    return super.onKeyDown(keyCode, event);
	}
}
