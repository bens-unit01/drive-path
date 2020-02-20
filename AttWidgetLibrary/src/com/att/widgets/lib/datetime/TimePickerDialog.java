package com.att.widgets.lib.datetime;

import java.text.SimpleDateFormat;

import android.app.Dialog;
import android.content.Context;
import android.view.View;
import android.view.ViewGroup;

import com.att.widgets.lib.R;
import com.att.widgets.lib.button.StaticTextButton;

public class TimePickerDialog extends Dialog implements android.view.View.OnClickListener{

	private String format = String.format("%%0%dd", 2); 
	private StaticTextButton mDoneButton;
	private TimePicker timePicker;
	private WheelView mHourWheel, mMinuteWheel, mAmPmWheel;
	private TimePickerListener mListener;
	
	public TimePickerDialog(Context context,TimePicker tp) {
		super(context,R.style.dialog);
		timePicker=tp;
		init(context);
	}
	
	private void init(Context context) {
		setContentView(R.drawable.time_picker);

		mHourWheel = (WheelView)findViewById(R.id.time_picker_hours);
		mMinuteWheel = (WheelView)findViewById(R.id.time_picker_minutes);
		mAmPmWheel = (WheelView)findViewById(R.id.time_picker_am_pm);
		
		mAmPmWheel.setImg_wheel(WheelView.TIME_PICKER_RIGTH_IMG);
		mAmPmWheel.setAdapter(new ArrayWheelAdapter<String>(timePicker.MERIDIANS));
		mAmPmWheel.setCyclic(false);
		mHourWheel.setAdapter(new NumericWheelAdapter(1,12, format));
		mHourWheel.setImg_wheel(WheelView.TIME_PICKER_LEFT_IMG);
		mHourWheel.setCyclic(true);
		mMinuteWheel.setImg_wheel(WheelView.TIME_PICKER_MIDDLE_IMG);
		mMinuteWheel.setAdapter(new NumericWheelAdapter(0, 59, format));
		mMinuteWheel.setCyclic(true);
		
		this.setCurrentTime();
		
		mDoneButton = (StaticTextButton) findViewById(R.id.time_picker_done);
		mDoneButton.setType(StaticTextButton.PRIMARY_BUTTON_HIGHLIGHTED);
		mDoneButton.setTextColor(0xFFFFFFFF);
		mDoneButton.setOnClickListener(this);
	}
	
	public void setCurrentTime(){
		mHourWheel.setCurrentItem(timePicker.getHours()-1);
        mMinuteWheel.setCurrentItem(timePicker.getMinutes());
        SimpleDateFormat sdf2= new SimpleDateFormat("a");
		
        if(!timePicker.isHour_format_24()){
        
			String AM_PM=sdf2.format(timePicker.getCalendar().getTime());
			if(AM_PM.equals("AM")){
				
				mAmPmWheel.setCurrentItem(0);
			}else { mAmPmWheel.setCurrentItem(1);}
		
        }else{
        	
        	mHourWheel.setCurrentItem(timePicker.getHours());
        }
       
	}
	

	
	public void onClick(View v) {
		if(mListener != null){
			mListener.onTimeChange(getTimeStr());
		}
		this.dismiss();
	}

	public void setFocusable(boolean focusabled) {
		mHourWheel.setFocusable(focusabled);
		mHourWheel.setEnabled(focusabled);
		mMinuteWheel.setFocusable(focusabled);
		mMinuteWheel.setEnabled(focusabled);
		mAmPmWheel.setFocusable(focusabled);
		mAmPmWheel.setEnabled(focusabled);
		
	}

	public void setFocusableInTouchMode(boolean focusabled) {
		mHourWheel.setFocusableInTouchMode(focusabled);
		mMinuteWheel.setFocusableInTouchMode(focusabled);
		mAmPmWheel.setFocusableInTouchMode(focusabled);
		mHourWheel.setFocusable(focusabled);
		mMinuteWheel.setFocusable(focusabled);
		mAmPmWheel.setFocusable(focusabled);
	}
	
	public String getTimeStr(){
		
		
		if(!timePicker.isHour_format_24()){
			return String.format("%02d:%02d %s", mHourWheel.getCurrentItem()+1, mMinuteWheel.getCurrentItem(), timePicker.MERIDIANS[mAmPmWheel.getCurrentItem()]);
		}
			
		return String.format("%02d:%02d", mHourWheel.getCurrentItem(), mMinuteWheel.getCurrentItem());
	}

		
	public void setTimePickerListener(TimePickerListener listener) {
		this.mListener = listener;
	}
	
	public interface TimePickerListener{
		public void onTimeChange(String time);
	}
	
	public void setFormat24(){
	
		mHourWheel.setAdapter(new NumericWheelAdapter(0,23, format));
		ViewGroup linearInside = (ViewGroup)findViewById(R.id.linear_inside);
		linearInside.removeViewAt(2);
		this.setCurrentTime();
		
		
		
	}
	
}