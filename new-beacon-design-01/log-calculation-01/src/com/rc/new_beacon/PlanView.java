package com.rc.new_beacon;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

public class PlanView extends View{
	
	static String TAG ="PlanView"; 
	
	private int mX = 1200, mY = 1200; 
    private Paint mPaint = new Paint();
    private int mXbase, mYbase; 
	
    public PlanView(Context context) {
		super(context);
		initPlanView();
	}
 
	public PlanView(Context context, AttributeSet attrs) {
		super(context, attrs);
		initPlanView();
	}

	private void initPlanView() {
	
		//mX = getWidth() / 2; 
		//mX = getHeight(); 
		//invalidate();
	}

	public PlanView(Context context, AttributeSet attrs, int defaultStyle) {
		super(context, attrs, defaultStyle);
		initPlanView();
	}	
	

	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		
        mXbase = getWidth() / 2;
        mYbase = getHeight();
        int radius;
        radius = 20;
        mPaint.setStyle(Paint.Style.FILL);
        mPaint.setColor(Color.WHITE);
        canvas.drawPaint(mPaint);
        // Use Color.parseColor to define HTML colors
        mPaint.setColor(Color.parseColor("#CD5C5C"));
        canvas.drawCircle(mX, mY, radius, mPaint);
        
        mPaint.setColor(Color.parseColor("#6AB4CC"));
        canvas.drawCircle( mXbase,  mYbase, radius, mPaint);
 
        mPaint.setColor(Color.BLACK);
        mPaint.setStrokeWidth(4); 
        canvas.drawLine(mX, mY, mXbase, mYbase, mPaint);
        Log.d(TAG,"onDraw x _ y:" +  mXbase + " _  " + mYbase); 
	
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
	    Log.d(TAG, "touch event ..." + event.getRawX() + " " + event.getRawY()); 
	    invalidate();
	   return super.onTouchEvent(event);
	}
	
	public void setCoords(int x, int y){
		
	    mX = x + mXbase;
	    mY = -y + mYbase; 
	    invalidate(); 
	}
}
