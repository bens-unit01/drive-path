package com.megamip.util;

//  the code of this class is partially inspired from :http://codersapprentice.blogspot.ca/2011/09/android-integrate-jetty-server-in-my.html
/**
 * 
 * @author Messaoud BENSALEM
 * @version 1.0 09/12/13 
 * 
 * This class provide a listener to a i-Jetty web server through
 * the JettyListener interface according to the Observer design pattern
 * 
 */

import java.io.IOException;
import java.util.ArrayList;
import java.util.EventObject;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.eclipse.jetty.server.Handler;
import org.eclipse.jetty.server.Request;
import org.eclipse.jetty.server.Server;
import org.eclipse.jetty.server.handler.AbstractHandler;

import com.megamip.voice.MainActivity;
import com.megamip.voice.R;





import android.app.Notification;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

public class JettyServer extends Service {

	public static final String TAG = JettyServer.class.getName();
	public static final int SERVERPORT = 8080;
	public static final String SPLIT_CHAR = "/";
	private static ArrayList<JettyListener> mListeListeners = new ArrayList<JettyListener>();

	private Handler jettyHandler = new AbstractHandler() {
		// @Override
		public void handle(String target, Request request,
				HttpServletRequest MainRequestObject,
				HttpServletResponse response) throws IOException,
				ServletException {
			try {
				

				for (JettyListener b : mListeListeners) {

					b.onNotify(new ServerEvent(this, target));
					// Log.d(TAG3, "JettyServer  onNotify   ");
				}
			
			} catch (Exception ex) {
				Log.i(TAG, "Error" + ex.getMessage());
			}
		}

	};

	// constructor

	public JettyServer() {
		super();

//		Server server = new Server(SERVERPORT);
//		server.setHandler(jettyHandler);
//		try {
//			server.start();
//			Log.i(TAG, "server started ...");
//
//		} catch (Exception e) {
//			e.printStackTrace();
//			Log.i(TAG, "block catch ...ex: " + e.getMessage());
//		}
	
		
	}
	


	public interface JettyListener {

		public void onNotify(ServerEvent e);
	}

	public class ServerEvent extends EventObject {

		public ServerEvent(Object source, String params) {
			super(source);
			this.params = params;
	}

		/**
		 * 
		 * 
		 * 
		 */

		private static final long serialVersionUID = 1L;
		private String params; // forward slash separated values including the
								// method called, expel :
								// /function/param1/param2 ...

		public String getParams() {
			return params;
		}

		public void setParams(String params) {
			this.params = params;
		}

	}

	public void addJettyListener(JettyListener listener) {
		mListeListeners.add(listener);
		Log.d(TAG, "new jetty listener added ...");

	}
	
	
	// sevice methods 
	
	/**
	 * this method is used to stop the service 
	 * @param view
	 */
	 public void stopService(View view) {
		 
	        stopService(new Intent(this, JettyServer.class));
	        Log.d(TAG,"stopService ... ");
	 }
	 
	 public void startNewService(Context context) {
	 	 
	        startService(new Intent(context, JettyServer.class));
	     
	        Log.d(TAG,"MainActivity - startNewService ");
     }
	 
	    
	 @Override
		public int onStartCommand(Intent intent, int flags, int startId) {
			
			
			Log.w(getClass().getName(), "onStartCommand ...");
			Server server = new Server(SERVERPORT);
			server.setHandler(jettyHandler);
			try {
				server.start();
				Log.i(TAG, "server started ...");
	
			} catch (Exception e) {
				e.printStackTrace();
				Log.i(TAG, "block catch ...ex: " + e.getMessage());
			}
			
			// making the service a foreground service 
			Notification note = new Notification(R.drawable.led_on,
					"Megamip lanched ...", System.currentTimeMillis());
			
			Intent i= new Intent(this, JettyServer.class);
			i.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_SINGLE_TOP);
			
			PendingIntent pi = PendingIntent.getActivity(this, 0, i, 0);
			note.setLatestEventInfo(this, "Fake Player", "Now Playing : ..",pi);
			note.flags |= Notification.FLAG_NO_CLEAR;
			startForeground(1337, note);
			
			return (START_NOT_STICKY);
		}
  

	


	@Override
	public IBinder onBind(Intent intent) {
	
		return null;
	}

	@Override
	public void onStart(Intent intent, int startId) {

		Log.d(TAG, "service started ...");
	}

	@Override
	public void onDestroy() {
        Log.d(TAG, "service stopped ...");
	}

}
