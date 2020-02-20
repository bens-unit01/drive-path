package com.att.widgets.lib.control;

import java.text.DecimalFormat;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.view.LayoutInflater;
import android.view.ViewGroup;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.SeekBar.OnSeekBarChangeListener;

import com.att.widgets.lib.R;

/**
 * A SliderControl is an extension of ProgressBar that adds a draggable thumb like a SeekBar. The user can touch the thumb and drag left or right to set the current progress level or use the arrow keys. 
 * A SliderControl has at least two Labels which indicate the minimum value of the SliderControl and its maximum value. It could contains two or more labels at the same time, indicates the values of the
 * SliderControl. Placing focusable widgets to the left or right of a SeekBar is discouraged. 
 * 
 * Clients of the SeekBar can attach a SeekBar.OnSeekBarChangeListener to be notified of the user's actions.
 */
public class SliderControl extends RelativeLayout implements OnSeekBarChangeListener {

	private float step = 0.1f;
	private int maxRange = 1000;
	private float minValue;
	private float maxValue;
	private int divideBy;
	private float current;
	private SeekBar seek;
	private Paint paint;
	private OnClickListener listener;
	private DecimalFormat decimalFormatter;
	private DisplayMetrics displayMetrics;
	private double chunk;
	
	/**
	 * Returns the current value.
	 * @return
	 */
	public float getCurrent() {
		return current;
	}

	/**
	 * Sets the current value.
	 * @param current
	 */
	public void setCurrent(float current) {
		this.current = minValue + ((current / maxRange) * (maxValue - minValue));
	}

	/**
	 * Returns the step of the SliderControl
	 * @return
	 */
	public float getStep() {
		return step;
	}

	/**
	 * Sets the step of the SliderControl. 
	 * For Example: 2.5f (float value). 
	 * @param step
	 */
	public void setStep(float step) {
		maxRange = Math.round((maxValue - minValue)/step);
		seek.setMax(maxRange);
		setUserProgressBar();
		this.step = step;
	}
	
	
	/**
	 * Creates a SliderControl Widget.
	 * @param context
	 */
	public SliderControl(Context context) {
		super(context);
		init(context);
	}
	
	/**
	 * Creates a SliderControl Widget with a set of attributes.
	 * @param context
	 * @param attrs
	 */
	public SliderControl(Context context, AttributeSet attrs) {
		super(context, attrs, android.R.attr.progressBarStyleHorizontal);
		init(context, attrs);
	}
	
	/**
	 * Initialize the class with default values.
	 * @param context
	 */
	private void init(Context context)  {
		LayoutInflater inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		ViewGroup inflated = (ViewGroup) inflater.inflate(R.drawable.slider_bar, null);
		this.addView(inflated);
		seek = (SeekBar) findViewById(R.id.seek_bar);
		setCurrent(0);
		setMaxValue(maxRange);
		setMinValue(0);
		setDivideBy(0);
		createLabels();
		seek.setOnSeekBarChangeListener(this);
	}
	
	@Override
	public void setOnClickListener(OnClickListener l) {
		this.listener = l;
	}
	
	/**
	 * Initialize the class with a set of attributes defined in the XML layout.
	 * @param context
	 * @param attrs
	 */
	private void init(Context context, AttributeSet attrs)  {
		LayoutInflater inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		ViewGroup inflated = (ViewGroup) inflater.inflate(R.drawable.slider_bar, null);
		this.addView(inflated);
		seek = (SeekBar) findViewById(R.id.seek_bar);
		seek.incrementProgressBy(1);
		this.setWillNotDraw(false);
		
		TypedArray a = getContext().obtainStyledAttributes(attrs, R.styleable.Sliderbar);
		minValue = a.getInt(R.styleable.Sliderbar_min_value, 0);
		maxValue = a.getInt(R.styleable.Sliderbar_max_value, 100);
		divideBy = a.getInt(R.styleable.Sliderbar_divide_in, 1);
		current = a.getInt(R.styleable.Sliderbar_current_value, 0);
		validateDivideBy(divideBy);
		setStep(step);
		validateCurrent(current);
		paint = new Paint();
		paint.setColor(Color.LTGRAY);
		paint.setTypeface(Typeface.DEFAULT);
		paint.setTextSize(14);
		paint.setAntiAlias(true);
		paint.setTextAlign(Paint.Align.CENTER);
		
		seek.setOnSeekBarChangeListener(this);
		
		decimalFormatter = new DecimalFormat("##.#");
		displayMetrics = new DisplayMetrics();
		chunk = ( ( maxValue - minValue ) /  (double) divideBy);
		createLabels();
	}

