package com.megamip.view;

/*
 * This class is inspired by the android project code at  https://android.googlesource.com/platform/packages/inputmethods/LatinIME/+/0e7cdae045b93e0370ae575072e76b6af91b6b0c/src/com/android/inputmethod/voice/RecognitionView.java
 * 
 * */

import java.util.ArrayList;
import java.util.List;

import com.megamip.voice.R;
import com.megamip.voice.R.drawable;
import com.megamip.voice.R.id;
import com.megamip.voice.R.layout;

import android.app.Dialog;
import android.content.ContentResolver;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.drawable.Drawable;
import android.os.Handler;
import android.util.Log;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.widget.ImageView;
import android.widget.TextView;
import android.provider.Settings;



public class SpeakNow extends Dialog {

    public static final String LATIN_IME_MIN_MICROPHONE_LEVEL =
	            "latin_ime_min_microphone_level";
	public static final String LATIN_IME_MAX_MICROPHONE_LEVEL =
	            "latin_ime_max_microphone_level";
	public static final String TAG1 = "A1", TAG2 = "A2", TAG3 = "A3";
	public static final String TAG4 ="A4";
	// members 
	private String mTitle;
	private Handler mUiHandler;
	private ImageView mImage;
	private List<Drawable> mSpeakNow;
	private final float mMinMicrophoneLevel;
	private final float mMaxMicrophoneLevel;
	private float mVolume = 0.0f;
	private int mLevel = 0;
	private enum State {RUNNING, STOPPING, STOPPED}
	private State mState = State.STOPPED;
	private Runnable mUpdateVolumeRunnable = new Runnable(){

		@Override
		public void run() {
		
			if(State.STOPPING == mState){
				mState = State.STOPPED;
				return;
			}
			
			final float min = mMinMicrophoneLevel;
			final float max = mMaxMicrophoneLevel;
			final int maxLevel = mSpeakNow.size() - 1;
			
			
			 int index = (int) ((mVolume - min) / (max - min) * maxLevel);
	            final int level = Math.min(Math.max(0, index), maxLevel);
	            
             Log.d(TAG4," SpeakNow mUpdateVolume... level:"+level);
             
	            if (level != mLevel) {
	                mImage.setImageDrawable(mSpeakNow.get(level));
	                mLevel = level;
	            }
	            mUiHandler.postDelayed(mUpdateVolumeRunnable, 50);
		}
		
		
	};
	
	
	
	// constructor 
	public SpeakNow(String mTitle, Context context) {
		super(context, android.R.style.Theme_Black_NoTitleBar_Fullscreen);
		this.mTitle = mTitle;
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		this.setContentView(R.layout.speak_now);
		this.getWindow().setLayout(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT);
	
	//	TextView text = (TextView) this.findViewById(R.id.text);
	//	text.setText(mTitle);
		mImage = (ImageView) this.findViewById(R.id.image);
		mImage.setImageResource(R.drawable.android__voice_search);
	
		mUiHandler = new Handler();
		ContentResolver cr = context.getContentResolver();
		mMinMicrophoneLevel = getSettingsFloat(
                cr, LATIN_IME_MIN_MICROPHONE_LEVEL, 15.f);
        mMaxMicrophoneLevel = getSettingsFloat(
                cr, LATIN_IME_MAX_MICROPHONE_LEVEL, 30.f);
        
        
        Resources resources = context.getResources();
        mSpeakNow = new ArrayList<Drawable>();
        mSpeakNow.add(resources.getDrawable(R.drawable.speak_now_level0));
        mSpeakNow.add(resources.getDrawable(R.drawable.speak_now_level1));
        mSpeakNow.add(resources.getDrawable(R.drawable.speak_now_level2));
        mSpeakNow.add(resources.getDrawable(R.drawable.speak_now_level3));
        mSpeakNow.add(resources.getDrawable(R.drawable.speak_now_level4));
        mSpeakNow.add(resources.getDrawable(R.drawable.speak_now_level5));
        mSpeakNow.add(resources.getDrawable(R.drawable.speak_now_level6));
       
	}


	private static float getSettingsFloat(ContentResolver cr, String key, float defaultValue) {
        return Settings.Secure.getFloat(cr, key, defaultValue);
    }
	
	
	
	// -------
	public void showListening(){
		mState = State.RUNNING;
		mUiHandler.post(new Runnable() {
			
			@Override
			public void run() {
			
				mImage.setImageDrawable(mSpeakNow.get(0));
				
			}
		});
		
		mUiHandler.postDelayed(mUpdateVolumeRunnable, 50);
	}
	
	
	public void updateVoiceMeter(final float rmsdB) {
        mVolume = rmsdB;
    }
	
	@Override
	public void dismiss() {
		
		super.dismiss();
		 Log.d(TAG3,"SpeakNow dismiss");
		 mState = State.STOPPING;
	};
	
	@Override
	protected void onStop() {
		
		super.onStop();
		  Log.d(TAG3,"SpeakNow onStop");
		mState = State.STOPPING;
	}
}
