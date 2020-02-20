package com.megamip.util;


// the source of this class is inspired from : http://bhargavg.wordpress.com/category/programming/







import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.List;

import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.BatteryManager;
import android.util.Base64;
import android.util.Log;

public class MipDeviceManager {
	public static final String TAG1 = "A1", TAG2 = "A2", TAG3 = "A3";

	Context context;
// -- constructors 
	
	
	private MipDeviceManager() { // never used 
		super();
		
	}
	
	public MipDeviceManager(Context context) {
		super();
		this.context = context;
	}
	
	
	//--------- public methods 
	public String getNotifications(){
		
		IntentFilter ifilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
		Intent batteryStatus = context.registerReceiver(null, ifilter);
		
		HttpURLConnection con;
		ConnectivityManager cm; 
		NetworkInfo networkInfo;
		String returnValue = "";
		try {
			
			//-----------  getting the number of unread emails 
			con = (HttpURLConnection) new URL("https://mail.google.com/mail/feed/atom/").openConnection();
			con.setRequestProperty("Authorization", "Basic " + Base64.encodeToString(("megamip01@gmail.com" + ":" + "megamip1234").getBytes(),Base64.DEFAULT ));
			con.connect();
			
			BufferedReader reader = new BufferedReader(new InputStreamReader(con.getInputStream()));
			 
			String line;
			String contents = "";
			 
			while ((line = reader.readLine()) != null) {
			  contents += line;
			}
			
			String[] t1 = contents.split("<fullcount>");
			String[] t2 = t1[1].split("</fullcount>");
			returnValue = t2[0];
			//Log.d(TAG3,"MipHttpClient - bloc try - new emails: "+t2[0]);
			
			
			//--------------getting the wifi signal strength-------------
			
			//-- code inspired from http://stackoverflow.com/questions/13834343/how-to-get-network-strength-in-android-application
			
			cm =(ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
			networkInfo = cm.getActiveNetworkInfo();
			int wifiPct =0;
			
			if(null == networkInfo || !networkInfo.isConnectedOrConnecting()){
			//	Log.d(TAG3, "No connection");
			}else{
				int netType = networkInfo.getType();
				int netSubType = networkInfo.getSubtype();
				
				if (ConnectivityManager.TYPE_WIFI == netType) {
				//	Log.d(TAG3,"MipHttpClient - bloc try - connection wifi");
					WifiManager wifiManager = (WifiManager) context.getApplicationContext().getSystemService(Context.WIFI_SERVICE);
					int rssi = wifiManager.getConnectionInfo().getRssi();
				    String ssid = wifiManager.getConnectionInfo().getSSID();
					int dBm =0;
					int index = 0;
					
					
				
					
					 List<ScanResult> scanResult = wifiManager.getScanResults();
		            for (int i = 0; i < scanResult.size(); i++) {
		            	
		            	if (ssid.equals(scanResult.get(i).SSID)) {
							index = i;
						}
		             //   Log.d(TAG3,"MipHttpClient - bloc try - nework SSID: "+scanResult.get(i).SSID+" strength: "+scanResult.get(i).level);//The db level of signal
		                
		            }
                       
		            dBm = scanResult.get(index).level;
		          //-- calculating the percentage 
					
					if (dBm <= -100) {
						wifiPct = 0;
					} else if (dBm >= -50) {
						wifiPct = 100;
					} else {
						wifiPct = 2 * (dBm + 100);
					} 
					
				//	Log.d(TAG3,"MipHttpClient - bloc try - connection wifi - rssi: "+rssi+" dbm: "+dBm+" quality: "+wifiPct+" ssid: "+ssid);
				
					
				}
			}
			
			
			returnValue += ":"+wifiPct;
			//------------- getting the battery level 
			
			int level = batteryStatus.getIntExtra(BatteryManager.EXTRA_LEVEL, -1);
			int scale = batteryStatus.getIntExtra(BatteryManager.EXTRA_SCALE, -1);
			
			float batteryPct = level / (float)scale;
			batteryPct *= 100;
			level = (int)batteryPct;
			returnValue += ":"+level;
			//Log.d(TAG3,"MipHttpClient - bloc try - battery level - batteryPct: "+batteryPct);
			
			
		} catch (MalformedURLException e) {
			returnValue = "0:0:0";
			Log.d(TAG3,"MipHttpClient - MalformedURLExceptio - bloc catch"+e.getMessage());
		} catch (IOException e) {
			returnValue = "0:0:0";
			Log.d(TAG3,"MipHttpClient - IOException - bloc catch"+e.getMessage());
		} catch(Exception e){
			returnValue = "0:0:0";
			Log.d(TAG3,"MipHttpClient - Exception - bloc catch"+e.getMessage());
		}
		
		
		return returnValue;
	}

	



}