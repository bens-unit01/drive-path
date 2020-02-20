package com.att.widgets.lib.dropdown;

import java.util.Vector;

import android.content.Context;
import android.graphics.Color;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.PopupWindow;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.AdapterView.OnItemClickListener;

import com.att.widgets.lib.R;

public class DropDown extends RelativeLayout{

	private static final int DEFAULT_FIELDS = 4;
	private static final int BORDER_SIZE = 4;
	
	private TextView mTextView;
	private ImageView mArrowImage;
	private PopupWindow mPopup;
	private ListView mDropDownView;
	
	//adapter
	private DropDownAdapter mDropDownAdapter;
	
	//listeners
	private PopupListener mPopupListener;
	private OnClickListener mOnClickListener;
	
	/**
	 * Creates a DropDown Widget.
	 * @param context
	 */
	public DropDown(Context context) {
		super(context);
		init(context, null);
	}
	
	/**
	 * Creates a DropDown Widget with a set of attributes.
	 * @param context
	 * @param attrs
	 */
	public DropDown(Context context, AttributeSet attrs) {
		super(context, attrs);
		init(context, attrs);
	}
	
	/**
	 * Creates a DropDown Widget with the defined style given by the user.
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public DropDown(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init(context, null);
	}
	
	/**
	 * Initialize the class with a set of attributes defined in the XML Layout.
	 * @param context
	 * @param attrs
	 */
	private void init(Context context, AttributeSet attrs){
		LayoutInflater inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		ViewGroup inflated = (ViewGroup) inflater.inflate(R.drawable.drop_down, null);
		this.addView(inflated);
		
		mTextView = (TextView)findViewById(R.id.drop_down_text);
		mArrowImage = (ImageView)findViewById(R.id.drop_down_arrow);
		
		mTextView.setFocusable(true);
		

		mPopup = new PopupWindow(context, attrs);
		mPopup.setBackgroundDrawable(getResources().getDrawable(R.drawable.dropdown_background));
		
		mDropDownView = new ListView(context, attrs);
		mDropDownView.setCacheColorHint(0x00000000); //Remove the android background optimization cache		
		
		mDropDownAdapter = new DropDownAdapter(context);
		mDropDownView.setAdapter(mDropDownAdapter);
		mDropDownView.setOnItemClickListener(itemClickListener);
		mDropDownView.setDivider(getResources().getDrawable(R.drawable.dropdown_line));
		mDropDownView.setSelector(R.drawable.drop_down_focus);
		
		mPopup.setFocusable(true);
		
		//Dismiss the pop up when is close
		mPopup.setOnDismissListener(new PopupWindow.OnDismissListener() {
			public void onDismiss() {dismissDropDown();}});
		
		mPopup.setContentView(mDropDownView);
		
		mPopupListener = new PopupListener();
		mTextView.setOnClickListener(mPopupListener);
		
		//Set the default numbers of fields
		setFieldsNumber(DEFAULT_FIELDS);
	}

	/**
     * Pop up click listener
     * their own click listener.
     */
    private class PopupListener implements OnClickListener {
        /** {@inheritDoc} */
        public void onClick(View v) {
            if(isPopupShowing()){
            	dismissDropDown();
            }else{    		
            	showDropDown();
            }
        }
    }
    
    /**
     * <p>Displays the drop down on screen.</p>
     */
    public void showDropDown() {
    	mArrowImage.setBackgroundResource(R.drawable.dropdown_close);
    	mPopup.setWidth(getWidth());
    	mPopup.setContentView(mDropDownView);
    	mPopup.showAsDropDown(this);
    }
    
    @Override
    public void setOnClickListener(OnClickListener l) {
    	this.mOnClickListener = l;
    }

    @Override
    protected void onDetachedFromWindow() {
    	dismissDropDown();
    	super.onDetachedFromWindow();
    }
    
    /**
     * <p>Closes the drop down if present on screen.</p>
     */
    public void dismissDropDown() {
    	mArrowImage.setBackgroundResource(R.drawable.dropdown_open);
        mPopup.dismiss();
        mPopup.setContentView(null);
    }
    
    /**
     * <p>Indicates whether the popup menu is showing.</p>
     *
     * @return true if the popup menu is showing, false otherwise
     */
    public boolean isPopupShowing() {
        return mPopup.isShowing();
    }

