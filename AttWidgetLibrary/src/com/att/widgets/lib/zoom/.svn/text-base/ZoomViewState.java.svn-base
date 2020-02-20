package com.att.widgets.lib.zoom;

public class ZoomViewState{

    private float mZoom;
    private float mPanX;
    private float mPanY;


    public float getPanX() {
        return mPanX;
    }


    public float getPanY() {
        return mPanY;
    }


    public float getZoom() {
        return mZoom;
    }


    public float getZoomX(float aspectQuotient) {
        return Math.min(mZoom, mZoom * aspectQuotient);
    }

    public float getZoomY(float aspectQuotient) {
        return Math.min(mZoom, mZoom / aspectQuotient);
    }

    public void setPanX(float panX) {
        if (panX != mPanX && panX>=0 && panX <=1){
            mPanX = panX;
        }
    }

    public void setPanY(float panY) {
        if (panY != mPanY && panY>=0 && panY <=1) {
            mPanY = panY;
        }
    }

    public void setZoom(float zoom) {
        if (zoom != mZoom) {
            mZoom = zoom;
        }
    }
}
