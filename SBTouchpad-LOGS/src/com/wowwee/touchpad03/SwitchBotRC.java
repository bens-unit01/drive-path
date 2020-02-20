package com.wowwee.touchpad03;



import java.net.URISyntaxException;
import java.util.Set;

import com.wowwee.touchpad03.R;
import com.wowwee.util.SBBluetoothClient;
import com.wowwee.util.SBProtocol;
import com.wowwee.util.UartService;
import com.wowwee.util.SBBluetoothClient.SBBluetoothListener;
import com.wowwee.util.Utils;
import com.wowwee.views.JoystickView;
import com.wowwee.views.JoystickView.OnJoystickMoveListener;

import android.os.Bundle;
import android.os.Handler;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
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

public class SwitchBotRC extends Activity  implements OnClickListener {
	private static final int REQUEST_SELECT_DEVICE = 1;
	private static final int REQUEST_ENABLE_BT = 2;
	private BluetoothDevice mDevice = null;
	private BluetoothAdapter mBtAdapter = null;
	private boolean isDisconnectRequested = false;
	private String mDeviceAdress = null; 
	private SBBluetoothClient mBtClient = null;
	private final int MAX_VALUE = 0x10;

	final String TAG = getClass().getSimpleName();
	private Button  btnConnect;
	private ImageButton ledBluetooth;
	public BluetoothAdapter mBluetoothAdapter;
    private JoystickView joystick; 
    
	private void log(final String str) {
				Log.d(TAG, " log " + str  );
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.switchbot_rc);
		