    private OnItemClickListener itemClickListener = new OnItemClickListener() {
		public void onItemClick(AdapterView<?> arg0, View v, int i, long arg3) {
			mDropDownAdapter.setSelectedPosition(i);
			mDropDownAdapter.notifyDataSetChanged();
			DropDownData d = (DropDownData)mDropDownAdapter.getItem(i);
			mTextView.setText(d.getText());
			dismissDropDown();
			
			if(mOnClickListener != null){mOnClickListener.onClick(DropDown.this);}
		}
	};
	
    private class DropDownAdapter extends BaseAdapter{
    	
    	private Vector<DropDownData> data;
    	private Context context;
    	private int selectedPosition = -1;
    	
    	public DropDownAdapter(Context context){
    		this.context = context;
    	}
    	
    	public void setData(Vector<DropDownData> data){
    		selectedPosition = -1;
    		this.data = data;
    	}
    	
    	public void setData(String[] array){
    		selectedPosition = -1;
    		if(array != null){
    			Vector<DropDownData> d = new Vector<DropDownData>();
    			for(int i=0; i<array.length; i++){
    				d.add(new DropDownData(array[i], i));
    			}
    			data = d;
    		}
    	}
    	
    	public int getselectedPosition(){
    		return selectedPosition;
    	}
    	
    	public void setSelectedPosition(int position){
    		if(position >= 0 && position < data.size()){    			
    			this.selectedPosition = position;
    		}
    	}

		public int getCount() {
			int count = 0;
			if(data != null){
				count = data.size();
			}
			return count;
		}

		public Object getItem(int i) {
			if(data != null && 
				i >= 0 && i < data.size()){
				return data.get(i);
			}
			return null;
		}

		public long getItemId(int position) {
			return position;
		}

		public View getView(int position, View convertView, ViewGroup parent) {
			View view = null;
			Object o = getItem(position);
			if(o != null && o instanceof DropDownData){				
				if(convertView != null && convertView instanceof DropDownItemView){
					((DropDownItemView)convertView).updateItem(((DropDownData)o).getText(), position==selectedPosition);
					view = convertView;
				}else{
					view = new DropDownItemView(context);
					((DropDownItemView)view).updateItem(((DropDownData)o).getText(), position==selectedPosition);
				}
			}
			return view;
		}

		public DropDownData getSelectedItem() {
			Object o = getItem(selectedPosition);
			if(o != null && o instanceof DropDownData){
				return (DropDownData)o;
			}
			return null;
		}
    	
    }
    
    /**
     * set the display data
     * @param data
     */
    public void setData(String[] data){
    	mDropDownAdapter.setData(data);
    }
    
    /**
     * Set the display data
     * @param data
     */
    public void setData(Vector<DropDownData> data){
    	mDropDownAdapter.setData(data);
    }
    
    /**
     * Set the selected position
     * @param item
     */
    public void setSelectedItemIndex(int item){
    	mDropDownAdapter.setSelectedPosition(item);
    }
    
    /**
     * Set the number of fields visible on the
     * drop down list.
     * @param fields
     */
    public void setFieldsNumber(int fields){
    	if(fields > 0){    		
    		mPopup.setHeight((int)(DropDownItemView.HEIGHT * fields + BORDER_SIZE));
    	}
    }
    
    /**
     * Set the display text
     * @param text
     */
    public void setDisplayText(String text){
    	mTextView.setText(text);
    }
    
    /**
     * Return the index of the selected position or
     * -1 if none selected.
     * @return
     */
    public int getSelectedIndex(){
    	return mDropDownAdapter.getselectedPosition();
    }
    
    /**
     * Return the selected Data
     * @return
     */
    public DropDownData getSelectedData(){
    	DropDownData d = mDropDownAdapter.getSelectedItem();
    	if(d != null){
    		return d;
    	}
    	return null;
    }
    
    /**
     * Return the Text of the selected data
     * @return
     */
    public String getSelectedText(){
    	DropDownData d = getSelectedData();
    	if(d != null){
    		return d.getText();
    	}
    	return null;
    }
    
    @Override
    public void setEnabled(boolean enabled) {
    	mTextView.setEnabled(enabled);
    	mTextView.setTextColor(enabled?Color.BLACK:Color.GRAY);
    	super.setEnabled(enabled);
    }
    
}
