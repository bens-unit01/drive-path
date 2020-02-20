package com.wowwee.gui_dpmatrix01;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedList;

import com.att.widgets.lib.button.SegmentedTextToggleButton;
import com.example.gui_dpmatrix01.R;
import com.google.gson.Gson;
import com.wowwee.drivepath.DPMap;
import com.wowwee.drivepath.Node;
import com.wowwee.gui_dpmatrix01.MainActivity.MapView.Line;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.LinearLayout;

public class MainActivity extends Activity {

	final String TAG = getClass().getSimpleName();
	private ArrayList<Line> map;
	private Line currentLine;
	Button btnInsertDelete, btnSetLabel, btnSwap, btnSave;
	private DPMap mDPMap;
	private int mLastPosition;
    public final static int REQUEST_CODE = 1;
	enum Mode {INSERT, DELETE, SWAP, SET_LABEL}
	private SegmentedTextToggleButton toggleButtons;

    private Mode mode = Mode.INSERT;	
	public static int getX(View view) {
		return view.getLeft() + 62 + 240;
	}
	
	public static int getY(View view) {
		return view.getBottom() + 343;
//		return view.getBottom();
	}
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(new MapView(this));
	    testJson();
	    testPersistance();
		map = new ArrayList<Line>();
	    mDPMap = new DPMap();
	    toggleButtons = (SegmentedTextToggleButton)findViewById(R.id.segmented_enabled);
        toggleButtons.getChildAt(0).setActivated(true);
		toggleButtons.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
			
				switch (toggleButtons.getSelectedIndex()) {
				case 0: mode = Mode.INSERT;
					break;
				case 1: mode = Mode.DELETE; 
				default:
					break;
				}
			}
		});
/*	
		btnSetLabel.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
			  startActivityForResult(new Intent(MainActivity.this, LabelInputDialog.class ), REQUEST_CODE);	
			}
		});
*/

	}
	
  private void testPersistance() {
	

	  
	}

private void testJson() {
		LinkedList path = new LinkedList();
		Node node01 = new Node(1, 0);
		Node node02 = new Node(2, 1);
		Node node03 = new Node(3, 2);
	    path.add(node01);
	    path.add(node02);
	    path.add(node03);
	    
		//ArrayList<Integer> node01Neighbors = new ArrayList<Integer>();
		node01.getNeighbors().add(node02.getId());
		//node01.setNeighbors(node01Neighbors);
		
		//ArrayList<Integer> node02Neighbors = new ArrayList<Integer>();
		node02.getNeighbors().add(node01.getId());
		node02.getNeighbors().add(node03.getId());
//		node02.setNeighbors(node02Neighbors);
		
//		ArrayList<Integer> node03Neighbors = new ArrayList<Integer>();
		node03.getNeighbors().add(node02.getId());
//		node03.setNeighbors(node03Neighbors);
		
	    //DPMap map = new DPMap(path);
	    DPMap map = new DPMap();
	    map.add(node01);
	    map.add(node02);
	    map.add(node03);
	    map.add(node03);
		Log.d(TAG, "map " + map);
		// parcours du path 
		
	    Gson gson = new Gson();
	    String jsonString = gson.toJson(map);
		//log("encoding: \n" + jsonString);
	    Log.d(TAG, "json " + jsonString);  
	    
	    DPMap des = gson.fromJson(jsonString, DPMap.class);
	    Log.d(TAG, "des " + des);

	  String FILENAME = "hello_file";

   // mCreateAndSaveFile(FILENAME, "hello");
   // mReadJsonData(FILENAME);
   // mCreateAndSaveFile(FILENAME, jsonString);
    mReadJsonData(FILENAME);
	}

public void mCreateAndSaveFile(String params, String mJsonResponse) {
    try {
        FileWriter file = new FileWriter("/data/data/" + getApplicationContext().getPackageName() + "/" + params);
        file.write(mJsonResponse);
        file.flush();
        file.close();
    } catch (IOException e) {
        e.printStackTrace();
    }
}

public void mReadJsonData(String params) {
    try {
        File f = new File("/data/data/" + getPackageName() + "/" + params);
        FileInputStream is = new FileInputStream(f);
        int size = is.available();
        byte[] buffer = new byte[size];
        is.read(buffer);
        is.close();
        String mResponse = new String(buffer);
        
        Log.d(TAG, " file reading result : " + mResponse);
    } catch (IOException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
    }
}

