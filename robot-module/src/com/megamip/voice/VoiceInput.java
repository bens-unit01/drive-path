package com.megamip.voice;

import java.io.Serializable;
import java.util.Arrays;

import android.util.Log;

public class VoiceInput extends MipInput implements Serializable {
	
	private final static String TAG = "A3";
	public static final int SEARCH_CMD = 0;
	public static final int SEARCH_PIC = 1;
	public static final int SEARCH_VID = 2;
	

	private String input;
	
	// constructors 
	
	public VoiceInput() {
		super();
	}
	
	public VoiceInput(String input){
		this.input = input;
		String[] inputArray = input.split(" ");
		try{
		     action = inputArray[0];	
		     args = new String[inputArray.length-1];
		     
		     Log.d(TAG, " VoiceCommand bloc try action = "+action);
		     
		     for(int i =0; i< inputArray.length-1; i++){
		    	 
		    	 args[i] = inputArray[i+1];
		     }
			
		}catch(Exception ex){
			
			Log.d(TAG, " VoiceCommand bloc catch action = "+action);
			action = null;
			args = null;
		}
		
	}

	
	// getters and setters 
	public String getAction() {
		return action;
	}

	public void setAction(String action) {
		this.action = action;
	}

	public String[] getArgs() {
		return args;
	}

	public void setArgs(String[] args) {
		this.args = args;
	}

	@Override
	public String toString() {
		return "VoiceCommand [action=" + action + ", args="
				+ Arrays.toString(args) + "]";
	}

	public String getInput() {
		return input;
	}

	
	
    
	
	
	
	

}
