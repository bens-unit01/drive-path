package com.megamip.util;

import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.ScheduledFuture;






public class MipTimer {
	
	   public static final String TAG1 = "A1", TAG2 = "A2", TAG3 = "A3";
       private Timer timer;
       
       
       private static MipTimer reminder = null;
       private MipTimerListener listener;
       private int period;
       private int id;
       private ScheduledFuture sf;
       
     
   // constructors 
    public MipTimer(int period, int id) {
    	this.id = id;
    	this.period = period;
        timer = new Timer();
        timer.schedule(new MipTimerTask(),  period*1000);
        System.out.println("MipReminder - constructor");
	}
    
    public MipTimer(float period, int id) {
    	this.id = id;
    //	this.period = period;
        timer = new Timer();
        timer.schedule(new MipTimerTask(), (int) period*1000);
        System.out.println("MipReminder - constructor");
	}

    
    // getters & setters 
    
	    public int getId() {
			return id;
		}
    
    // Class methods 
    public void resetTimer(){
    	
    	if(timer != null){
    	  timer.cancel();
          timer.purge();
    	}
    	timer = new Timer();
    	timer.schedule(new MipTimerTask(), period*1000);
    	//System.out.println("MipReminder - resetReminder() timerID: "+id);
    	
    }
    
   


	public void stopTimer(){
    	
    	timer.cancel();
    	System.out.println("MipReminder - stopReminder()");
    }
    public class MipTimerTask extends TimerTask {
        public void run() {
            
              timer.cancel(); 
              timer.purge();
              timer = new Timer();
              timer.schedule(new MipTimerTask(), period*1000);
       
            if( listener != null){
            	listener.update();
            }
        }
    }
    
    public interface MipTimerListener{
    	public void update();
    }
    
    public void addEventListener(MipTimerListener listener){
    	this.listener = listener;
    }
    
    

   
}