		btnConnect = (Button)findViewById(R.id.btnConnect);
		//tglContinuesMode.setChecked(false);
		joystick = (JoystickView)findViewById(R.id.joystick);
		ledBluetooth = (ImageButton)findViewById(R.id.btnLedBluetooth);
		mBtAdapter = BluetoothAdapter.getDefaultAdapter();
		setListeners();
		
		 

	}

	private void setListeners() {
		
		
	joystick.setOnJoystickMoveListener(new OnJoystickMoveListener() {
		
		@Override
		public void onValueChanged(int angle, int power, int direction) {
			
			updateValues(angle, power);
		}


	}, 200);	
	
	mBtClient = new SBBluetoothClient(this);
	mBtClient.addSBBluetoothListener(new SBBluetoothListener() {

		@Override
		public void onNotify(String action, byte[] data) {
			if (action.equals(UartService.ACTION_DATA_AVAILABLE)) {
				if(data.length < 2) return;  // we need at least 2 bytes   
				//displayStatus(data[1]);
                Log.i(TAG, "data: " + Utils.bytesToHex2(data));
			}

			if (action.equals(UartService.ACTION_GATT_CONNECTED)) {
				log(mDevice.getName() + " - ready");
				ledBluetooth.setPressed(true);
				btnConnect.setText("Disconnect");
			}

			if (action.equals(UartService.ACTION_GATT_DISCONNECTED)) {
				log(" Disconnected from: " + mDevice.getName());
				btnConnect.setText("Connect");
				ledBluetooth.setPressed(false);
			// we reconnect	
				if(!isDisconnectRequested)
					mBtClient.connect(mDeviceAdress);
			}
			
			Log.i(TAG, "onNotify action: " + action  + " mState: " + mBtClient.getState());

		}

	});
    

	btnConnect.setOnClickListener(new OnClickListener() {

		@Override
		public void onClick(View v) {

			if (!mBtAdapter.isEnabled()) {
				Log.i(TAG, "onClick - BT not enabled yet");
				Intent enableIntent = new Intent(
						BluetoothAdapter.ACTION_REQUEST_ENABLE);
				startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
			} else {
				if (btnConnect.getText().equals("Connect")) {

					// Connect button pressed, open DeviceListActivity
                    isDisconnectRequested = false;
					Intent newIntent = new Intent(SwitchBotRC.this,
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

	}
	
	private void updateValues(int angle, int power) {
	 
		int fwd_bwd = 0, left_right = 0;
		byte[] cmd = new byte[3];
        
		if( angle <= 90 && angle >= -90){
        	// fwd_bwd = power * 0x20 / 100;
			   fwd_bwd = power * MAX_VALUE / 100;
        	if(angle >= 0 ){
        	    left_right  = getAngle(angle, 0x41);	
//        		log("forward right " + fwd_bwd + " " + left_right);
        	}else {
        	    left_right  = getAngle(-angle, 0x61);	
 //       		log("forward left"+ fwd_bwd + " " + left_right);
        	}
        }else{
//        	fwd_bwd = (power * 0x20 /100) + 0x21;
        	fwd_bwd = (power * MAX_VALUE/100) + 0x21;
        	if(angle <= -90){
        	    left_right  = getAngle(180 + angle, 0x61);	
  //      		log("backward left"+ fwd_bwd + " " + left_right);
        	}else {
        	    left_right  = getAngle(180 - angle, 0x41);	
   //     		log("backward right"+ fwd_bwd + " " + left_right);
        	}
        }

		cmd[0] = SBProtocol.DRIVE;
		cmd[1] = (byte) fwd_bwd;
		cmd[2] = (byte) left_right;
		
		if(isBtConnected()) {
		   mBtClient.writeRX(cmd);	
		}           
		
	}
	private int getAngle(int angle, int minValue) {
		//return angle * 0x1f / 90 + minValue;  
		return angle * MAX_VALUE/ 90 + minValue;  
	}

	@Override
	public void onClick(View view) {
		
	byte[] cmd = null;	
      switch (view.getId()) {
		
		case R.id.btnKneel:
			byte[] tmp6 = {SBProtocol.KNEEL, 0, 0};
			cmd = tmp6; 
			break;
		case R.id.btnStandup:
			byte[] tmp7 = { SBProtocol.STAND_UP, 0, 0 }; 
			cmd = tmp7; 
			break;
		case R.id.btnLeanForward:
			byte[] tmp8 = SBProtocol.ENCODED_LEAN_FORWARD; 
			cmd = tmp8; 
			break;
		case R.id.btnLeanbackward:
			byte[] tmp9 =  SBProtocol.ENCODED_LEAN_BACKWARD ; 
			cmd = tmp9;
			break;
		case R.id.btnEStop:
			byte[] tmp10 = {SBProtocol.ESTOP, 0, 0}; 
			cmd = tmp10;
			break;
		case R.id.btnClrEStop:
			byte[] tmp11 = {SBProtocol.CLEAR_ESTOP, 0, 0}; 
			cmd = tmp11;
			break;

	default:
		break;
	}
      
      if(isBtConnected() && cmd != null){
    	  mBtClient.writeRX(cmd);
      }
		
	}
	
	private boolean isBtConnected() {
		return mBtClient.getState() == SBBluetoothClient.UART_PROFILE_CONNECTED;
	}

	
	@Override
	protected void onPause() {
		super.onPause();
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
	
	
	@Override
	public void onBackPressed() {;
	    isDisconnectRequested = true;
		mBtClient.disconnect();
		btnConnect.setText("Connect");
		this.finish();
	}
	
	@Override
	public void onResume() {
		super.onResume();
		Log.d(TAG, "onResume");
		if (!mBtAdapter.isEnabled()) {
			Log.i(TAG, "onResume - BT not enabled yet");
			Intent enableIntent = new Intent(
					BluetoothAdapter.ACTION_REQUEST_ENABLE);
			startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
		}
		
		ledBluetooth.setPressed(false);
		if(isBtConnected()) {
			ledBluetooth.setPressed(true);
		}

	}
	
	@Override
	public void onDestroy() {
		super.onDestroy();
		isDisconnectRequested = true;
		mBtClient.onDestroy();

	}


}
