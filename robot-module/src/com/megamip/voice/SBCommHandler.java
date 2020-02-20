package com.megamip.voice;

import java.io.IOException;
import java.util.LinkedList;

import android.util.Log;

import com.google.gson.Gson;
import com.google.gson.JsonSyntaxException;
import com.megamip.drivepath.DPMap;
import com.megamip.drivepath.Node;
import com.megamip.util.AdbUtils;
import com.megamip.util.Utils;
import com.megamip.voice.SBUsbDevice.UsbEvent;
import com.megamip.voice.SBUsbDevice.UsbListener;
import com.ww.websocket_server.CommHandler;

public class SBCommHandler implements CommHandler{

	SBRealDevice mNordicsBoard;
	
	
	public SBCommHandler(SBRealDevice mNordicsBoard) {
		super();
		this.mNordicsBoard = mNordicsBoard;
		this.mNordicsBoard.addUsbListener(new SBUsbListener());
		
	}


	@Override
	public void handle(String str) {
	  try{
		  log(" #handle str: " + str);
		  jettyHandler(str);	
	  }catch(NumberFormatException e)	{
		  
	  }catch(ArrayIndexOutOfBoundsException e){
		  
	  }catch(JsonSyntaxException e){
		  
	  }
	   
	}
	
	
	public class SBUsbListener implements UsbListener {

		@Override
		public void onNotify(UsbEvent e) {
		 
			// log("data: " + Utils.bytesToHex2(e.getData()));
			 
			 nordicHandler(e.getData()[1]);
		}
		
	}
	
	protected void nordicHandler(byte b) {

		switch (b) {
		case SBProtocol.NOTF_GET_NEXT_BEACON:
	       //  log("go to next beacon : " + pathIndex);
	      //   goToNextBeacon(); 
			break;
		case SBProtocol.NOTF_NORDIC_MB_TEST:
	    //    log("Test Nordic --> Mediabox OK " );
			break;

		default:
			break;
		}
	}
	
	

private void jettyHandler(String params)
		throws ArrayIndexOutOfBoundsException, NumberFormatException, JsonSyntaxException {

	String[] input = params.split(SBProtocol.JETTY_SPLIT_CHAR);


	// int turnTime = MipReceiver.TIME / 2;
	//mipUsbDeviceUno = MipUsbDevice.getInstance(context, DeviceType.UNO);

	// Log.d(TAG, "jettHandler cmd: " + input[1]);
	
	// --------- drive path handling 
   	
	if(input[1].equals(SBProtocol.JETTY_GOTO_BEACON)){
	 int beaconId  = Integer.parseInt(input[2]) ;
	 log("drivepath go : " + beaconId );
//	 goToBeacon(beaconId);
	}
	
	if(input[1].equals(SBProtocol.JETTY_DISCONNECT_BEACON)){
	  log("drivepath disconnect "); 	
	 byte data[] = {SBProtocol.DP_STOP, 0, 0};
	 mNordicsBoard.writeRaw(data);
	}
	
	if(input[1].equals(SBProtocol.JETTY_CLR_ESTOP)){
		 log("clear eStop ");
		 byte data[] = {SBProtocol.CLEAR_ESTOP, 0, 0};
		 mNordicsBoard.writeRaw(data);
	}
	
	if(input[1].equals("setEStop")){
		 log("set eStop ");
		 byte data[] = {SBProtocol.ESTOP, 0, 0};
		 mNordicsBoard.writeRaw(data);
	  }
	
	if(input[1].equals(SBProtocol.JETTY_DRIVEPATH_INIT)){
	 log("\ndrivepath init config: " + input[2]);
	 // data decoding
	 log("\ndecoding:");
	 Gson gson = new Gson();
	 DPMap l = gson.fromJson(input[2], DPMap.class); 
	 LinkedList<Node> nodes = l.getNodes();
	 
	 log("current: " + l.getCurrent());
	 for(Node element: nodes){
	    log(" id: " + element.getId() + " weight: " + element.getWeight()); 
	 }
	}
	
	
	if(input[1].equals(SBProtocol.JETTY_DRIVETO_BEACON)){
		if(input[2].equals("1")){
			//currentPath = mPathToBeacon1;
			//pathIndex = 0;
			log("drive to beacon  1");
			//goToNextBeacon();
		};
		if(input[2].equals("3")){
			//currentPath = mPathToBeacon3;
			//pathIndex = 0;
			log("drive to beacon 3");
			//goToNextBeacon();
	
		};
		
	}
	if(input[1].equals(SBProtocol.JETTY_ACTIVATE_ADB)){
       log("activating adb ...");	
       try {
		AdbUtils.set(5555);
	} catch (IOException e) {
       log("catch: " + e.getMessage());	
		e.printStackTrace();
	} catch (InterruptedException e) {
       log("catch: " + e.getMessage());	
		e.printStackTrace();
	}
	}
	
	
	if(input[1].equals(SBProtocol.JETTY_NORDIC_MEDIABOX_TEST)){
		 log("request test : Nordic --> Mediabox  ");
		 byte data[] = {SBProtocol.DP_NORDIC_MB_TEST, 0, 0};
		 mNordicsBoard.writeRaw(data);	
	}
}


private void log(String string) {
  Log.d("SBCommHandler", string);	
}

}
