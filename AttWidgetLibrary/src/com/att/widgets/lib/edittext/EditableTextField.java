package com.att.widgets.lib.edittext;

import android.content.Context;
import android.content.res.TypedArray;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.att.widgets.lib.R;

/**
 * The {@link EditableTextField} is a compose view that
 * have a {@link TextView} to set a label and a
 * {@link EditText} to enter and edit the text.
 * Also when is enabled have a cross button on the
 * right bottom corner to clean the text.
 */
public class EditableTextField extends RelativeLayout implements View.OnClickListener {

	private ImageButton closeImage;
	private boolean removeButton;
	private EditText editText;
	private TextView textView;
	private ViewGroup inflated;

	private String text;
	private String textLabel;
	private boolean enabled = true;
	
	/**
	 * Creates an EditableTextField Widget.
	 * @param context
	 */
	public EditableTextField(Context context) {
		super(context);
		init(context);
	}
	
	/**
	 * Creates an EditableTextField Widget with a set of attributes.
	 * The default of maximum lines the Widget can expand is four unless the user change it manually.
	 * @param context
	 * @param attrs
	 */
	public EditableTextField(Context context, AttributeSet attrs) {
		super(context, attrs);
		init(context,attrs);
	}
	

	/**
	 * Creates an EditableTextField Widget withe a defined style given by the user.
	 * The default of maximum lines the Widget can expand is four unless the user change it manually.
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public EditableTextField(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init(context,attrs);
	}

	/**
	 * Initialize an EditableTextField with a set of attributes.
	 * The default of maximum lines the Widget can expand is four unless the user change it manually.
	 * @param context
	 * @param attrs
	 */
	private void init(Context context, AttributeSet attrs) {
		TypedArray a = getContext().obtainStyledAttributes(attrs, R.styleable.LabelEditText);
		textLabel = a.getString(R.styleable.LabelEditText_label_text_label);
		text = a.getString(R.styleable.LabelEditText_label_text_text);
		enabled = a.getBoolean(R.styleable.LabelEditText_label_text_enabled,true);
		if(text == null)
			text="";
		if(textLabel == null)
			textLabel="";
		init(context);
	}

	/**
	 * This method initialize and inflate the resource xml that defines this widget.
	 * The default of maximum lines the Widget can expand is four.
	 * @param context
	 */
	private void init(Context context)  {
		
		LayoutInflater inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		inflated = (ViewGroup) inflater.inflate(R.drawable.label_edit_text, null);
		this.addView(inflated);
		
		textView = (TextView) findViewById(R.id.label_text);
		editText = (EditText) findViewById(R.id.edit_text);
		closeImage = (ImageButton) findViewById(R.id.clear_button);
		
		textView.setText(textLabel);
		editText.setText(text);
		editText.setEnabled(enabled);
		
		editText.setMaxLines(4);
		closeImage.setOnClickListener(this);
		removeButton = false;
	}

	/**
	 * Set the label text on the
	 * {@link TextView}
	 * @param label
	 */
	public void setLabelText(String label) {
		textView.setText(label);
	}
	
	/**
	 * Set the text on the
	 * {@link EditText}
	 * @param msg
	 */
	public void setText(String msg) {
		editText.setText(msg);
	}
	
	/**
	 * Return the label text
	 * @return
	 */
	public String getLabelText() {
		return textView.getText().toString();
	}
	
	/**
	 * Return the text on the {@link EditText}
	 * @param msg
	 * @return
	 */
	public String getText() {
		return editText.getText().toString();
	}
	
	/**
	 * Set the {@link EditableTextField}
	 * state to enabled or disabled
	 * @param enabled
	 */
	public void setEnabled(boolean enabled) {
		if(!enabled) {
			closeImage.setVisibility(GONE);
			removeButton = true;
		}else if(removeButton) {
			closeImage.setVisibility(VISIBLE);
			removeButton = false;
		}
		editText.setEnabled(enabled);
		textView.setEnabled(enabled);
	}
	
	/**
	 * Set the {@link EditableTextField}
	 * to be focusabled ot not.
	 */
	public void setFocusable(boolean focusable) {
		editText.setFocusable(focusable);
		textView.setFocusable(focusable);
		closeImage.setFocusable(focusable);
	}
	
	public void onClick(View v) {
		editText.setText("");
	}
	
	/**
	 * Set the number of max lines
	 * to be expanded the {@link EditText}
	 * @param lines
	 */
	public void setMaxLines(int lines){
		editText.setMaxLines(lines);
	}
}
