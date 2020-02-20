package com.rc.new_beacon;

import java.util.ArrayList;

public class LogDataList {
	

	private ArrayList<LogData> logDataList;
	public LogDataList() {
		super();
		this.logDataList = new ArrayList<LogData>();  
	}
	
	public void addLog(LogData log){
          this.logDataList.add(log); 	
	}

}
