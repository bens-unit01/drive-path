package com.megamip.view;

import com.megamip.voice.R;

import android.content.Context;
import android.graphics.Canvas;
import android.util.Log;
import android.view.LayoutInflater;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

public class CarouselGui extends FrameLayout{
	
	FrameLayout mCarouselGui;
	
	final String TAG  = CarouselGui.class.getName();

	public CarouselGui(Context context) {
		super(context);
		init();
		
	}
	
	private void init()
	{
	   
	    LayoutInflater inflator = (LayoutInflater)getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);
	    mCarouselGui = (FrameLayout) inflator.inflate(R.layout.activity_carousel, this,false);
	    setWillNotDraw(false);
	    addView(mCarouselGui);
	    

	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
	}

}
