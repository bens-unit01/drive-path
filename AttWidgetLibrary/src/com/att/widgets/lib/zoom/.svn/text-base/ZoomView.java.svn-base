package com.att.widgets.lib.zoom;

import java.util.Vector;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.widget.ImageView;

public class ZoomView extends ImageView{
	private static final float STEP = 0.05f;
	private Paint mPaint = new Paint(Paint.FILTER_BITMAP_FLAG);
	
	private Bitmap mImage;
	private Rect mRectSrc = new Rect();
    private Rect mRectDst = new Rect();
    
    private float mx, my;
    float leftPos, topPos;
    
    private float zoomX = 0;
    
    //Pre-calculated aspect quotient
    private float mAspectQuotient;
    
    // State of the zoom
    private ZoomViewState mState;
    
    // Layer points
    private Vector<LayerPoint> layerPoints;

	
    public ZoomView(Context context){
    	super(context);
    }
    
	public ZoomView(Context context, AttributeSet attrs) {
		super(context, attrs);	
	}
	
	public void setImage(int resource){
		mState = new ZoomViewState();
		resetZoomState();
		mImage = BitmapFactory.decodeResource(getResources(), resource);

		calculateAspectQuotient();
	}
	
	public void addLayerPoint(LayerPoint point){
		if(layerPoints == null){
			layerPoints = new Vector<LayerPoint>();
		}
		layerPoints.add(point);
	}
	
	public void removeLayerPoint(LayerPoint point){
		if(layerPoints != null){
			layerPoints.remove(point);
		}
	}
	
	private void resetZoomState() {
		mState.setPanX(0.5f);
		mState.setPanY(0.5f);
		mState.setZoom(1f);
    }
	
    private void calculateAspectQuotient() {
        if (mImage != null) {
            mAspectQuotient = (((float)mImage.getWidth()) / mImage.getHeight())
                    / (((float)getWidth()) / getHeight());
        }
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
    	super.onSizeChanged(w, h, oldw, oldh);
    	calculateAspectQuotient();
    }

	@Override
	public boolean onTouchEvent(MotionEvent event) {
        final int action = event.getAction();
        final float x = event.getX()/(getZoom()) * (mImage.getWidth() / (float)getWidth());
        final float y = event.getY()/(getZoom()) * (mImage.getHeight() / (float)getHeight());

        switch (action) {
            case MotionEvent.ACTION_DOWN:
                mx = x;
                my = y;
                break;

            case MotionEvent.ACTION_MOVE:
                final float dx = (x - mx) / getWidth();
                final float dy = (y - my) / getHeight();

                mState.setPanX(mState.getPanX() - dx);
               	mState.setPanY(mState.getPanY() - dy);
               	invalidate();
                
                mx = x;
                my = y;
                break;
                
            case MotionEvent.ACTION_UP:
            	//TODO: aca ver si la posicion x e y es la misma q en la acttion_down si es asi ver si hay algun layer en esa pos y disparar el listener
            	break;
        }
        

        return true;
	}
	
	private void controlBorders() {
		//Set left position
		float panxMin, panxMax;
		float panyMin, panyMax;
		
		final float zoomX = mState.getZoomX(mAspectQuotient) * getWidth() / mImage.getWidth();
		float w = mImage.getWidth() * zoomX;
		panxMin = 0.5f - (w - getWidth())/(2*w);
		panxMax = 0.5f + (w - getWidth())/(2*w);
		
		if(panxMin > 0.5f || panxMax < 0.5f){
			mState.setPanX(0.5f);
		}else if(mState.getPanX()< panxMin){
			mState.setPanX(panxMin);
		}else if(mState.getPanX() > panxMax){
			mState.setPanX(panxMax);
		}
		
		final float zoomY = mState.getZoomY(mAspectQuotient) * getHeight() / mImage.getHeight();
		float h = mImage.getHeight() * zoomY;
		panyMin = 0.5f - (h - getHeight())/(2*h);
		panyMax = 0.5f + (h - getHeight())/(2*h);
		
		if(panyMin > 0.5f || panyMax < 0.5f){
			mState.setPanY(0.5f);
		}else if(mState.getPanY()< panyMin){
			mState.setPanY(panyMin);
		}else if(mState.getPanY() > panyMax){
			mState.setPanY(panyMax);
		}

	}
	
