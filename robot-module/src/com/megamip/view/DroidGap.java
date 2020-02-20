package com.megamip.view;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import com.megamip.voice.R;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.widget.ScrollView;
import android.widget.TextView;

public class DroidGap extends Activity  {

	private WebView wv;
	private MegamipGui megamipGui;
	private Handler handler;
	private ScreenOrientation screenOrientation;
	private TextView txtLog;
	private ScrollView scrLog;

	private final ExecutorService threadPool = Executors.newCachedThreadPool();

	public enum ScreenOrientation {
		POSITION_0, POSITION_180
	}

	/* Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
        txtLog = (TextView)findViewById(R.id.txtLog);
        scrLog = (ScrollView) findViewById(R.id.scrLog);
		handler = new Handler();

	}

	public WebView getWebView() {
		return wv;
	}

	public void setScreenOrientation(ScreenOrientation screenOrientation) {
		this.screenOrientation = screenOrientation;
		final int rotation = (this.screenOrientation == ScreenOrientation.POSITION_0)? 0 : 180;
		
		
					handler.post(new Runnable() {

						@Override
						public void run() {
						
							megamipGui.setRotation(rotation);
							if (rotation == 0) {
								megamipGui.setScaleX(-1);
								megamipGui.setScaleY(1);
							} else {
								megamipGui.setScaleX(1);
								megamipGui.setScaleY(1);
							}
							

						}
					});

			

	}


    protected void log(final String str) {
            runOnUiThread(new Runnable() {

                    @Override
                    public void run() {
                            txtLog.append(str + "\n");
                            scrLog.smoothScrollTo(0, txtLog.getBottom());
                            Log.d("DroidGap - log:", " log " + str  );
                    }
            });

    }	
}