public static	 void LOG(String string){
		Log.d("TAG1", string);
	}

	public class MapView extends FrameLayout {

		private FrameLayout mGui;
		private Context mContext;
		private Paint mPaint;
		private Path mPath;
		private boolean isSelectMode = true;
        private Pt startPt, endPt;
        private GridView gridView;
        private View lastView;
		class Pt {

		   int	 x, y ;
		   long id;
			Pt(int _x, int _y) {
				x = _x;
				y = _y;

			}
			
			Pt(int _x, int _y, long _id) {
				x = _x;
				y = _y;
				id = _id;

			}
			
			@Override
				public boolean equals(Object o) {
					return this.id == ((Pt)o).id ;
				}
			
		}
		
		class Line {
			Pt startPoint;
			Pt endPoint;

			@Override
			public boolean equals(Object o) {
				boolean returnValue = false;
				Line firstLine = (Line) this;
				Line secondLine = (Line) o;
				returnValue = firstLine.startPoint.id == secondLine.startPoint.id
						&& firstLine.endPoint.id == secondLine.endPoint.id;
			    if(!returnValue){
			    	returnValue = firstLine.startPoint.id == secondLine.endPoint.id
							&& firstLine.endPoint.id == secondLine.startPoint.id;
			    }	
				return returnValue;
			}
		}
      

		public MapView(Context context) {

			super(context);
			mContext = context;
			setWillNotDraw(false);
			init();

		}

		private void init() {
			
			// pen init
			mPaint = new Paint();
			mPaint.setColor(Color.BLACK);
			mPaint.setStrokeWidth(13);
			mPaint.setStyle(Paint.Style.STROKE);
			mPath = new Path();
			
			
			// gui init 
			LayoutInflater inflator = (LayoutInflater) getContext()
					.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
			mGui = (FrameLayout) inflator.inflate(R.layout.activity_drive_path,
					this, false);
			// setWillNotDraw(false);
			gridView = (GridView) mGui.findViewById(R.id.grid_view);
			gridView.setAdapter(new GridAdapter(mContext));
			gridView.setOnItemClickListener(new ClickHandler());
			addView(mGui);

		}
		

		@Override
		protected void onDraw(Canvas canvas) {

			super.onDraw(canvas);
			LOG("onDraw ...");
            canvas.drawPath(mPath, mPaint);
		

		}
		

		
		
	  
	  
		public class ClickHandler implements OnItemClickListener {

			@Override
			public void onItemClick(AdapterView<?> parent, View view,
					int position, long id) {
				// view.setPressed(isSelectMode);
				if (isSelectMode) {
					startPt = new Pt(MainActivity.getX(view),
							MainActivity.getY(view), id);
					isSelectMode = false;
					// ImageView iv = (ImageView)gridView.getI;
					// iv.setImageResource(R.drawable.pos01);
					// ImageView iv =
					// (ImageView)gridView.getAdapter().getView(position,
					// view, gridView);
					LinearLayout ll = (LinearLayout)view;
					ImageView iv = (ImageView)ll.getChildAt(0);
					iv.setImageResource(R.drawable.dp_node_red);
					lastView = view;
					currentLine = new Line();
					currentLine.startPoint = startPt;
					mLastPosition = position;
					LOG("if ... x: " + startPt.x + " y: " + startPt.y);
				} else {
	                    endPt = new Pt(MainActivity.getX(view),
								MainActivity.getY(view), id);
						// mPath = new Path();
					
						isSelectMode = true;
						LOG("else ...start-> x: " + startPt.x + " y: "
								+ startPt.y + " end x" + endPt.x + " y:"
								+ endPt.y);
						LinearLayout ll = (LinearLayout)lastView;
						ImageView iv = (ImageView) ll.getChildAt(0);
						iv.setImageResource(R.drawable.dp_node);
						currentLine.endPoint = endPt;
						if(currentLine.endPoint.equals(currentLine.startPoint)) return; // line not accepted
			
				switch (mode) {
				case INSERT:
				
						LOG("i contains: " + map.contains(currentLine));
						if(!map.contains(currentLine)) {
							map.add(currentLine);
						//drawing the new line 	
							mPath.moveTo(startPt.x, startPt.y);
							mPath.lineTo(endPt.x, endPt.y);
						// updating mDPMap with the new nodes 	
						Node node1 = new Node(mLastPosition + 1, 0);
						Node node2 = new Node(position +1, 0);
						if(mDPMap.getNodes().contains(node1)){
							node1 = mDPMap.getNode(node1.getId());
						}
						
						if(mDPMap.getNodes().contains(node2)){
							node2 = mDPMap.getNode(node2.getId());
						}
						node1.getNeighbors().add(node2.getId());
						node2.getNeighbors().add(node1.getId());
						mDPMap.add(node1);
						mDPMap.add(node2);
						//node1;
						}

					
					break;
				case DELETE:
			
						LOG("d contains: " + map.contains(currentLine));
						
						if(map.contains(currentLine)){
							map.remove(currentLine);
							// removing 2 nodes from neighbors 
							Node node1 = new Node(mLastPosition + 1, 0);
							Node node2 = new Node(position +1, 0);
						    node1 = mDPMap.getNode(node1.getId());
						    node2 = mDPMap.getNode(node2.getId());
						    int index = node1.getNeighbors().indexOf(node2.getId());
						    node1.getNeighbors().remove(index);
						    index = node2.getNeighbors().indexOf(node1.getId());
						    node2.getNeighbors().remove(index);
						    if(0 == node1.getNeighbors().size()){
						    	mDPMap.getNodes().remove(node1);
						    }
						    
						    if(0 == node2.getNeighbors().size()){
						    	mDPMap.getNodes().remove(node2);
						    }
						    // redrawing the map
							mPath.reset();	
							for (Line l:map){
								mPath.moveTo(l.startPoint.x, l.startPoint.y);
								mPath.lineTo(l.endPoint.x, l.endPoint.y);
							}
							
						}

					break;
				}
				
				
			LOG("ID: " + id)	;
				Log.d(TAG, "mDPMap: " + mDPMap + " mode: " + mode);
			 MapView.this.invalidate();


			}


		}

	}
	
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        
         if(resultCode == Activity.RESULT_OK && requestCode == REQUEST_CODE && data != null ){
           String newLabel = data.getStringExtra(LabelInputDialog.NEW_LABEL); 
           Log.d(TAG, " onActivityResult label: " + newLabel );	
         }
	
	}

}
