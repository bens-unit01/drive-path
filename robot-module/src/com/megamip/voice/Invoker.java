package com.megamip.voice;

public class Invoker {

	
	public enum State {
		PROCESSING,
		IDLE,
		ACTIVE
	}
	
	private State state;
	
	
	public State getState() {
		return state;
	}


	public void setState(State state) {
		this.state = state;
	}
	
	
	
	// constructor 
	
	

	


	public Invoker(){}
	 
	 
	 // public methods 
	 
	 public void launch(Command command){
		 
		 command.execute();
	 }
}
