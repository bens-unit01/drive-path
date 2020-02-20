package com.att.widgets.lib.zoom;


import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.AttributeSet;
import android.view.View;
import android.widget.ImageView;
import android.widget.RelativeLayout;

import com.att.widgets.lib.R;
import com.att.widgets.lib.zoom.slider.ZoomSlider;
import com.att.widgets.lib.zoom.slider.ZoomSlider.ZoomSliderListener;

/**
 * ZoomControl widget
 */
public class ZoomControl extends RelativeLayout{
	private static final long SLIDER_SHOW_TIME = 4000;
	
	//views
	private ZoomView mZoomView;
	private ZoomSlider mZoomSlider;
	private ImageView mShowBtn;

	//The thread which hides the slider bar
	private HiderThread mHiderThread;
	
	//A lock variable to avoid race conditions with threads
	private final Lock lock = new ReentrantLock();
	
	/**
	 * Creates a ZoomControl Widget with a context.
	 * @param context
	 */
	public ZoomControl(Context context) {
		super(context);
		init(context);
	}
	
	/**
	 * Creates a ZoomControl Widget with a set of attributes.
	 * @param context
	 * @param attrs
	 */
	public ZoomControl(Context context, AttributeSet attrs) {
		super(context, attrs);
		init(context);
	}
	
	private void init(Context context){
		inflate(context, R.drawable.zoom_control, this);
		//Views
		mZoomSlider = (ZoomSlider)findViewById(R.id.zoom_control_slider);
		mZoomView = (ZoomView)findViewById(R.id.zoom_control_view);
		mShowBtn = (ImageView)findViewById(R.id.zoom_control_show);
		
		//Listeners
		mShowBtn.setOnClickListener(mShowSliderListener);
		mZoomSlider.setZoomSliderListener(mZoomSliderListener);
	}
	
	private OnClickListener mShowSliderListener = new OnClickListener() {
		@Override
		public void onClick(View v) {
			showSlider(true);
			autoHideSlider();
		}
	};
	
	private ZoomSliderListener mZoomSliderListener = new ZoomSliderListener() {
		@Override
		synchronized public void progressChange(int progress) {		
			lock.lock();
			if(mHiderThread != null){
				mHiderThread.mHide = false;
			}
			lock.unlock();
			mZoomView.setZoomX(progress);
		}

		@Override
		public void startTracking() {
			cancelHiderThread();
		}

		@Override
		public void stopTracking() {
			autoHideSlider();
		}
	};
	
	/**
	 * Set a image Resource to the zoom view
	 * container.
	 * @param resource
	 */
	public void setImage(int resource){
		mZoomView.setImage(resource);
	}

	private void showSlider(boolean show) {
		if(show){			
			mShowBtn.setVisibility(GONE);
			mZoomSlider.setVisibility(VISIBLE);
		}else{
			mShowBtn.setVisibility(VISIBLE);
			changeShowBtnPos();
			mZoomSlider.setVisibility(GONE);
		}
		
	}
	
	private void changeShowBtnPos(){
		int y = (int)(mZoomSlider.getTop() + mZoomSlider.getThumbPos() + mShowBtn.getHeight()/2);
		RelativeLayout.LayoutParams rl = new RelativeLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
		rl.setMargins(0, y, 0, 0);
		mShowBtn.setLayoutParams(rl);
	}

	/**
	 * Zoom In the image.
	 */
	public void zoomIn() {
		mZoomSlider.zoomIn();
	}

	/**
	 * Zoom out the image.
	 */
	public void zoomOut() {
		mZoomSlider.zoomOut();
	}
	
	
	/**
	 * Set the max zoom X.
	 * @param maxZoom
	 */
	public void setMaxZoom(int maxZoom){
		if(maxZoom > 0){			
			mZoomSlider.setMax(Math.round(maxZoom / mZoomView.getStep()));
		}
	}
	
	private void cancelHiderThread(){
		lock.lock();
		if(mHiderThread != null){
			mHiderThread.interrupt();
			mHiderThread = null;
		}
		lock.unlock();
	}
	
	 private void autoHideSlider(){			
		if(mHiderThread != null){
			return;
		}
		
		mHiderThread = new HiderThread();
		mHiderThread.start();
	}
	
	private class HiderThread extends Thread{
		private boolean mHide;
		
		@Override
		public void run() {
			try {
				do{
					mHide = true;						
					Thread.sleep(SLIDER_SHOW_TIME);
				}while(!mHide);
				
				//hide the slider bar
				post(new Runnable() {public void run() {showSlider(false);}});
			} catch (InterruptedException e) {
			}
				
			mHiderThread = null;
		}
	}
	
	
	

}
