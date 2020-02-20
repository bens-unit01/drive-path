package com.wowwee.touchpad;

import java.net.URISyntaxException;
import java.util.Set;

import com.wowwee.util.SBProtocol;

import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;
import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Intent;

public class MainActivity extends Activity implements OnClickListener {

	final String TAG = getClass().getSimpleName();
	private Button btnRc, btnShe, btnStatus, btnTelepresence;
	private final String SHE_PACKAGE_NAME = "com.godog.godogfetch";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

	}

	@Override
	protected void onPause() {
		super.onPause();

	}

	@Override
	protected void onResume() {
		super.onResume();

	}

	@Override
	public void onClick(View v) {
		Intent intent;
		switch (v.getId()) {

		case R.id.btnRC:
		 	if (isRightOS()) {
				intent = new Intent(this, SwitchBotRC.class);
				startActivity(intent);
			}
			break;
		case R.id.btnShe:
			intent = MainActivity.this.getPackageManager().getLaunchIntentForPackage(SHE_PACKAGE_NAME);
		    startActivity(intent);	
			break;
		case R.id.btnStatus:
			if (isRightOS()) {
				intent = new Intent(this, SwitchBotStatus.class);
				startActivity(intent);
			}

			break;
		case R.id.btnTelepresence:
			try {
				intent = Intent.parseUri("telepresenceapp://",
						Intent.URI_INTENT_SCHEME);
				startActivity(intent);
			} catch (URISyntaxException e) {
				e.printStackTrace();
			} catch (ActivityNotFoundException e) {
				e.printStackTrace();
			}
			break;

		}

	}

	private boolean isRightOS() {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR2) {
			return true;
		} else {
			Toast t = Toast.makeText(this,
					"This activity uses BLE, you need Android 4.3 or higher",
					Toast.LENGTH_LONG);
			t.show();
			return false;
		}
	}

}
