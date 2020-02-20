package com.att.widgets.lib.datetime;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnDismissListener;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.att.widgets.lib.R;
import com.att.widgets.lib.datetime.TimePickerDialog.TimePickerListener;

public class TimePicker extends RelativeLayout {

	public static final String[] MERIDIANS = new String[]{"AM", "PM"};
	private String format= String.format("%%0%dd", 2);
    private int hours,minutes;
	private ViewGroup inflated;
	private TextView timeLabel;
	private TextView currentTime;
	private TextView background;
	private Calendar calendar;
	private TimePickerDialog timePickerDialog;
	private StringBuffer sb;
	public boolean hour_format_24;
	
	public TimePicker(Context context,Calendar myCalendar) {
		super(context);
		init(context,myCalendar);
	}
	
	public TimePicker(Context context) {
		super(context);
		init(context,null);
	}

	public TimePicker(Context context, AttributeSet attrs) {
		super(context, attrs);
		init(context,null);
	}

	
	
	

	private void init(Context context,Calendar myCalendar) {
		
		if(myCalendar==null){
			
			calendar = Calendar.getInstance();
			
		}else{
			
			calendar=myCalendar;
			
		}
		
		LayoutInflater inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		inflated = (ViewGroup) inflater.inflate(R.drawable.date_time_picker, null);
		this.addView(inflated);
		
		background = (TextView) findViewById(R.id.label_background);
		timeLabel = (TextView) findViewById(R.id.label_text_bold);
		currentTime = (TextView) findViewById(R.id.label_text_date_time);
		
		background.setOnClickListener(clickListener);
		timeLabel.setText("Time");
		
		currentTime.setText(this.getStringTime());
		
		timePickerDialog = new TimePickerDialog(context,this);
		timePickerDialog.setFocusable(true);
		timePickerDialog.setFocusableInTouchMode(true);
		timePickerDialog.setTimePickerListener(mTimePickerListener);
		
	}
	
	private TimePickerListener mTimePickerListener = new TimePickerListener() {		
		public void onTimeChange(String time) {
			currentTime.setText(time);

		}
	};
	
	
	private String getStringTime(){
		String hourFormat,minuteFormat;
		//Format 24
		//hours =calendar.get(Calendar.HOUR_OF_DAY);
		hours =calendar.get(Calendar.HOUR);
		minutes = calendar.get(Calendar.MINUTE);
		
		hourFormat=String.format(this.format,hours);
		minuteFormat=String.format(this.format,minutes);
		
		sb= new StringBuffer();
		sb.append(hourFormat).append(":").append(minuteFormat).append(" ").append(MERIDIANS[0]);
		
		
		
	//	int indice=0;
		
		DateFormat df=DateFormat.getTimeInstance();
		df.format(calendar.getTime());
		
		String format= "h:mm a";
		SimpleDateFormat sdf= new SimpleDateFormat(format);
		String date=sdf.format(calendar.getTime());
		SimpleDateFormat sdf2= new SimpleDateFormat("a");
		
		sdf2.format(calendar.getTime());
		return date;
		//return String.format("%02d:%02d %s", calendar.get(Calendar.HOUR), calendar.get(Calendar.MINUTE), MERIDIANS[0]);*/
		
	}
	
	
	private OnClickListener clickListener = new OnClickListener() {	
		public void onClick(View v) {
			
			timePickerDialog.show();
		}
	};
	
	@SuppressWarnings("unused")
	private OnDismissListener dismissListener = new OnDismissListener() {
		
		public void onDismiss(DialogInterface dialog) {
			currentTime.setText(timePickerDialog.getTimeStr());
		}
	};

    public String getCurrentTime(){
		
		return (String) currentTime.getText();
		
	}

	public Calendar getCalendar() {
		return calendar;
	}

	public void setCalendar(Calendar calendar) {
		this.calendar = calendar;
	}

	public int getHours() {
		return hours;
	}

	public void setHours(int hours) {
		this.hours = hours;
	}

	public int getMinutes() {
		return minutes;
	}

	public void setMinutes(int minutes) {
		this.minutes = minutes;
	}

	public StringBuffer getSb() {
		return sb;
	}

	public void setSb(StringBuffer sb) {
		this.sb = sb;
	}
	
	
	public boolean isHour_format_24() {
		return hour_format_24;
	}

    
	public void setHour_format_24(boolean hourFormat24) {
		this.hour_format_24 = hourFormat24;
		if(hour_format_24){
			this.timePickerDialog.setFormat24();
			currentTime.setText(timePickerDialog.getTimeStr());
		}
		
	}

}