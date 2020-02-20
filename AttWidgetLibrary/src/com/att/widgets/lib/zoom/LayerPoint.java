package com.att.widgets.lib.zoom;

import android.graphics.Bitmap;
import android.view.View.OnClickListener;

public class LayerPoint {
	int x, y;
	Bitmap image;
	OnClickListener listener;
	
	public LayerPoint(int x, int y, Bitmap image) {
		setImage(image);
		setPosition(x, y);
	}		
	
	public void setImage(Bitmap image) {
		this.image = image;
	}
	
	public void setPosition(int x, int y){
		this.x = x;
		this.y = y;
		
	}
	
	public void setOnClickListener(OnClickListener listener){
		this.listener = listener;
	}
	
}
