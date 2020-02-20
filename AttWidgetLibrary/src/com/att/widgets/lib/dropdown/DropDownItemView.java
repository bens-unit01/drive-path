package com.att.widgets.lib.dropdown;

import android.content.Context;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.att.widgets.lib.R;

/**
 * It's the view used to display the
 * items in the {@link DropDown} list.
 */
public class DropDownItemView extends LinearLayout{

	public static final int HEIGHT = 35;
	private TextView textView;
	private ImageView checkImage;
	
	public DropDownItemView(Context context) {
		super(context);
		inflate(context, R.drawable.dropdown_item_view, this);
		
		textView = (TextView)findViewById(R.id.dropdown_item_text);
		checkImage = (ImageView)findViewById(R.id.dropdown_item_check);
	}
	
	public void updateItem(String text, boolean check){
		textView.setText(text);
		checkImage.setVisibility(check?VISIBLE:INVISIBLE);
		setBackgroundDrawable(check?getResources().getDrawable(R.drawable.drop_down_pressed):null);
	}

}
