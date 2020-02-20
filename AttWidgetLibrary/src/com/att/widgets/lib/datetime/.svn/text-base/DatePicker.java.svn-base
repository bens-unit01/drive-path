package com.att.widgets.lib.datetime;

import java.util.Calendar;
import java.util.Date;

import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.att.widgets.lib.R;

/**
 * This Widget shows the current selected date. This Widget has no real
 * functionality at all.
 * If you touch or click on it, a DatePickerDialog will appear.
 */
public class DatePicker extends RelativeLayout {

	public Date date;
	
	//Default Values
	public  int START_YEAR = 1900;
	public  int END_YEAR = 2100;
	public  int YEAR=-1;
	private int MONTH;
    private int DAY;
	private String format= String.format("%%0%dd", 2);
	public Calendar calendar;
	public String stringCurrentDate=null;
	private String months_string[] = new String[] {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sept","Oct","Nov","Dec"};
	private ViewGroup inflated;
	private TextView dateLabel;
	private TextView currentDate;
	private TextView background;
	private DatePickerDialog datePickerDialog;
    private StringBuffer sb;
	
    
    /**
	 * Creates a DatePicker Widget.
	 * @param context
	 * @param myCalendar
	 */
	public DatePicker(Context context,Calendar myCalendar) {
		super(context);
		init(context,myCalendar);
	}
	
	/**
	 * Creates a DatePicker Widget with a set of attributes.
	 * @param context
	 * @param attrs
	 */
	public DatePicker(Context context, AttributeSet attrs) {
		super(context, attrs);
		init(context,null);
		
	}
	
	/**
	 * Creates a DatePicker Widget with the defined style given by the user.
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public DatePicker(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init(context,null);
		
		
	}
	
	/**
	 * @author jvera
	 * Update screen with current value from calendar
	 * First time the Date is from system 
	 * 
	 * */
	public void updateScreen(){
		String dayFormat;
		
		
		//first Time
		if(stringCurrentDate==null){
			   
		           YEAR = calendar.get(Calendar.YEAR);
			       MONTH = calendar.get(Calendar.MONTH);
			       DAY = calendar.get(Calendar.DAY_OF_MONTH);
			 }
		
			   dayFormat= String.format(format,calendar.get(Calendar.DAY_OF_MONTH));
			   sb = new StringBuffer();
			   sb.append(months_string[(calendar.get(Calendar.MONTH))]).append("  "); 
			   sb.append(dayFormat).append("  ");
			   sb.append(String.valueOf(calendar.get(Calendar.YEAR)));
			   stringCurrentDate=sb.toString();
			   currentDate.setText(stringCurrentDate);	
		
	}
	
	
	/**
	 * Initialize the class with a set of attributes defined in the XML Layout.
	 * @param context
	 */
	private void init(Context context,Calendar myCalendar) {
		
		if(myCalendar==null){
			
			calendar = Calendar.getInstance();
			
		}else{
			
			calendar=myCalendar;
		}
		
		this.init(context);
		
	}

	
	private void init(Context context){
		
		LayoutInflater inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		inflated = (ViewGroup) inflater.inflate(R.drawable.date_time_picker, null);
		this.addView(inflated);
		
		background = (TextView) findViewById(R.id.label_background);
		dateLabel = (TextView) findViewById(R.id.label_text_bold);
		currentDate = (TextView) findViewById(R.id.label_text_date_time);
				
		this.updateScreen();
	      
		datePickerDialog = new DatePickerDialog(context,this);
		datePickerDialog.setFocusable(true);
		datePickerDialog.setFocusableInTouchMode(true);
		
		dateLabel.setText("Date");
		background.setOnClickListener(clickListener);
		
	}
	
	
	private OnClickListener clickListener = new OnClickListener() {
		
		public void onClick(View v) {
			// go to the real Date Picker selector
			datePickerDialog.show();
			
		}
	};
	
	/**
	 * Return current Time
	 * @return
	 */
    public String getCurrentDate(){
		
		return stringCurrentDate;
		
	}
	
	
	
	public void setYear(int year){
	
		YEAR = year;
		
	}

	public Calendar getCalendar() {
		return calendar;
	}

	public void setCalendar(Calendar myCalendar) {
		this.calendar=myCalendar;
		datePickerDialog.setCurrentTime();
		this.updateScreen();
	}
	
	
	/**
	 * label of Date Picker   
	 * @param text
	 */
	public void setDateLabel(String text){
		
		this.dateLabel.setText(text);
		
	}
	

	public void setStartAndEndYear(int startYear,int endYear){
    	START_YEAR=startYear;
    	END_YEAR=endYear;
    	datePickerDialog.setStartAndEndYear(startYear, endYear);
	    
    }
     
	
	
}