package com.megamip.voice;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import com.hoho.android.usbserial.driver.UsbSerialDriver;
import com.hoho.android.usbserial.driver.UsbSerialProber;
import com.hoho.android.usbserial.util.SerialInputOutputManager;

import android.content.Context;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.util.Log;

public class SBRealDevice implements SBUsbDevice {

	// final String TAG = getClass().getSimpleName();
	final String TAG = "SwitchBot";
	public static final int FTDI_PRODUCT_ID = 24577; // 0x6001
	private final ExecutorService mExecutor = Executors
			.newSingleThreadExecutor();
	private Context mContext;
	private UsbManager mUsbManager;
	private UsbSerialDriver mUsbDriver;
	private SerialInputOutputManager mSerialIoManager;
	private ArrayList<UsbListener> mListeListeners = new ArrayList<UsbListener>();
	private static SBRealDevice mUsbDevice;
	public static final int MIN_SIZE = 3;
	private boolean eStopFlag = true;
	private boolean isListening = false;	
	
	private final SerialInputOutputManager.Listener mListener = new SerialInputOutputManager.Listener() {

		@Override
		public void onRunError(Exception e) {
			Log.d(TAG, "UsbDevice onRunError " + e.getMessage());
		}

		@Override
		public void onNewData(final byte[] data) {
			
			
         // Log.d(TAG, " data: " + Utils.bytesToHex2(data));
          if(isListening) return;  
        	synchronized (this) {
			isListening = true;
		   }	
	      byte[] out = {SBProtocol.START_BYTE, data[0], SBProtocol.END_BYTE};	
			for (UsbListener b : mListeListeners) {
				b.onNotify(new UsbEvent(this, out));
			}
			
			 
			// ignoring push button events for 2 seconds
			new Thread(new Runnable() {

				@Override
				public void run() {
					try {
						Thread.sleep(2000);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					synchronized (this) {
						isListening = false;
					}

				}
			}).start();

		}

	};
	
	/**
	 * This method is used to get an instance of the SBRealDevice according
	 * to the singleton design pattern. This instance represent the FTDI board 
	 * connected to the nordic board pca10001
	 * @param context : the calling activity context
	 * @return
	 */

	public static SBRealDevice getInstance(Context context) {

		if (mUsbDevice != null) {
			return mUsbDevice;
		} else {

			mUsbDevice = new SBRealDevice(context);
			return mUsbDevice;
		}

	}
	
	
    /**
     * private constructor 
     * @param context : the activity context
     */
	private SBRealDevice(Context context ) {
		super();
		mContext = context;
		mUsbManager = (UsbManager) context
				.getSystemService(Context.USB_SERVICE);

		HashMap<String, UsbDevice> deviceList = mUsbManager.getDeviceList();
		UsbDevice usbDevice = null;
		for (UsbDevice element : deviceList.values()) {

			if (FTDI_PRODUCT_ID == element.getProductId()) {

						usbDevice = element;
					
			}
		}

		try {
			mUsbDriver = UsbSerialProber.acquire(mUsbManager, usbDevice);
			mUsbDriver.open();
			int baudrate = 115200;
			mUsbDriver.setParameters(baudrate, UsbSerialDriver.DATABITS_8,
					UsbSerialDriver.STOPBITS_1, UsbSerialDriver.PARITY_NONE);
			mUsbDriver.setDTR(true);

		} catch (IOException e) {

			try {
				mUsbDriver.close();
			} catch (IOException e1) {

				e1.printStackTrace();
			}
			e.printStackTrace();
			Log.e(TAG,
					"MipUsbDevice - constructor - bloc catch ex = "
							+ e.getMessage());
		} catch (NullPointerException e) {

			e.printStackTrace();
			Log.e(TAG,
					"MipUsbDevice - constructor - bloc catch ex = "
							+ e.getMessage());
		}

		stopIoManager();
		startIoManager();
	}

	/**
	 * method used to add a listener to handle the received data from the nordic board board
	 */
	@Override
	public void addUsbListener(UsbListener usbListener) {
		mListeListeners.add(usbListener);
	}

	/**
	 * method used to send data to the nuveton board through the nordic board  
	 * the data is processed according to the SwitchBot protocol, see SBProtocol.java
	 */
	@Override
	public void write(byte[] data) {

		byte cmd = data[0];
		if (cmd <= 0x31) { // encoding the command before sending it to the MCU
			byte[] encodedCommand = null;
			int nbSteps = 0;
			switch (cmd) {
			case SBProtocol.MOVE_FORWARD:
				encodedCommand = SBProtocol.ENCODED_DRIVE_FORWARD;
				nbSteps = 10;
				break;
			case SBProtocol.MOVE_BACKWARD:
				encodedCommand = SBProtocol.ENCODED_DRIVE_BACKWARD;
				nbSteps = 10;
				break;
			case SBProtocol.TURN_LEFT:
				encodedCommand = SBProtocol.ENCODED_TURN_LEFT;
				nbSteps = 4;
				break;
			case SBProtocol.TURN_RIGHT:
				encodedCommand = SBProtocol.ENCODED_TURN_RIGHT;
				nbSteps = 4;
				break;
			case SBProtocol.LEAN_FORWARD:
				encodedCommand = SBProtocol.ENCODED_LEAN_FORWARD;
				nbSteps = 4;
				break;
			case SBProtocol.LEAN_BACKWARD:
				encodedCommand = SBProtocol.ENCODED_LEAN_BACKWARD;
				nbSteps = 4;
				break;

			}

			if (encodedCommand == null)
				return;              // not supported command code
            final byte[] cmdOut = encodedCommand;
            final int steps = nbSteps;
			new Thread(new Runnable() {

				@Override
				public void run() {

					Log.d(TAG, "handling motion command ");
					//int nbSteps = SBProtocol.DRIVE_NB_STEPS;
					for (int i = 0; i < steps; i++) {
						try {
							mUsbDriver.write(cmdOut, 1000);
							Thread.sleep(200);
						} catch (InterruptedException e) {
							e.printStackTrace();
							Log.d(TAG, " write - block catch" + e.getMessage());
						} catch (Exception e) {
							Log.d(TAG, " write - block catch" + e.getMessage());
						}
					}
				   // stop command	
					try {
						mUsbDriver.write(new byte[]{SBProtocol.DRIVE, 0, 0}, 1000);
					} catch (IOException e) {
						e.printStackTrace();
					}

				}
			}).start();

		} else {  // the other commands are sent directly without encoding
		    byte[] dataOut = new byte[3];
		    dataOut[0] = cmd; dataOut[1] = 0; dataOut[2] = 0;
		    
			if(cmd == SBProtocol.DANCE){
				byte out = (eStopFlag)?SBProtocol.CLEAR_ESTOP:SBProtocol.ESTOP;
			    dataOut[0] = out;
			    eStopFlag = !eStopFlag;
			}
			
			writeRaw(dataOut);
		}

	}
   
	/**
	 * this method is not used since we have only one board 
	 */
	@Override
	public boolean isAllUsbConnected(Context context) {
		// TODO Auto-generated method stub
		return false;
	}
	
	/**
	 * this method sends data to the nordic board
	 */
	@Override
	public void writeRaw(byte[] data) {
		try {
			mUsbDriver.write(data, 1000);
		} catch (Exception e) {

			Log.d(TAG,
					"MipUsbDevice - write - block catch ex:" + e.getMessage());
			e.printStackTrace();
		}
	}

	private void stopIoManager() {
		if (mSerialIoManager != null) {
			Log.i(TAG, "Stopping io manager ..");
			mSerialIoManager.stop();
			mSerialIoManager = null;
		}
	}

	private void startIoManager() {
		if (mUsbDriver != null) {
			Log.i(TAG, "Starting io manager ..");
			mSerialIoManager = new SerialInputOutputManager(mUsbDriver,
					mListener);
			mExecutor.submit(mSerialIoManager);
			Log.d(TAG, "UsbDevice 6");
		}
	}
	
	
    /**
     * Method to disconnect the USB device 
     */
	@Override
	public void disconnect(DeviceType deviceType) {

			mUsbDevice = null;
			stopIoManager();
	}
   
	/**
	 * Method to check of the usb device is connected 
	 * @param context : the calling activity context 
	 * @return  true if the device is connected, false if not 
	 */
	public static boolean isConnected(Context context ) {
		UsbManager usbManager = (UsbManager) context
				.getSystemService(Context.USB_SERVICE);

		HashMap<String, UsbDevice> deviceList = usbManager.getDeviceList();

		try {
			for (UsbDevice element : deviceList.values()) {
				if (FTDI_PRODUCT_ID == element.getProductId()) {
	
						return true;
				}

			}
		} catch (NullPointerException ex) {
			ex.printStackTrace();
		}

		return false;
	}

	/**
	 * this method is not used since we have only one device 
	 */
	@Override
	public boolean isDeviceConnected(Context context, DeviceType deviceType) {
		// TODO Auto-generated method stub
		return false;
	}

}
