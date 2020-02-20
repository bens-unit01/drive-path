package com.example.touchpad01;

import java.net.URISyntaxException;
import java.util.Set;

import com.drivepath.rc.R;
import com.example.touchpad01.Robot.RobotListener;
import com.wowwee.util.SBBluetoothClient;
import com.wowwee.util.SBProtocol;
import com.wowwee.util.UartService;
import com.wowwee.util.SBBluetoothClient.SBBluetoothListener;
import com.wowwee.util.Utils;
import com.wowwee.views.JoystickView;
import com.wowwee.views.JoystickView.OnJoystickMoveListener;

import android.os.Bundle;
import android.os.Handler;
import android.preference.PreferenceManager;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ScrollView;
import android.widget.SeekBar;
import android.widget.Toast;
import android.widget.ToggleButton;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;

public class SwitchBotRC extends Activity implements OnClickListener {
	private static final int REQUEST_SELECT_DEVICE = 1;
	private static final int REQUEST_ENABLE_BT = 2;
	// private BluetoothDevice mDevice = null;
	// private BluetoothAdapter mBtAdapter = null;
	private boolean isDisconnectRequested = false;
	// private String mDeviceAdress = null;
	// private SBBluetoothClient mBtClient = null;
	private final int MAX_VALUE = 0x10;

	private boolean isLeftRight = false;
	private SeekBar seekBar;
	public static String ROBOT_IP = "10.10.250.173";
	private static final int RESULT_SETTINGS = 1;
	final String TAG = getClass().getSimpleName();
	private Button btnConnect;
	private ImageButton ledWifi;
	public BluetoothAdapter mBluetoothAdapter;
	private JoystickView joystick;

	private void log(final String str) {
		Log.d(TAG, " log " + str);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.switchbot_rc);

		btnConnect = (Button) findViewById(R.id.btnConnect_rc);
		// tglContinuesMode.setChecked(false);
		joystick = (JoystickView) findViewById(R.id.joystick);
		ledWifi = (ImageButton) findViewById(R.id.ledWifi_rc);
		seekBar = (SeekBar) findViewById(R.id.seekBar1);
		seekBar.setMax(100);
		seekBar.setProgress(50);
		// mBtAdapter = BluetoothAdapter.getDefaultAdapter();
		setListeners();
		initSettings();

		seekBar.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			boolean isSendMode = false;

			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
				seekBar.setProgress(50);
				isLeftRight = false;
				isSendMode = false;
				Log.d(TAG, " seekBar onStop ... ");
				
				String cmd = SBProtocol.JETTY_ROTATE
						+ SBProtocol.JETTY_SPLIT_CHAR;
				cmd += "0" + SBProtocol.JETTY_SPLIT_CHAR;
				cmd += "0" + SBProtocol.JETTY_SPLIT_CHAR;
				try {
					Thread.sleep(200);
				} catch (InterruptedException e) {
					Log.d(TAG, " bloc catch ...");
					e.printStackTrace();
				}