	@Override
    protected void onDraw(Canvas canvas) {
		//calculateAspectQuotient();
        if (mImage != null && mState != null) {
        	 controlBorders();
            final int viewWidth = getWidth();
            final int viewHeight = getHeight();
            final int bitmapWidth = mImage.getWidth();
            final int bitmapHeight = mImage.getHeight();

            final float panX = mState.getPanX();
            final float panY = mState.getPanY();
            final float zoomX = mState.getZoomX(mAspectQuotient) * viewWidth / bitmapWidth;
            final float zoomY = mState.getZoomY(mAspectQuotient) * viewHeight / bitmapHeight;

            // Setup source and destination rectangles
            mRectSrc.left = (int)(panX * bitmapWidth - viewWidth / (zoomX * 2));
            mRectSrc.top = (int)(panY * bitmapHeight - viewHeight / (zoomY * 2));
            mRectSrc.right = (int)(mRectSrc.left + viewWidth / zoomX);
            mRectSrc.bottom = (int)(mRectSrc.top + viewHeight / zoomY);
            mRectDst.left = getLeft();
            mRectDst.top = getTop();
            mRectDst.right = getRight();
            mRectDst.bottom = getBottom();

            // Adjust source rectangle so that it fits within the source image.
            if (mRectSrc.left < 0) {
                mRectDst.left += -mRectSrc.left * zoomX;
                mRectSrc.left = 0;
            }
            if (mRectSrc.right > bitmapWidth) {
                mRectDst.right -= (mRectSrc.right - bitmapWidth) * zoomX;
                mRectSrc.right = bitmapWidth;
            }
            if (mRectSrc.top < 0) {
                mRectDst.top += -mRectSrc.top * zoomY;
                mRectSrc.top = 0;
            }
            if (mRectSrc.bottom > bitmapHeight) {
                mRectDst.bottom -= (mRectSrc.bottom - bitmapHeight) * zoomY;
                mRectSrc.bottom = bitmapHeight;
            }

            canvas.drawBitmap(mImage, mRectSrc, mRectDst, mPaint);
            
            System.out.println("ZOOM X: " + zoomX + " ZOOM Y: " + zoomY + " panx: " + panX + " pany: " + panY);
            System.out.println("surce w: " + (mRectSrc.right - mRectSrc.left) + " h: " + (mRectSrc.bottom - mRectSrc.top) );
            
            if(layerPoints != null){          
        		float w = bitmapWidth * zoomX;
        		float h = bitmapHeight * zoomY;
        		
        		float mW = viewWidth / 2.0f;
        		float mH = viewHeight / 2.0f;
        		
        		for(int i=0; i < layerPoints.size(); i++){
        			LayerPoint p = layerPoints.get(i);
        			// calculate the x & y position in relation with the center
        			float x = mW // the horizontal center
        					+ ((p.x / (float)bitmapWidth - 0.5f) * w) // the horizontal point position 
        					+ (0.5f - panX) * w 	// used to move the image horizontally from the center
        					- p.image.getWidth()/2;	// center the width image
        			
        			float y = mH // the vertical center 
        					+ ((p.y / (float)bitmapHeight - 0.5f) * h) // the vertical point position 
        					+(0.5f - panY) * h // used to move the image vertically from the center
        					- p.image.getHeight()/2; // center the height image
        			
        			if(x >= 0 - p.image.getWidth() && y >= 0 - p.image.getHeight()
        					&& x <= viewWidth && y <= viewHeight){
        				canvas.drawBitmap(p.image, x, y, null);
        			}
        		}            
            }
            
        }
    }
	
	private boolean isOnScreen(LayerPoint point){
		boolean onScreen = true;
		if(point != null){
			
		}
		return onScreen;
	}

	public void zoomIn() {
		setZoomX(zoomX + 1);
	}

	public void zoomOut() {
		setZoomX(zoomX - 1);
	}

	private float getZoom(){
		return 1 + zoomX * STEP;
	}

	public void setZoomX(float progress) {
		if(mImage == null) return;		

		zoomX = progress;
		mState.setZoom(getZoom());
		invalidate();
	}

	public float getStep(){
		return STEP;
	}
	
//	public class LayerPoint {
//		int x, y;
//		Bitmap image;
//		
//		public LayerPoint(int x, int y, int resid) {
//			setPosition(x, y);
//			setImageResource(resid);
//		}		
//		
//		public void setImageResource(int resid) {
//			image = BitmapFactory.decodeResource(getResources(), resid);
//		}
//		
//		public void setPosition(int x, int y){
//			this.x = x;
//			this.y = y;
//		}
//	}
	
}
