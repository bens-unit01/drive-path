package com.rc.new_beacon;

import java.util.ArrayList;

public class LogData {
	private String mLabel; 
	private String mRange;
	private String mHeading;
	private String mPosAngle;
	private String[][] mMatrix;

	
	
	public LogData(String mLabel, String mRange, String mHeading,
			String mPosAngle, int[][] mMatrix) {
		super();
		this.mLabel = mLabel;
		this.mRange = mRange;
		this.mHeading = mHeading;
		this.mPosAngle = mPosAngle;
	
		this.mMatrix = new String[9][4]; 
		for(int i = 0; i < 9; i++){
		    for(int j = 0; j < 4; j++){
		    	this.mMatrix[i][j] = " " + mMatrix[i][j]; 
		    }
		 }
		
	}
	
	
	public LogData(String mLabel, String mRange, String mHeading,
			String mPosAngle, ArrayList<String> mMatrix) {
		super();
		this.mLabel = mLabel;
		this.mRange = mRange;
		this.mHeading = mHeading;
		this.mPosAngle = mPosAngle;
	
		this.mMatrix = new String[9][4]; 
		

		int k = 0;

		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 4; j++) {
			//	data.set(k + j, "" + (buffer[i][j]));
				this.mMatrix[i][j] = mMatrix.get(k + j);
			}
			k += 4;
		}
		
		
	}
	
	public LogData(String mLabel, String mRange, String mHeading,
			String mPosAngle) {
		super();
		this.mLabel = mLabel;
		this.mRange = mRange;
		this.mHeading = mHeading;
		this.mPosAngle = mPosAngle;
	}
	
	
	
   private LogData(){}
}
