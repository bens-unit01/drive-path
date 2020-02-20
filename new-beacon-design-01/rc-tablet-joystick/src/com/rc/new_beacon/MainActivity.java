package com.rc.new_beacon;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import com.rc.new_beacon.R;
import com.rc.new_beacon.JoystickView.OnJoystickMoveListener;
import com.rc.new_beacon.SBBluetoothClient.SBBluetoothListener;

public class MainActivity extends Activity {
//    private native void nativeInit();     // Initialize native code, build pipeline, etc
//    private native void nativeFinalize(); // Destroy pipeline and shutdown native code
//    private native void nativePlay();     // Set pipeline to PLAYING
//    private native void nativePause();    // Set pipeline to PAUSED
    private static native boolean nativeClassInit(); // Initialize native class: cache Method IDs for callbacks
    private native void nativeSurfaceInit(Object surface);
    private native void nativeSurfaceFinalize();
    private long native_custom_data;      // Native code will use this to keep private data
	private static final int REQUEST_SELECT_DEVICE = 1;
	private static final int REQUEST_ENABLE_BT = 2;
	private BluetoothDevice mDevice = null;
	private BluetoothAdapter mBtAdapter = null;
	private boolean isDisconnectRequested = false;
	private String mDeviceAdress = null; 
	private SBBluetoothClient mBtClient = null; 
   
	private boolean is_playing_desired;   // Whether the user asked to go to PLAYING
    private JoystickView joystick; 
    private Button btnConnect;
	private ImageButton ledBluetooth;
    private static final String[] connectionState = {"not connected","listen", "connecting", "connected"};
    public static final int MESSAGE_LOG = 0;
    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
	private static final int MAX_VALUE = 16;
	private static final String TAG = "TOUCHPAD";
    
    

    // Called when the activity is first created.
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);


        setContentView(R.layout.main);
		ledBluetooth = (ImageButton)findViewById(R.id.btnLedBluetooth);
        btnConnect = (Button)findViewById(R.id.btnConnect);
        joystick = (JoystickView)findViewById(R.id.joystick); 
       
        setListeners();
    }

    protected void onSaveInstanceState (Bundle outState) {
        Log.d ("GStreamer", "Saving state, playing:" + is_playing_desired);
    }

    protected void onDestroy() {
      //  nativeFinalize();
        super.onDestroy();
        isDisconnectRequested = true;
		mBtClient.onDestroy();
    }
    
    @Override
	public void onResume() {
		super.onResume();
		log( "onResume");
		if (!mBtAdapter.isEnabled()) {
			log( "onResume - BT not enabled yet");
			Intent enableIntent = new Intent(
					BluetoothAdapter.ACTION_REQUEST_ENABLE);
			startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
		}
		
		ledBluetooth.setPressed(false);
		if(isBtConnected()) {
			ledBluetooth.setPressed(true);
		}

	}
    
    private void setListeners() {
		
		
		mBtAdapter = BluetoothAdapter.getDefaultAdapter();
    	joystick.setOnJoystickMoveListener(new OnJoystickMoveListener() {
    		
    		@Override
    		public void onValueChanged(int angle, int power, int direction) {
    			
    			updateValues(angle, power);
    		}


    	}, 50);	
    	
    	mBtClient = new SBBluetoothClient(this);
    	mBtClient.addSBBluetoothListener(new SBBluetoothListener() {

    		@Override
    		public void onNotify(String action, byte[] data) {
    			if (action.equals(UartService.ACTION_DATA_AVAILABLE)) {
    				if(data.length < 2) return;  // we need at least 2 bytes   
    				displayStatus(data);

    			}

    			if (action.equals(UartService.ACTION_GATT_CONNECTED)) {
    				//log(mDevice.getName() + " - ready");
    				ledBluetooth.setPressed(true);
    				btnConnect.setText("Disconnect");
    			}

    			if (action.equals(UartService.ACTION_GATT_DISCONNECTED)) {
    				//log(" Disconnected from: " + mDevice.getName());
    				btnConnect.setText("Connect");
    				ledBluetooth.setPressed(false);
    			// we reconnect	
    				if(!isDisconnectRequested)
    					mBtClient.connect(mDeviceAdress);
    			}
    			
    			Log.i("MainActivity", "onNotify action: " + action  + " mState: " + mBtClient.getState());

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

    	}
    
    private void updateValues(int angle, int power) {
   	 
//		int fwd_bwd = 0, left_right = 0;
		byte[] cmd = new byte[3];
//        
//		if( angle <= 90 && angle >= -90){
//        	fwd_bwd = power * 0x20 / 100;
//        	if(angle >= 0 ){
//        	    left_right  = getAngle(angle, 0x41);	
////        		log("forward right " + fwd_bwd + " " + left_right);
//        	}else {
//        	    left_right  = getAngle(-angle, 0x61);	
// //       		log("forward left"+ fwd_bwd + " " + left_right);
//        	}
//        }else{
//        	fwd_bwd = (power * 0x20 /100) + 0x21;
//        	if(angle <= -90){
//        	    left_right  = getAngle(180 + angle, 0x61);	
//  //      		log("backward left"+ fwd_bwd + " " + left_right);
//        	}else {
//        	    left_right  = getAngle(180 - angle, 0x41);	
//   //     		log("backward right"+ fwd_bwd + " " + left_right);
//        	}
//        }
		int fwd_bwd = 0, left_right = 0;

		if (angle <= 90 && angle >= -90) {
			fwd_bwd = power * 0x20 / 100;
			fwd_bwd = power * MAX_VALUE / 100;
            
          if( power > 20)      
			if (angle >= 0) {
				left_right = getAngle(angle, 0x41);
				log("forward right " + fwd_bwd + " " + left_right);
			} else {
				left_right = getAngle(-angle, 0x61);
				log("forward left" + fwd_bwd + " " + left_right);
			}
		} else {
			fwd_bwd = (power * 0x20 / 100) + 0x21;
			fwd_bwd = (power * MAX_VALUE / 100) + 0x21;

			if( power > 20)      
			if (angle <= -90) {
				left_right = getAngle(180 + angle, 0x61);
				log("backward left" + fwd_bwd + " " + left_right);
			} else {
				left_right = getAngle(180 - angle, 0x41);
				log("backward right" + fwd_bwd + " " + left_right);
			}
		}                                                             
                                                                              
		cmd[0] = (byte) 0x81;
		cmd[1] = (byte) fwd_bwd;
		cmd[2] = (byte) left_right;
		
		Log.d(TAG, " " + fwd_bwd + " " + left_right); 
		
		if(isBtConnected()) {
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
		Log.d("rc-new-beacon#MainActivity", " log " + str  );
    }
    
    
    @Override
	public void onBackPressed() {;
	    isDisconnectRequested = true;
		mBtClient.disconnect();
		btnConnect.setText("Connect");
		this.finish();
	}
	

	public void logOnScreen(final String str) {
	}

	private void displayStatus(final byte[] data) {
		// b0 --> usb, b1 --> wifi, b2 --> telep.
	}


}
