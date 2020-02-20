package com.att.widgets.lib.control;
		
import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Color;
import android.util.AttributeSet;
import android.widget.CheckBox;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.att.widgets.lib.R;

public class SwitchControl extends LinearLayout {

	
	private TextView offLabel;
	private CheckBox checkBox;
	private TextView onLabel;
	
	/**
	 * Creates a SwitchControl Widget with a set of attributes.
	 * @param context
	 * @param attrs
	 */
	public SwitchControl(Context context, AttributeSet attrs) {
		super(context, attrs);
		init(attrs);
	}


	/**
	 * Creates a SwitchControl Widget. 
	 * @param context
	 */
	public SwitchControl(Context context) {
		super(context);
		init();
	}

	/**
	 * Sets the checked state.
	 * @param checked
	 */
	public void setChecked(boolean checked) {
		checkBox.setChecked(checked);
	}
	
	@Override
	public void setEnabled(boolean enabled) {
		super.setEnabled(enabled);
		checkBox.setEnabled(enabled);
		setTextColor(enabled);
	}
	
	@Override
	public boolean isEnabled() {
		return checkBox.isEnabled();
	}
	
	/**
	 * Initialize the class.
	 * @param attrs
	 */
	private void init(AttributeSet attrs) {
		TypedArray a = getContext().obtainStyledAttributes(attrs, R.styleable.SwitchControl);
		boolean enabled = a.getBoolean(R.styleable.SwitchControl_switch_enabled, true);
		boolean checked = a.getBoolean(R.styleable.SwitchControl_switch_checked, false);
		String leftLabel = a.getString(R.styleable.SwitchControl_switch_left_label);
		String rightLabel = a.getString(R.styleable.SwitchControl_switch_right_label);
		
		offLabel = new TextView(getContext());
		offLabel.setText(leftLabel);
		offLabel.setPadding(0, 0, 5, 0);
		checkBox = new CheckBox(getContext());
		checkBox.setButtonDrawable(R.drawable.switch_button);
		checkBox.setEnabled(enabled);
		checkBox.setChecked(checked);
		checkBox.setWidth(80);
		onLabel = new TextView(getContext());
		onLabel.setText(rightLabel);
		onLabel.setPadding(5, 0, 0, 0);
		setTextColor(isEnabled());
		
		addView(offLabel);
		addView(checkBox);
		addView(onLabel);
	}
	
	/**
	 * Sets the color of the Widget.
	 * @param enabled
	 */
	private void setTextColor(boolean enabled){
		if(enabled){
			offLabel.setTextColor(Color.BLACK);
			onLabel.setTextColor(Color.BLACK);
		}else{
			offLabel.setTextColor(Color.GRAY);
			onLabel.setTextColor(Color.GRAY);
		}
	}
	
	private void init() {
		
	}
	
	/**
	 * Gets the off label.
	 * @return
	 */
	public TextView getOffLabel() {
		return offLabel;
	}
	
	/**
	 * Sets the off label.
	 * @param offLabel
	 */
	public void setOffLabel(TextView offLabel) {
		this.offLabel = offLabel;
	}
	
	/**
	 * Gets the on label.
	 * @return
	 */
	public TextView getOnLabel() {
		return onLabel;
	}
	
	/**
	 * Sets the on label.
	 * @param onLabel
	 */
	public void setOnLabel(TextView onLabel) {
		this.onLabel = onLabel;
	}

	/**
	 * Gets the CheckBox of the Widget.
	 * @return
	 */
	public CheckBox getCheckBox() {
		return checkBox;
	}

	/**
	 * Sets the CheckBox of the Widget.
	 * @param checkBox
	 */
	public void setCheckBox(CheckBox checkBox) {
		this.checkBox = checkBox;
	}

	/**
	 * Returns if the SwitchControl is checked or not.
	 * @return
	 */
	public boolean isChecked() {
		return checkBox.isChecked();
	}
}