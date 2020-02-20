package com.att.widgets.lib.datetime;

import java.util.Calendar;

import android.app.Dialog;
import android.content.Context;
import android.view.View;

import com.att.widgets.lib.R;
import com.att.widgets.lib.button.StaticTextButton;

/**
 * A view for selecting a month / day / year based on a calendar like layout.
 */

public class DatePickerDialog extends Dialog implements android.view.View.OnClickListener {
	
	private String format= String.format("%%0%dd", 2);
	private WheelView years;
	private WheelView months;
	private WheelView days;
	private StaticTextButton buttonPicker;
	private String months_string[] = new String[] {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sept","Oct","Nov","Dec"};
	public  String stringCurrentDate;
	private DatePicker datePicker;
    private int currentDay;
	/**
	 * Creates a DatePicker Dialog Widget. This Widget is the date picker itself and is part of
	 * the DatePicker Widget.
	 * @param context
	 */
	public DatePickerDialog(Context context,DatePicker dp) {
		super(context,R.style.dialog);
		datePicker=dp;
		init(context);
		
	}

	private void init(Context context)  {
		
		setContentView(R.drawable.date_picker);
		
		initWheelDays(R.id.days);
        initWheelYear(R.id.years);
        initWheelMonths(R.id.months);
       
        
        
        this.setCurrentTime();
        
        buttonPicker = (StaticTextButton) findViewById(R.id.primary_button_time_picker);
        buttonPicker.setType(StaticTextButton.PRIMARY_BUTTON_HIGHLIGHTED);
        buttonPicker.setTextColor(0xFFFFFFFF);
        buttonPicker.setOnClickListener(this);		
	}
	
				
	
	@SuppressWarnings("static-access")
	public void setCurrentTime(){
		
        years.setCurrentItem(datePicker.calendar.get(datePicker.calendar.YEAR)-datePicker.START_YEAR);
        months.setCurrentItem(datePicker.calendar.get(datePicker.calendar.MONTH));
        days.setCurrentItem(datePicker.calendar.get(datePicker.calendar.DAY_OF_MONTH)-1);
		
	}
	
	
	/**
	 * Sets if this Widget will be focusable by touch inputs.
	 * @param focusableInTouchMode
	 */
	public void setFocusableInTouchMode(boolean focusableInTouchMode) {

		days.setFocusableInTouchMode(focusableInTouchMode);
		years.setFocusableInTouchMode(focusableInTouchMode);
		months.setFocusableInTouchMode(focusableInTouchMode);
		days.setFocusable(focusableInTouchMode);
		years.setFocusable(focusableInTouchMode);
		months.setFocusable(focusableInTouchMode);
	}
	

	/**
	 *	Initialize all the days in the Widget.
	 * @param id
	 */
	private void initWheelDays(int id) {
        days = getWheel(id);
        days.setImg_wheel(WheelView.DATE_PICKER_MIDDLE_IMG);
     	days.setAdapter(new NumericWheelAdapter(1,31,format));
        days.addChangingListener(changedListener);
        days.addScrollingListener(scrolledListener);
        days.setCyclic(true);
        
    }

	/**
	 *	Initialize all the years in the Widget.
	 * @param id
	 */
    private void initWheelYear(int id) {
	    years = getWheel(id);
	    years.setImg_wheel(WheelView.DATE_PICKER_RIGTH_IMG); 
	    years.setAdapter(new NumericWheelAdapter(datePicker.START_YEAR,datePicker.END_YEAR));
	    years.addChangingListener(new OnWheelChangedListener() {
 			public void onChanged(WheelView wheel, int oldValue, int newValue) {
 				currentDay=days.getCurrentItem();
 				days.setAdapter(DatePickerDialog.this.daysOfMonths(years,months));
 				days.setCurrentItem(DatePickerDialog.this.maxDayOfMonth(currentDay));
 			}
 		  });  
	    years.addScrollingListener(scrolledListener02);
	    years.setCyclic(true);
    }
    
    
    /**
     * this method set custom inverval for wheel years
     * @param startYear
     * @param endYear
     */
    @SuppressWarnings("static-access")
	public void setStartAndEndYear(int startYear,int endYear){
    	
    	years.setAdapter(new NumericWheelAdapter(datePicker.START_YEAR,datePicker.END_YEAR));
    	years.setCurrentItem(datePicker.calendar.get(datePicker.calendar.YEAR)-datePicker.START_YEAR);
	    
    }
    
	/**
	 *	Initialize all the months in the Widget.
	 * @param id
	 */
    private void initWheelMonths(int id){
    	 
    	 months = getWheel(id); 
    	 months.setImg_wheel(WheelView.DATE_PICKER_LEFT_IMG);
    	 months.setAdapter(new ArrayWheelAdapter<String>(months_string));
    	 months.addChangingListener(new OnWheelChangedListener() {
 			public void onChanged(WheelView wheel, int oldValue, int newValue) {
 				currentDay=days.getCurrentItem();
 				days.setAdapter(DatePickerDialog.this.daysOfMonths(years,months));
 				days.setCurrentItem(DatePickerDialog.this.maxDayOfMonth(currentDay));
 			}
 		  }); 
    	 months.addScrollingListener(scrolledListener03);
 	     months.setCyclic(true);
 	     days.invalidate();
    	
        } 
	    
    /**
     * Returns wheel by Id
     * @param id the wheel Id
     * @return the wheel with passed Id
     */
    private WheelView getWheel(int id) {
    	return (WheelView) findViewById(id);
    }
	    
	    
	
	/**
	 * Set the enabled state of this view.
	 * @param enabled
	 */
	public void setEnabled(boolean enabled) {
		
		months.setEnabled(enabled);
		years.setEnabled(enabled);
		days.setEnabled(enabled);
	}
	
	/**
	 * Set whether this view can receive the focus. Setting this to false will also ensure that this view is not focusable in touch mode.
	 * @param focusable
	 */
	public void setFocusable(boolean focusable ) {
		
		months.setFocusable(focusable);
		months.setEnabled(focusable);
		days.setFocusable(focusable);
		days.setEnabled(focusable);
		years.setFocusable(focusable);
		years.setEnabled(focusable);
		
	}

	
	 // Wheel scrolled flag
    private boolean wheelScrolled = false;
	
	 // Wheel scrolled listener
    OnWheelScrollListener scrolledListener = new OnWheelScrollListener() {
        public void onScrollingStarted(WheelView wheel) {
            wheelScrolled = true;
        }
        public void onScrollingFinished(WheelView wheel) {
            wheelScrolled = false;
        }
    };
    
    // Wheel changed listener
    private OnWheelChangedListener changedListener = new OnWheelChangedListener() {
        public void onChanged(WheelView wheel, int oldValue, int newValue) {
            if (!wheelScrolled) {
            }
        }
    };
    
    
	
	 // Wheel scrolled listener
   OnWheelScrollListener scrolledListener02 = new OnWheelScrollListener() {
       public void onScrollingStarted(WheelView wheel) {
           wheelScrolled = true;
       }
       public void onScrollingFinished(WheelView wheel) {
           wheelScrolled = false;
       }
   };
   
   // Wheel changed listener
   @SuppressWarnings("unused")
   private OnWheelChangedListener changedListener02 = new OnWheelChangedListener() {
       public void onChanged(WheelView wheel, int oldValue, int newValue) {
           if (!wheelScrolled) {
           }
       }
   };
   
	
	 // Wheel scrolled listener
   OnWheelScrollListener scrolledListener03 = new OnWheelScrollListener() {
      public void onScrollingStarted(WheelView wheel) {
          wheelScrolled = true;
      }
      public void onScrollingFinished(WheelView wheel) {
          wheelScrolled = false;
      }
   };
  

	
   /**
    * TODO // add comments
    * @param viewYear
    * @param viewMonth
    * @return
    */
   private NumericWheelAdapter daysOfMonths(WheelView viewYear ,WheelView viewMonth){
	  datePicker.calendar.set(viewYear.getCurrentItem()+ datePicker.START_YEAR, viewMonth.getCurrentItem(),01);
	  int days = datePicker.calendar.getActualMaximum(Calendar.DAY_OF_MONTH); // 28
	  return new NumericWheelAdapter(1,days,format);
   }

  
   /**
    * This method validates the last day of all the months.
    * @param currentDay
    * @return
    */
   private int maxDayOfMonth(int currentDay){
	  
	  int maxDay = datePicker.calendar.getActualMaximum(Calendar.DAY_OF_MONTH); // 28
	  
	  if(currentDay == 0){
		  
	      return maxDay;
	  }
	  if(currentDay+1 <= maxDay){
		  	
		  return currentDay;
		  
	  	}
	  
	  return maxDay-1;
   
   } 
   public void onClick(View v) {
	   datePicker.calendar.set(years.getCurrentItem()+ datePicker.START_YEAR, months.getCurrentItem(), days.getCurrentItem()+1);
	   datePicker.updateScreen();
	   datePicker.invalidate();
	   this.dismiss();
		
   }
   
}