package com.tomagoyaky.Activities;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnFocusChangeListener;
import android.view.ViewGroup.LayoutParams;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;

import com.tomagoyaky.Attacker.MemSearch;

public class MemorySearchActivity extends Activity {
	
	protected static final String TAG = "tomagoyaky";
	private Context context;
	private TextView textView;
	private EditText editText;
	private String appName;
	private String pid;
	private InputMethodManager imm;
	private final Handler handler = new Handler(){

		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			
			switch (msg.what) {
			case 1:
				textView.setText(msg.obj.toString());
				break;

			default:
				break;
			}
		}
	};
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		context = this;
		imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
		Intent intent = this.getIntent(); 

		appName = intent.getStringExtra("appName");
		pid = intent.getStringExtra("pid");
		this.setTitle(appName + ":" + pid);
		
		LinearLayout Frame = new LinearLayout(context);
		this.editText = new EditText(context);
		Button btn_search = new Button(context);
		this.textView = new TextView(context);

		this.editText.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, 200));
		this.editText.setOnFocusChangeListener(new OnFocusChangeListener() {
			
			@Override
			public void onFocusChange(View v, boolean hasFocus) {
				
				if(hasFocus){
					editText.setInputType(EditorInfo.TYPE_CLASS_PHONE);
					imm.showSoftInput(editText, InputMethodManager.SHOW_FORCED);  
				}
			}
		});
		this.textView.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
		this.textView.setText("请输入程序中出现的数据");
		btn_search.setText("search");
		btn_search.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				imm.hideSoftInputFromWindow(editText.getWindowToken(), 0); //强制隐藏键盘  
				String data = editText.getEditableText().toString();
				String result = MemSearch.dowork(data, Integer.parseInt(pid));
				
				Message msg = new Message();
				msg.what = 1;
				msg.obj = "result of {" + data + "}:\n" + result;
				handler.sendMessage(msg);
			}
		});

		Frame.setOrientation(LinearLayout.VERTICAL);
		Frame.addView(this.editText);
		Frame.addView(btn_search);
		
		LinearLayout scrollFrame = new LinearLayout(context);
		ScrollView scrollView = new ScrollView(context);
		scrollView.addView(this.textView);
		scrollFrame.addView(scrollView);
		Frame.addView(scrollFrame);
		
		this.setContentView(Frame);
	}
}
