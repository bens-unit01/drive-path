package com.megamip.view;

import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;

public class ProgressTask extends AsyncTask<String, Void, Boolean>{

	
	private ProgressDialog dialog;
	private Context context;
	private Task task;
	
	public ProgressTask(Context context, Task task){
	    this.context = context;
 		this.task = task;
		dialog = new ProgressDialog(context);
	}
	
   public ProgressTask(){
		
	}
	
	@Override
	protected Boolean doInBackground(String... params) {
		
	   task.run();
		
		return true;
	}
	
	
	@Override
	protected void onPreExecute() {
		super.onPreExecute();
		//this.dialog.setMessage("Progress start");
	   
		this.dialog.show();
	}
	
	@Override
	protected void onPostExecute(Boolean result) {
		super.onPostExecute(result);
		
		if(dialog.isShowing()){
			dialog.dismiss();
		}
		
		
	}
	
	public interface Task{
		public void run();
	}

}
