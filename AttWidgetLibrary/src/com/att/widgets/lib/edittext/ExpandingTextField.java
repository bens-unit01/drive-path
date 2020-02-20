package com.att.widgets.lib.edittext;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.EditText;

import com.att.widgets.lib.R;

/**
 * {@link ExpandingTextField} extend from {@link EditText}
 * and add a personalize style.
 */
public class ExpandingTextField extends EditText {

	/**
	 * Creates an ExpandingTextField Widget.
	 * The default of maximum lines the Widget can expand is four unless the user change it manually.
	 * @param context
	 */
	public ExpandingTextField(Context context) {
		super(context);
		init();
	}
	
	/**
	 * Creates an ExpandingTextField Widget with a set of attributes.
	 * The default of maximum lines the Widget can expand is four unless the user change it manually.
	 * @param context
	 * @param attrs
	 */
	public ExpandingTextField(Context context, AttributeSet attrs) {
		super(context, attrs);
		init();
	}
	
	/**
	 * Creates an ExpandingTextField Widget with a set of attributes and with a defined style.
	 * The default of maximum lines the Widget can expand is four unless the user change it manually.
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public ExpandingTextField(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init();
	}

	/**
	 * Initialize the class. The default of maximum lines the Widget can expand is four.
	 */
	private void init() {
		setBackgroundResource(R.drawable.expand_edit_text_style);
		this.setMaxLines(4);
	}
	
	@Override
	public void setEnabled(boolean enabled) {
		super.setEnabled(enabled);
	}
	
	@Override
	public void setFocusable(boolean focusable) {
		super.setFocusable(focusable);
		
	}
	
}
