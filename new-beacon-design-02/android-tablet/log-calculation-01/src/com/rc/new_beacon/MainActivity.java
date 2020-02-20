package com.rc.new_beacon;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Arrays;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.content.Intent;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.ImageButton;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import com.google.gson.Gson;
import com.rc.new_beacon.R;
import com.rc.new_beacon.JoystickView.OnJoystickMoveListener;
import com.rc.new_beacon.SBBluetoothClient.SBBluetoothListener;

public class MainActivity extends Activity {

	private static native boolean nativeClassInit(); // Initialize native class:

    private EditText txtLabel; 	
	private TextView txtRange;
	private TextView txtHeading;
	private TextView txtPosAngle, txtXY;
	private ScrollView scrLog;
	private static final int REQUEST_SELECT_DEVICE = 1;
	private static final int REQUEST_ENABLE_BT = 2;
	private BluetoothDevice mDevice = null;
	private BluetoothAdapter mBtAdapter = null;
	private boolean isDisconnectRequested = false;
	private String mDeviceAdress = null;
	private SBBluetoothClient mBtClient = null;
    private final int MAX_VALUE = 0x10;
	private boolean is_playing_desired; // Whether the user asked to go to
										// PLAYING
	private JoystickView joystick;
	private PlanView planView;
	private Button btnConnect, btnSave;
	private ImageButton ledBluetooth;
	private static final String[] connectionState = { "not connected",
			"listen", "connecting", "connected" };
	public static final int MESSAGE_LOG = 0;
	public static final int MESSAGE_STATE_CHANGE = 1;
	public static final int MESSAGE_READ = 2;
	public static final int MESSAGE_WRITE = 3;
	private static final String TAG = "joystick";
    private DecimalFormat floatFormat =  new DecimalFormat("###.##"); 
	private GridView gridView;
	ArrayList<String> data = new ArrayList<String>();
	// var pour le calcul
	static double PI = 3.14159265;
	static float radian = (float) PI / 180;

	int[] angle = { 90, 180, 270, 0 };
	int[] emitter = { 0, 18, 36, 54, 72, 90, 108, 126, 144, 162 };

	float[] ex = new float[10];
	float[] ey = new float[10];
	float[] rx = new float[10];
	float[] ry = new float[10];
	private GridViewCustomAdapter adapter;
	
	public static String FILE_NAME;
    private LogDataList mLogDataList; 
	private int[][] buffer = new int[9][4];

