package com.att.widgets.lib.edittext;

import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.RelativeLayout;

import com.att.widgets.lib.R;

/**
 * {@link SimpleTextField} is a compose view
 * with a {@link EditText} and a {@link ImageButton}
 * to clear the text.
 */
public class SimpleTextField extends RelativeLayout implements View.OnClickListener {

	private ImageButton imageButton;
	private EditText editText;
	
	
	/**
	 * Creates a SimpleTextField Widget.
	 * @param context
	 */
	public SimpleTextField(Context context) {
		super(context);
		init(context);
		
	}

	/**
	 * Creates a SimpleTextField Widget with a defined Style.
	 * @param context
	 * @param attrs
	 */
	public SimpleTextField(Context context, AttributeSet attrs) {
		super(context, attrs);
		init(context);
	}

	/**
	 * Creates a SimpleTextField Widget with a defined Style and a defined style.
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public SimpleTextField(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init(context);
	}

	/**
	 * This method initialize and inflate the resource xml that defines this widget.
	 * @param context
	 */
	private void init(Context context)  {
		
		LayoutInflater inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		ViewGroup inflated = (ViewGroup) inflater.inflate(R.drawable.close_edit_text, null);
		this.addView(inflated);
		editText  = (EditText) findViewById(R.id.edit_text);
		imageButton = (ImageButton) findViewById(R.id.clear_button);
		imageButton.setOnClickListener(this);
	}


	/**
	 * Set the enabled state of this view.
	 * @param enabled
	 */
	public void setEnabled(boolean enabled) {
		editText.setEnabled(enabled);
		imageButton.setEnabled(enabled);
	}
	
	/**
	 * Set whether this view can receive the focus. Setting this to false will also ensure that this view is not focusable in touch mode.
	 * @param focusable
	 */
	public void setFocusable(boolean focusable ) {
		
		editText.setFocusable(focusable);
		imageButton.setFocusable(focusable);
		imageButton.setEnabled(focusable);
	}

	
	public void setText(String msg) {
		editText.setText(msg);
	}
	
	public String getText() {
		return editText.getText().toString();
	}
	
	/**
	 * Clear the text.
	 * @param arg0
	 */
	public void onClick(View arg0) {
		editText.setText("");	
	}
	

	
	
	
	
}
