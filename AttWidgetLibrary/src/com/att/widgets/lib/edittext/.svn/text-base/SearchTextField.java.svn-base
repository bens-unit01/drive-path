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
 * {@link SearchTextField} is a compose view
 * with a {@link EditText} and two {@link ImageButton},
 * one to search and the other to clean the text.
 */
public class SearchTextField extends RelativeLayout {

	private ImageButton closeImage;
	private ImageButton searchAction;
	private EditText editText;
	
	/**
	 * Creates a SearchTextField Widget.
	 * @param context
	 */
	public SearchTextField(Context context) {
		super(context);
		init(context);
		
	}

	/**
	 * Creates a SearchTextField Widget with a set of attributes.
	 * @param context
	 * @param attrs
	 */
	public SearchTextField(Context context, AttributeSet attrs) {
		super(context, attrs);
		init(context);
	}

	/**
	 * Creates a SearchTextField Widget with a set of attributes and a defined style.
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public SearchTextField(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init(context);
	}

	/**
	 * This method initialize and inflate the resource xml that defines this widget.
	 * @param context
	 */
	private void init(Context context)  {
		
		LayoutInflater inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		ViewGroup inflated = (ViewGroup) inflater.inflate(R.drawable.search_edit_text, null);
		this.addView(inflated);
		editText  = (EditText) findViewById(R.id.edit_text);
		closeImage = (ImageButton) findViewById(R.id.clear_button);
		closeImage.setOnClickListener(clearClick);
		
		searchAction = (ImageButton) findViewById(R.id.search_button);
	}


	public ImageButton getSearchAction() {
		return searchAction;
	}

	/**
	 * Set the enabled state of this view.
	 * @param enabled
	 */
	public void setEnabled(boolean enabled) {
		
		editText.setEnabled(enabled);
		closeImage.setEnabled(enabled);
		searchAction.setEnabled(enabled);
	}
	
	/**
	 * Set whether this view can receive the focus. Setting this to false will also ensure that this view is not focusable in touch mode.
	 * @param focusable
	 */
	public void setFocusable(boolean focusable ) {
		
		editText.setFocusable(focusable);
		closeImage.setFocusable(focusable);
		closeImage.setEnabled(focusable);
		searchAction.setFocusable(focusable);
		searchAction.setEnabled(focusable);
	}

	/**
	 * Set the text to the {@link EditText}
	 * @param msg
	 */
	public void setText(String msg) {
		editText.setText(msg);
	}
	
	/**
	 * Get the text on the {@link EditText}
	 * @return
	 */
	public String getText() {
		return editText.getText().toString();
	}

	
	// Create an anonymous implementation of OnClickListener to put into clear button.
	private OnClickListener clearClick = new OnClickListener() {
	    public void onClick(View v) {
	    	
	    	editText.setText("");
	    }
	};
	
	@Override
	public void setOnClickListener(View.OnClickListener l) {
		searchAction.setOnClickListener(l);
		
	}
	
	@Override
	public void setOnKeyListener(OnKeyListener l) {
		editText.setOnKeyListener(l);		
	}

}
