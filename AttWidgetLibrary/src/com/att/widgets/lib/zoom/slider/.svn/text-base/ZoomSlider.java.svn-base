package com.att.widgets.lib.zoom.slider;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.widget.ImageButton;
import android.widget.LinearLayout;

import com.att.widgets.lib.R;
import com.att.widgets.lib.zoom.slider.VerticalSeekBar.OnSeekBarChangeListener;

public class ZoomSlider extends LinearLayout{

	private VerticalSeekBar mSlider;
	private ImageButton mPlusBtn, mMinusBtn;
	private ZoomSliderListener mListener;
	
	public ZoomSlider(Context context, AttributeSet attrs) {
		super(context, attrs);
		init(context);
	}
	public ZoomSlider(Context context) {
		super(context);
		init(context);
	}
	
	private void init(Context context){
		inflate(context, R.drawable.zoom_slider, this);
		mSlider = (VerticalSeekBar)findViewById(R.id.zoom_slider_bar);
		mPlusBtn = (ImageButton)findViewById(R.id.zoom_slider_plus);
		mMinusBtn = (ImageButton)findViewById(R.id.zoom_slider_minus);
		
		mPlusBtn.setOnClickListener(buttonsListener);
		mMinusBtn.setOnClickListener(buttonsListener);
		mSlider.setOnSeekBarChangeListener(seekbarListener);
		setMax(5);
		
		mSlider.setProgressDrawable(getResources().getDrawable(R.drawable.zoom_bar));
		mSlider.setThumbOffset(-1);
	}
	
	public void setZoomSliderListener(ZoomSliderListener listener){
		this.mListener = listener;
	}
	
	public void setMax(int max){
		mSlider.setMax(max);
	}
	
	public int getMax(){
		return mSlider.getMax();
	}
	
	private OnClickListener buttonsListener = new OnClickListener() {
		@Override
		public void onClick(View v) {
			if(v == mPlusBtn){
				zoomIn();
			}else if(v == mMinusBtn){
				zoomOut();
			}
		}
	};
	
	private OnSeekBarChangeListener seekbarListener = new OnSeekBarChangeListener() {
		@Override
		public void onStopTrackingTouch(VerticalSeekBar seekBar) {
			if(mListener != null){
				mListener.stopTracking();
			}
		}
			
		@Override
		public void onStartTrackingTouch(VerticalSeekBar seekBar) {
			if(mListener != null){
				mListener.startTracking();
			}
		}
		
		@Override
		public void onProgressChanged(VerticalSeekBar seekBar, int progress,
				boolean fromUser) {
			if(mListener != null){
				mListener.progressChange(progress);
			}
		}
	};
	
	public void zoomIn(){
		mSlider.incrementProgressBy(1);
	}
	
	public void zoomOut(){
		mSlider.incrementProgressBy(-1);
	}
	
	public interface ZoomSliderListener{
		public void progressChange(int progress);
		public void stopTracking();
		public void startTracking();
	}

	public int getProgress() {
		return mSlider.getProgress();
	}
	
	public int getThumbPos(){
		return mSlider.getThumbPos();
	}

}