	// Called when the activity is first created.
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
//		setContentView(new PlanView(this)); 
		mLogDataList = new LogDataList(); 
        FILE_NAME = "mnt/sdcard/logs/dp_logs.json"; 
        File file = new File(FILE_NAME); 
		ledBluetooth = (ImageButton) findViewById(R.id.btnLedBluetooth);
		btnConnect = (Button) findViewById(R.id.btnConnect);
		btnSave = (Button) findViewById(R.id.btnSave);
		txtLabel = (EditText) findViewById(R.id.txtLabel);
		txtRange = (TextView) findViewById(R.id.txtRange);
		txtHeading = (TextView) findViewById(R.id.txtHeading);
		txtPosAngle = (TextView) findViewById(R.id.txtPosAngle);
		txtXY = (TextView) findViewById(R.id.txtXY);
    	 joystick = (JoystickView)findViewById(R.id.joystick);
    	 planView = (PlanView) findViewById(R.id.planView);

		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 4; j++)
				data.add("0");
		}

		adapter = new GridViewCustomAdapter(this, data);
		gridView = (GridView) findViewById(R.id.grid_view);
		gridView.setAdapter(adapter);

		setListeners();
		initCalc();

	}
	
	
    

	protected void onSaveInstanceState(Bundle outState) {
		Log.d("GStreamer", "Saving state, playing:" + is_playing_desired);
		outState.putBoolean("playing", is_playing_desired);
	}

	protected void onDestroy() {
		// nativeFinalize();
		super.onDestroy();
		isDisconnectRequested = true;
		mBtClient.onDestroy();
	}

	@Override
	public void onResume() {
		super.onResume();
		log("onResume");

	}

	private void setListeners() {

		btnSave.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {

				LogData logData = new LogData(txtLabel.getText().toString(),
						txtRange.getText().toString(), txtHeading.getText()
								.toString(), txtPosAngle.getText().toString(),
						data);
			    mLogDataList.addLog(logData); 
				Gson gson = new Gson();
				final String logString = gson.toJson(mLogDataList);

				try {
					FileWriter file = new FileWriter(FILE_NAME);
					file.write(logString);
					file.flush();
					file.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		});
		
		mBtAdapter = BluetoothAdapter.getDefaultAdapter();

		mBtClient = new SBBluetoothClient(this);
		mBtClient.addSBBluetoothListener(new SBBluetoothListener() {

			@Override
			public void onNotify(String action, byte[] data) {
				if (action.equals(UartService.ACTION_DATA_AVAILABLE)) {
					if (data.length < 2)
						return; // we need at least 2 bytes
					displayStatus(data);

				}

				if (action.equals(UartService.ACTION_GATT_CONNECTED)) {
					// log(mDevice.getName() + " - ready");
					ledBluetooth.setPressed(true);
					btnConnect.setText("Disconnect");
				}

				if (action.equals(UartService.ACTION_GATT_DISCONNECTED)) {
					// log(" Disconnected from: " + mDevice.getName());
					btnConnect.setText("Connect");
					ledBluetooth.setPressed(false);
					// we reconnect
					if (!isDisconnectRequested)
						mBtClient.connect(mDeviceAdress);
				}

				Log.i("MainActivity", "onNotify action: " + action
						+ " mState: " + mBtClient.getState());

			}

		});

		btnConnect.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {

				if (!mBtAdapter.isEnabled()) {
					Log.i("MainActivity", "onClick - BT not enabled yet");
					Intent enableIntent = new Intent(
							BluetoothAdapter.ACTION_REQUEST_ENABLE);
					startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
				} else {
					if (btnConnect.getText().equals("Connect")) {

						// Connect button pressed, open DeviceListActivity
						isDisconnectRequested = false;
						Intent newIntent = new Intent(MainActivity.this,
								DeviceListActivity.class);
						startActivityForResult(newIntent, REQUEST_SELECT_DEVICE);
					} else {
						// Disconnect button pressed
						isDisconnectRequested = true;
						if (mDevice != null) {
							mBtClient.disconnect();
						}
					}
				}

			}
		});

		 joystick.setOnJoystickMoveListener(new OnJoystickMoveListener() {
		
		 @Override
		 public void onValueChanged(int angle, int power, int direction) {
		
		 updateValues(angle, power);
		 }
		
		
		 }, 200);

	}

	private void updateValues(int angle, int power) {

		int fwd_bwd = 0, left_right = 0;
		byte[] cmd = new byte[3];

		if (angle <= 90 && angle >= -90) {
			fwd_bwd = power * 0x20 / 100;
		    fwd_bwd = power * MAX_VALUE / 100; 
		
		    if( power > 20) 
			if (angle >= 0) {
				left_right = getAngle(angle, 0x41);
			} else {
				left_right = getAngle(-angle, 0x61);
			}
		} else {
			fwd_bwd = (power * 0x20 / 100) + 0x21;
			fwd_bwd = (power * MAX_VALUE/100) + 0x21;

			if( power > 20) 
			if (angle <= -90) {
				left_right = getAngle(180 + angle, 0x61);
			} else {
				left_right = getAngle(180 - angle, 0x41);
			}
		}
		
		Log.d(TAG, " " + angle + " " + power + " " + fwd_bwd + " " + left_right ); 

		cmd[0] = (byte) 0x81;
		cmd[1] = (byte) fwd_bwd;
		cmd[2] = (byte) left_right;

		if (isBtConnected()) {
			mBtClient.writeRX(cmd);
		}

	}

	private boolean isBtConnected() {
		return mBtClient.getState() == SBBluetoothClient.UART_PROFILE_CONNECTED;
	}

	private int getAngle(int angle, int minValue) {
		return angle * 0x1f / 90 + minValue;
	}


	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {

		switch (requestCode) {

		case REQUEST_SELECT_DEVICE:
			// When the DeviceListActivity return, with the selected device
			// address
			if (resultCode == Activity.RESULT_OK && data != null) {
				String deviceAddress = data
						.getStringExtra(BluetoothDevice.EXTRA_DEVICE);
				mDeviceAdress = deviceAddress;
				mDevice = BluetoothAdapter.getDefaultAdapter().getRemoteDevice(
						deviceAddress);

				// log("... onActivityResultdevice.address==" + mDevice);
				log(mDevice.getName() + "- connecting ...");

				mBtClient.connect(deviceAddress);
			}
			break;
		case REQUEST_ENABLE_BT:
			// When the request to enable Bluetooth returns
			if (resultCode == Activity.RESULT_OK) {
				Toast.makeText(this, "Bluetooth has turned on ",
						Toast.LENGTH_SHORT).show();

			} else {
				// User did not enable Bluetooth or an error occurred
				log("BT not enabled");
				Toast.makeText(this, "Problem in BT Turning ON ",
						Toast.LENGTH_SHORT).show();
				finish();
			}
			break;
		default:
			log("wrong request code");
			break;
		}

	}

	private void log(final String str) {
		Log.d("rc-new-beacon#MainActivity", " log " + str);
	}

	@Override
	public void onBackPressed() {
		;
		isDisconnectRequested = true;
		mBtClient.disconnect();
		btnConnect.setText("Connect");
		this.finish();
	}

	private void displayStatus(final byte[] data) {

		runOnUiThread(new Runnable() {

			@Override
			public void run() {

				Log.d("DATA-IN ", Utils.bytesToHex2(data));
				if (data.length < 10)
					return;

				if (data[0] == 0 || data[0] == 1) {

					if (data[0] == 1) {

						displayCoordinates();

						for (int i = 0; i < 9; i++)
							for (int j = 0; j < 4; j++) {
								buffer[i][j] = 0;
							}
					}
					int index = data[1];
					if (index > 8 || index < 0)
						return;

					int lsb = 0, msb = 0;
					int k = 2;
					for (int i = 0; i < 4; i++) {
						lsb = (int) (0xFF & data[k]);
						k++;
						msb = (int) (0xFF & data[k]);
						k++;
						msb = (int) (msb << 8);
						msb = (int) (lsb | msb);
						buffer[index][i] = msb;
					}
				}

			}
		});
	}

	void displayCoordinates() {

		float sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;

		for (int i = 0; i < 4; i++) {
			for (int j = 1; j < 10; j++) {
				sum1 += ex[j] * (buffer[j - 1][i]);
				sum2 += ey[j] * (buffer[j - 1][i]);
				sum3 += rx[i] * (buffer[j - 1][i]);
				sum4 += ry[i] * (buffer[j - 1][i]);

			}
		}

		int k = 0;
        int[] max = new int[]{0, 0, 0, 0};  
		for (int i = 0; i < 9; i++) {
			// Log.d("DATA-IN", " " + buffer[i][0] + buffer[i][1] + buffer[i][2]
			// + buffer[i][3] );
			for (int j = 0; j < 4; j++) {
				data.set(k + j, "" + (buffer[i][j]));
				if (buffer[i][j] > max[j]) {
					max[j] = buffer[i][j];
				}
			}
			k += 4;
		}
		adapter.notifyDataSetChanged();

		float Ex = sum1, Ey = sum2, Rx = sum3, Ry = sum4;
		float phi = (float) Math.atan2(Ex, Ey);
		float theta = (float) Math.atan2(Rx, Ry);
//		float range = (float) Math.sqrt((Rx * Rx) + (Ry * Ry));
//      int cal_range = (int)(100 - (range/100)); 
//      float cal_range = (-0.02932F * range) + 358.2F; 
//	    float cal_range = (-0.02289F * range) + 355.4F;
	    Log.d(TAG, " max " + max[0] + " " +max[1] + " "+ max[2] + " " + max[3]); 	
		int range = max[0] + max[1] + max[2] + max[3]; 
		float x = (float)(range * Math.cos( PI - phi));  
		float y =  (float)((range * Math.sin( PI - phi)));  

		phi = phi / radian;
		theta = theta / radian;

        String sx, sy;
        sx = floatFormat.format(x);
        sy = floatFormat.format(y);
		txtRange.setText(" " + (range));
		txtHeading.setText(" " + ((int) theta));
		txtPosAngle.setText(" " + ((int) phi));
		txtXY.setText(" " + sx + " _  " + sy);
		planView.setCoords((int)x, (int)y);
		
	}

	void initCalc() {

		for (int i = 1; i < 10; i++) {
			ex[i] = (float) Math.sin(radian * emitter[i]);
			ey[i] = (float) Math.cos(radian * emitter[i]);
		}

		for (int i = 0; i < 4; i++) {
			rx[i] = (float) Math.sin(radian * angle[i]);
			ry[i] = (float) Math.cos(radian * angle[i]);
		}

	}

}
