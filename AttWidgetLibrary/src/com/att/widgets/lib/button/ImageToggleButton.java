package com.att.widgets.lib.button;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.view.View;

import com.att.widgets.lib.R;

/**
 * The Image Toggle Button widget is a simple UI item that remains active after it is
 * pressed until it is pressed again. It has four states: enabled, pressed, active, and
 * disabled.
 */
public class ImageToggleButton extends StaticTextToggleButton {
	
	protected Drawable image;
	
	/**
	 * Creates a ImageToogleButton widget	
	 * @param context
	 */
	public ImageToggleButton(Context context) {
		super(context);
	}

	/**
	 * Creates an ImageToggleButton Widget with a set of attributes.
	 * @param context
	 * @param attrs
	 */
	public ImageToggleButton(Context context, AttributeSet attrs) {
		super(context, attrs);
	}
	
	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		setMeasuredDimension(image.getMinimumWidth(), image.getMinimumHeight());
	}
	
	/**
	 * Initialize the class.
	 * @param context
	 */
	protected void init(Context context){
		super.init(context);
		image = getResources().getDrawable(R.drawable.togglebutton_style);
		setBackgroundDrawable(image);
	}
	
	public void setBackgroundResource(int resid) {
		super.setBackgroundResource(resid);
		image = getBackground();
	}
	
	public void setBackgroundDrawable(Drawable d) {
		super.setBackgroundDrawable(d);
		image = d;
	}
	
	/**
	 * Set the text of the {@link ImageToggleButton}.
	 * Only apply when it's part of a {@link SegmentedTextToggleButton}
	 * @param s
	 */
	public void setText(String s){
		super.setText(s);
	}

	public void setEnabled(boolean enabled) {
		super.setEnabled(enabled);
	}
	
}
