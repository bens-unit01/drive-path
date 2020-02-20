package com.megamip.util;

import java.io.DataOutputStream;
import java.net.InetAddress;
import java.net.InterfaceAddress;
import java.net.NetworkInterface;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Enumeration;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.http.conn.util.InetAddressUtils;


import android.app.ActivityManager;
import android.content.Context;
import android.text.format.Formatter;
import android.util.Log;

public class MipUtils {

	// ------- constructors

	public MipUtils() {
		super();

	}

	
	
	public String getYoutubeVideoId(String youtubeUrl) {
		String video_id = "";
		Log.d("A3", "MipVideoPlayer#getYoutubeVideoId url: " + youtubeUrl);
		if (youtubeUrl != null && youtubeUrl.trim().length() > 0
				&& youtubeUrl.startsWith("http")) {

			String expression = "^.*((youtu.be"
					+ "\\/)"
					+ "|(v\\/)|(\\/u\\/w\\/)|(embed\\/)|(watch\\?))\\??v?=?([^#\\&\\?]*).*"; // var
																								// regExp
																								// =
																								// /^.*((youtu.be\/)|(v\/)|(\/u\/\w\/)|(embed\/)|(watch\?))\??v?=?([^#\&\?]*).*/;
			CharSequence input = youtubeUrl;
			Pattern pattern = Pattern.compile(expression,
					Pattern.CASE_INSENSITIVE);
			Matcher matcher = pattern.matcher(input);
			if (matcher.matches()) {
				String groupIndex1 = matcher.group(7);
				if (groupIndex1 != null && groupIndex1.length() == 11)
					video_id = groupIndex1;
			}
		}
		Log.d("A3", "MipVideoPlayer#getYoutubeVideoId video_id: " + video_id);
		return video_id;
	}
	
	public void closeApp(String packageName, Context context){
		
		ActivityManager am = (ActivityManager) context
				.getSystemService(Context.ACTIVITY_SERVICE);
		List<ActivityManager.RunningAppProcessInfo> pids = am
				.getRunningAppProcesses();
		int processid = 0;
		int uid = 0;
		int myUid = android.os.Process.myUid();
		for (int i = 0; i < pids.size(); i++) {
			ActivityManager.RunningAppProcessInfo info = pids
					.get(i);
		//	"air.air.MipVideoPlayer"
			if (info.processName
					.equalsIgnoreCase(packageName)) {
				processid = info.pid;
				uid = info.uid;
			}
		}
		
		
		List<String> cmdList = new ArrayList<String>();
		cmdList.add("kill -9 "+processid);
		try {
			doCmds(cmdList);
			Log.d("A3", "kill process end ...");
		} catch (Exception e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}

	}
	
	private static void doCmds(List<String> cmds) throws Exception {
	    Process process = Runtime.getRuntime().exec("su");
	    DataOutputStream os = new DataOutputStream(process.getOutputStream());

	    for (String tmpCmd : cmds) {
	            os.writeBytes(tmpCmd+"\n");
	    }

	    os.writeBytes("exit\n");
	    os.flush();
	    os.close();

	    process.waitFor();
	} 
	
	public static String getIPAddress() {
		try {
			for (Enumeration<NetworkInterface> en = NetworkInterface
					.getNetworkInterfaces(); en.hasMoreElements();) {
				NetworkInterface intf = en.nextElement();
				
				if(intf.getName().equals("wlan0") ){
					InterfaceAddress addr = intf.getInterfaceAddresses().get(1);
					return addr.getAddress().getHostAddress();
				}

			}
		} catch (Exception ex) {
			Log.d(MipUtils.class.getName(),
					"bloc catch ex: " + ex.getMessage());
		}
		return "";
	}
	
	
	 public static String getIPAddress(boolean useIPv4) {
	        try {
	            List<NetworkInterface> interfaces = Collections.list(NetworkInterface.getNetworkInterfaces());
	            for (NetworkInterface intf : interfaces) {
	                List<InetAddress> addrs = Collections.list(intf.getInetAddresses());
	                for (InetAddress addr : addrs) {
	                    if (!addr.isLoopbackAddress()) {
	                        String sAddr = addr.getHostAddress().toUpperCase();
	                        boolean isIPv4 = InetAddressUtils.isIPv4Address(sAddr); 
	                        if (useIPv4) {
	                            if (isIPv4) 
	                                return sAddr;
	                        } else {
	                            if (!isIPv4) {
	                                int delim = sAddr.indexOf('%'); // drop ip6 port suffix
	                                return delim<0 ? sAddr : sAddr.substring(0, delim);
	                            }
	                        }
	                    }
	                }
	            }
	        } catch (Exception ex) { } // for now eat exceptions
	        return "";
	    }


}
