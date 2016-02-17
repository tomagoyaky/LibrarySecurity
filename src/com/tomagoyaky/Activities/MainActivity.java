package com.tomagoyaky.Activities;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;

import com.tomagoyaky.librarysecurity.R;

public class MainActivity extends Activity{
	
	protected static final String TAG = "tomagoyaky";
	private Context context = null;
	
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		context = this;

		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

		ImageView iv = new ImageView(context);
		iv.setBackgroundResource(R.drawable.launcher);
		this.setContentView(iv);
	
		iv.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(context, AppInfoActivity.class);
				context.startActivity(intent);
			}
		});
	}
}