				if (isWsConnected()) {
		 			send(cmd);
	                Log.d(TAG, " seekBar " + cmd);
				}

			}

			@Override
			public void onStartTrackingTouch(final SeekBar seekBar) {
				new Thread(new Runnable() {

					@Override
					public void run() {
						isSendMode = true;
						while (isSendMode) {

							int progress = seekBar.getProgress();
			//				byte[] cmd = new byte[3];
							int power = 0, left_right = 0;
							if (progress == 50) {
								power = 0;
							} else {
								if (progress < 50) {
									power = 50 - progress;
								} else {
									power = progress;
								}
							}
							left_right = power;
							String cmd = SBProtocol.JETTY_ROTATE
									+ SBProtocol.JETTY_SPLIT_CHAR;
							cmd += "0" + SBProtocol.JETTY_SPLIT_CHAR;
							cmd += left_right + SBProtocol.JETTY_SPLIT_CHAR;

							if (isWsConnected()) {
					 			send(cmd);
				                Log.d(TAG, " seekBar " + cmd);
							}

							try {
								Thread.sleep(200);
							} catch (InterruptedException e) {
								Log.d(TAG, " bloc catch ...");
								e.printStackTrace();
							}
						}

					}
				}).start();
			}

			@Override
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				isLeftRight = true;
				//Log.d(TAG, " seekBar " + progress);
			}
		});

	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.settings, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {

		switch (item.getItemId()) {
		case R.id.menu_settings:
			Intent i = new Intent(this, UserSettingActivity.class);
			startActivityForResult(i, RESULT_SETTINGS);
			break;

		}
		return true;
	}

	private void initSettings() {

		SharedPreferences sharedPrefs = PreferenceManager
				.getDefaultSharedPreferences(this);
		ROBOT_IP = sharedPrefs.getString("prefHostname", "10.10.250.173");
		// mHost = "http://"+ mIp + ":8080/";

	}

	private void setListeners() {

		joystick.setOnJoystickMoveListener(new OnJoystickMoveListener() {

			@Override
			public void onValueChanged(int angle, int power, int direction) {

				updateValues(angle, power);
			}

		}, 200);

		btnConnect.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				if (btnConnect.getText().equals("Connect")) {
					// isDisconnectRequested = false;
					Robot.getInstance().connect();
				} else {
					// Disconnect button pressed
					// isDisconnectRequested = true;
					Robot.getInstance().disconnect();
				}

			}

		});

	}

	private void updateValues(int angle, int power) {

		if (isLeftRight)
			return; // control is for the slider, ignore joystick
		int fwd_bwd = 0, left_right = 0;

		if (angle <= 90 && angle >= -90) {
			// fwd_bwd = power * 0x20 / 100;
			fwd_bwd = power * MAX_VALUE / 100;
			if (angle >= 0) {
				left_right = getAngle(angle, 0x41);
				// log("forward right " + fwd_bwd + " " + left_right);
			} else {
				left_right = getAngle(-angle, 0x61);
				// log("forward left"+ fwd_bwd + " " + left_right);
			}
		} else {
			// fwd_bwd = (power * 0x20 /100) + 0x21;
			fwd_bwd = (power * MAX_VALUE / 100) + 0x21;
			if (angle <= -90) {
				left_right = getAngle(180 + angle, 0x61);
				// log("backward left"+ fwd_bwd + " " + left_right);
			} else {
				left_right = getAngle(180 - angle, 0x41);
				// log("backward right"+ fwd_bwd + " " + left_right);
			}
		}
		String cmd = SBProtocol.JETTY_DRIVE + SBProtocol.JETTY_SPLIT_CHAR;
		cmd += fwd_bwd + SBProtocol.JETTY_SPLIT_CHAR;
		cmd += left_right + SBProtocol.JETTY_SPLIT_CHAR;

		if (isWsConnected()) {
			send(cmd);
		}

	}

	private void send(final String cmd) {
		new Thread(new Runnable() {
			public void run() {
				Robot.getInstance().send(cmd);
			}
		}).start();
	}

	private int getAngle(int angle, int minValue) {
		// return angle * 0x1f / 90 + minValue;
		return angle * MAX_VALUE / 90 + minValue;
	}

	@Override
	public void onClick(View view) {

		String cmd = null;
		switch (view.getId()) {

		case R.id.btnKneel:
			cmd = SBProtocol.JETTY_KNEEL;
			break;
		case R.id.btnStandup:
			cmd = SBProtocol.JETTY_STAND_UP;
			break;
		case R.id.btnLeanForward:
			cmd = SBProtocol.JETTY_LEAN_FORWARD;
			break;
		case R.id.btnLeanbackward:
			cmd = SBProtocol.JETTY_LEAN_BACKWARD;
			break;
		case R.id.btnEStop:
			cmd = SBProtocol.JETTY_SET_ESTOP;
			break;
		case R.id.btnClrEStop:
			cmd = SBProtocol.JETTY_CLR_ESTOP;
			break;

		default:
			break;
		}

		if (cmd != null && Robot.getInstance().isConnected()) {
			cmd += SBProtocol.JETTY_SPLIT_CHAR;
			send(cmd);
		}
	}

	private boolean isWsConnected() {
		// return mBtClient.getState() ==
		// SBBluetoothClient.UART_PROFILE_CONNECTED;
		return Robot.getInstance().isConnected();
	}

	private boolean isBtConnected() {
		// return mBtClient.getState() ==
		// SBBluetoothClient.UART_PROFILE_CONNECTED;
		return false;
	}

	@Override
	protected void onPause() {
		super.onPause();
	}

	@Override
	public void onBackPressed() {
		;
		isDisconnectRequested = true;
		// mBtClient.disconnect();
		btnConnect.setText("Connect");
		this.finish();
	}

	@Override
	public void onResume() {
		super.onResume();

		Robot.getInstance().setRobotListener(new RobotListener() {

			@Override
			public void onNotify(String action, String data) {
				if (action.equals(Robot.ACTION_DATA_AVAILABLE)) {
					log("msg: " + data);
				}
				if (action.equals(Robot.ACTION_ROBOT_CONNECTED)) {
					log("connected");
					runOnUiThread(new Runnable() {
						public void run() {
							btnConnect.setText("Disconnect");
							ledWifi.setPressed(true);
						}
					});

				}
				if (action.equals(Robot.ACTION_ROBOT_DISCONNECTED)) {

					runOnUiThread(new Runnable() {
						public void run() {
							btnConnect.setText("Connect");
							ledWifi.setPressed(false);

						}
					});
					// displayStatus((byte) 0x00);
					// log("disconnected");
				}
			}
		});
		Log.d(TAG, "onResume");
		/*
		 * if (!mBtAdapter.isEnabled()) { Log.i(TAG,
		 * "onResume - BT not enabled yet"); Intent enableIntent = new Intent(
		 * BluetoothAdapter.ACTION_REQUEST_ENABLE);
		 * startActivityForResult(enableIntent, REQUEST_ENABLE_BT); }
		 */

		ledWifi.setPressed(false);
		if (isBtConnected()) {
			ledWifi.setPressed(true);
		}

	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		isDisconnectRequested = true;
		// mBtClient.onDestroy();

	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);

		switch (requestCode) {
		case RESULT_SETTINGS:
			SharedPreferences sharedPrefs = PreferenceManager
					.getDefaultSharedPreferences(this);
			ROBOT_IP = sharedPrefs.getString("prefHostname", "10.10.250.115");
			// mHost = "http://" + mIp + ":8080/";

			break;

		}
	}

}
