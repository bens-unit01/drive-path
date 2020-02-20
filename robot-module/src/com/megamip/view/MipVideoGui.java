package com.megamip.view;

import com.megamip.voice.R;

import android.content.Context;
import android.view.LayoutInflater;
import android.widget.FrameLayout;
import android.widget.RelativeLayout;

public class MipVideoGui extends FrameLayout {
	
	RelativeLayout mipVideoView;

	public MipVideoGui(Context context) {
		super(context);
		init();
	}
	
	private void init()
	{
	   
	    LayoutInflater inflator = (LayoutInflater)getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);
	    mipVideoView = (RelativeLayout) inflator.inflate(R.layout.mip_video_player, this,false);
	    setWillNotDraw(false);
	    addView(mipVideoView);
	    

	}

}
