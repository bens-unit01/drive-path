package com.megamip.util;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.ActivityManager;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.content.Context;
import android.util.Log;
import android.widget.ScrollView;
import android.widget.TextView;



public class PerformanceUtils {
	
	
	
	private ActivityManager manager;
	private ScrollView sc;
    private static Map<Integer, String> importance = new HashMap<Integer, String>();
	private static Map<Integer, String> reason = new HashMap<Integer, String>();


	static {
		importance.put(RunningAppProcessInfo.IMPORTANCE_FOREGROUND,
				"IMPORTANCE_FOREGROUND");
		importance.put(RunningAppProcessInfo.IMPORTANCE_PERCEPTIBLE,
				"IMPORTANCE_PERCEPTIBLE");
		importance.put(RunningAppProcessInfo.IMPORTANCE_VISIBLE,
				"IMPORTANCE_VISIBLE");
		importance.put(RunningAppProcessInfo.IMPORTANCE_SERVICE,
				"IMPORTANCE_SERVICE");
		importance.put(RunningAppProcessInfo.IMPORTANCE_BACKGROUND,
				"IMPORTANCE_BACKGROUND");
		importance.put(RunningAppProcessInfo.IMPORTANCE_EMPTY,
				"IMPORTANCE_EMPTY");
		importance.put(RunningAppProcessInfo.REASON_PROVIDER_IN_USE,
				"REASON_PROVIDER_IN_USE");
		importance.put(RunningAppProcessInfo.IMPORTANCE_SERVICE,
				"REASON_SERVICE_IN_USE");
		importance.put(RunningAppProcessInfo.REASON_UNKNOWN, "UNKNOWN");

	}


	public PerformanceUtils(Context context) {
		super();
		
		manager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
	}
	
	
	public void searchRunningAppProcesses() {
		final List<RunningAppProcessInfo> apps = manager.getRunningAppProcesses();
		StringBuilder sb = new StringBuilder();
		
		for (RunningAppProcessInfo rapi : apps) {
			StringBuilder tmp = new StringBuilder();
			tmp.append("processName : ").append(rapi.processName).append("\n");
			tmp.append("importance : ").append(importance.get(rapi.importance)).append("\n");
			tmp.append("importanceReasonCode : ").append(reason.get(rapi.importanceReasonCode)).append("\n");
			tmp.append("-----------\n");
			sb.append(tmp);
		}
		
	
	    Log.d(getClass().getName(), sb.toString());
	
		
	}


}
