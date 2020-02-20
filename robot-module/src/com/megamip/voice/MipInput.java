package com.megamip.voice;

import java.util.Arrays;

public abstract class MipInput {
	
	protected String action;
	protected String[] args;

    //-- constructors 
	public MipInput() {
		super();
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

	
	// toString()
	@Override
	public String toString() {
		return "MipInput [action=" + action + ", args=" + Arrays.toString(args)
				+ "]";
	}
	

	

	
	
	

}
