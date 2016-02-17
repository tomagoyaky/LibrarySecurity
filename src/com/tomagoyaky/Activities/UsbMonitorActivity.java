package com.tomagoyaky.Activities;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

import com.tomagoyaky.Protector.CheckUSBState;

public class UsbMonitorActivity extends Activity {
	
	private TextView mInfo;
	protected static final String TAG = "tomagoyaky";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		mInfo = new TextView(this);
		mInfo.append(CheckUSBState.getInfo() +  "\n");
		setContentView(mInfo);
	}
}