	/**
	 * Returns the minimum value of the SliderControl
	 * @return
	 */
	public float getMinValue() {
		return minValue;
	}

	/**
	 * Sets the minimum value of the SliderControl
	 * @param minValue
	 */
	public void setMinValue(int minValue) {
		this.minValue = minValue;
	}

	/**
	 * Returns the maximum value of the SliderControl
	 * @return
	 */
	public float getMaxValue() {
		return maxValue;
	}

	/**
	 * Sets the maximum value of the SliderControl
	 * @param maxValue
	 */
	public void setMaxValue(int maxValue) {
		this.maxValue = maxValue;
		seek.setMax(maxValue);
		
	}

	/**
	 * Returns the number of parts the SliderControl would be separated.
	 * @return
	 */
	public int getDivideBy() {
		return divideBy;
	}

	/**
	 * Sets the divideBy value, in order to separate the SliderControls in parts by the labels.
	 * For Example: if divideBy is four, the SliderControl would have five labels with they current values.
	 * @param divideBy
	 */
	public void setDivideBy(int divideBy) {
		this.divideBy = divideBy;
	}
	
	public void setEnabled(boolean enabled) {
		seek.setEnabled(enabled);
		
		if(enabled) {
			seek.setFocusable(true);
		} else {
			seek.setFocusable(false);
		}
	}
	
	public void setFocusable(boolean focusable) {
		seek.setEnabled(focusable);
	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		
		super.onDraw(canvas);
		int parts = seek.getWidth() / divideBy;
		TextView v1 = (TextView) findViewById(R.id.seekbar_label_left);
		displayMetrics.setToDefaults();

		for (int i = 1; i < divideBy; i++) {
			canvas.drawText(decimalFormatter.format(minValue + (chunk * i)), parts * i + 7,  v1.getBottom() - 4 * displayMetrics.density , paint );
		}
	}
	
	/**
	 * Creates the first and last labels, these labels correspond to the minimum and maximum values of the SliderControl.
	 */
	private void createLabels() {
		
		TextView textLeft = (TextView) findViewById(R.id.seekbar_label_left);
		TextView textRight = (TextView) findViewById(R.id.seekbar_label_right);
		textLeft.setText(decimalFormatter.format(minValue));
		textRight.setText(decimalFormatter.format(maxValue));		
	}
	
	/**
	 * Validates the current value indicates by the user. If the current value is bigger than the maximum value or
	 * is smaller than the minimum value the current value is the minimum value of the SliderControl.
	 * @param current
	 */
	private void validateCurrent(float current) {
		if(current <= maxValue && current >= minValue)  {
			
			setCurrent(((current - minValue) * maxRange / (maxValue - minValue)));
		}else {
			setCurrent(minValue);
			seek.setProgress((int)minValue);
		}
	}
	
	/**
	 * Validates the divideBy value indicates by the user. If the divideBy value is smaller or equal to 0, the dividerBy
	 * value will be one.
	 * @param divideBy
	 */
	private void validateDivideBy(int divideBy) {
		if(divideBy <= 0)  {
			setDivideBy(1);
		}else {
			setDivideBy(divideBy);
		}
	}
	
	/**
	 * Sets if the SliderControl widgets needs to refill the progress of the current value.
	 * @param fill
	 */
	public void setProgressFillColor(boolean fill){
		if(fill){
			seek.setProgressDrawable(getContext().getResources().getDrawable(R.drawable.slider_bar_fill_style));
		}else{
			seek.setProgressDrawable(getContext().getResources().getDrawable(R.drawable.slider_bar_style));
		}

		
		seek.setProgress(seek.getProgress() - 1);
		seek.setProgress(seek.getProgress() + 1);
	}


	public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
		if (listener != null){
			setCurrent(progress);
			listener.onClick(this);
		}
		
	}

	public void onStartTrackingTouch(SeekBar seekBar) {
		// TODO Auto-generated method stub
		
	}

	public void onStopTrackingTouch(SeekBar seekBar) {
		// TODO Auto-generated method stub
		
	}
	

	private void setUserProgressBar() {
		seek.setProgress((int)((current - minValue) * maxRange / (maxValue - minValue)));
	}
}
