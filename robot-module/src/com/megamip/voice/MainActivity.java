package com.megamip.voice;


import java.io.IOException;
import java.util.ArrayList;
import java.util.LinkedList;

import org.json.JSONStringer;
import org.json.simple.JSONValue;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbManager;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.util.Log;

import com.google.gson.Gson;
import com.google.gson.JsonSyntaxException;
import com.megamip.drivepath.DPMap;
import com.megamip.drivepath.Node;
import com.megamip.util.AdbUtils;
import com.megamip.util.JettyServer;
import com.megamip.util.JettyServer.JettyListener;
import com.megamip.util.JettyServer.ServerEvent;
import com.megamip.util.Utils;
import com.megamip.view.DroidGap;
import com.megamip.voice.SBUsbDevice.DeviceType;
import com.megamip.voice.SBUsbDevice.UsbEvent;
import com.megamip.voice.SBUsbDevice.UsbListener;
import com.ww.websocket_server.CommHandler;
import com.ww.websocket_server.CommServer;



public class MainActivity extends DroidGap {
    private final String TAG = this.getClass().getSimpleName();	
	private DPMap map;
	private SBRealDevice  mNordicBoard;
    private ArrayList mPathToBeacon3;	
    private ArrayList mPathToBeacon1;
    private ArrayList currentPath;
    private int mNextBeacon;
    private int pathIndex = 0;
    private Context context;
    private CommServer mCommServer;

	
@Override
public void onCreate(Bundle savedInstanceState) {
	super.onCreate(savedInstanceState);
	log("app started ...");	
	//Intent i = new Intent(this, JettyServer.class);
	//startService(i);
	
     mNordicBoard	= SBRealDevice.getInstance(this);
	
    mCommServer = new CommServer(new SBCommHandler(mNordicBoard));	
	context = this;
	setDisconnectListener();
    //-- path setup 
    	
	LinkedList path = new LinkedList();
	Node node01 = new Node(1, 0);
	Node node02 = new Node(2, 1);
	Node node03 = new Node(3, 2);
    path.add(node01);
    path.add(node02);
    path.add(node03);
    
	ArrayList<Integer> node01Neighbors = new ArrayList<Integer>();
	node01Neighbors.add(node02.getId());
	node01.setNeighbors(node01Neighbors);
	
	ArrayList<Integer> node02Neighbors = new ArrayList<Integer>();
	node02Neighbors.add(node01.getId());
	node02Neighbors.add(node03.getId());
	node02.setNeighbors(node02Neighbors);
	
	ArrayList<Integer> node03Neighbors = new ArrayList<Integer>();
	node03Neighbors.add(node02.getId());
	node03.setNeighbors(node03Neighbors);
	
    map = new DPMap(path);	
	// parcours du path 
	
    Gson gson = new Gson();
    String jsonString = gson.toJson(map);
	log("encoding: \n" + jsonString);

	mPathToBeacon1 = new ArrayList<Integer>();
	mPathToBeacon1.add(3);
	mPathToBeacon1.add(2);
	mPathToBeacon1.add(1);
	
	mPathToBeacon3 = new ArrayList<Integer>();
	mPathToBeacon3.add(1); 
	mPathToBeacon3.add(2); 
	mPathToBeacon3.add(3); 
	
	mNextBeacon = 1;
	
    //mNordicBoard.addUsbListener(new SBUsbListener());
	

	
	try {
	   Thread.sleep(10000);	
	} catch (Exception e) {
		e.printStackTrace();
		Log.d("PowerSound", "bloc catch ex: " + e.getMessage()); 
	}
	
	MediaPlayer mediaPlayer = MediaPlayer.create(this, R.raw.powerup);
	mediaPlayer.start();
	
}









private void goToBeacon(int beaconId) {
	byte data[] = {SBProtocol.DP_STOP, 0, 0};
	 mNordicBoard.writeRaw(data);
	 
	 try {
		Thread.sleep(2000);
	} catch (InterruptedException e) {
		e.printStackTrace();
	} 
	 data[0] = SBProtocol.DP_GOTO_BEACON;
	 data[1] = (byte)beaconId;
	 data[2] = 0;
	 mNordicBoard.writeRaw(data);
}

private void goToNextBeacon() {
	
	if(pathIndex >= currentPath.size()){
		log("target reached ...");
		return;
	}
	
	mNextBeacon = (Integer)currentPath.get(pathIndex);
	log("go to beacon " + mNextBeacon );
    goToBeacon(mNextBeacon);
	pathIndex++;
}

@Override
protected void onNewIntent(Intent intent) {
		super.onNewIntent(intent);
		Log.d(TAG, " onNewIntent ...");
	mNordicBoard = SBRealDevice.getInstance(this);
    checkDevice();
   // mNordicBoard.addUsbListener(new SBUsbListener());	
		
}

private void checkDevice() {
	if (SBRealDevice
			.isConnected(context)) {
	    Log.d(TAG, "MCU connected ...");
		
	} else {
		mNordicBoard.disconnect(DeviceType.NUTINY);
		Log.d(TAG, "MCU disconnected ...");
	}
}


private void setDisconnectListener() {
	BroadcastReceiver mUsbReceiver = new BroadcastReceiver() {

		@Override
		public void onReceive(Context context, Intent intent) {
			Log.d(TAG, "onReceive ...");
	       checkDevice();	
		}

	};
	
	IntentFilter filter = new IntentFilter();
	filter.addAction(UsbManager.ACTION_USB_DEVICE_DETACHED);
	registerReceiver(mUsbReceiver, filter);
}
}