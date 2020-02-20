package com.wowwee.gui_dpmatrix01;

import com.example.gui_dpmatrix01.R;
import com.example.gui_dpmatrix01.R.layout;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;

public class LabelInputDialog extends Activity {

	public final static String NEW_LABEL = "new_label";
	private Button btnSave, btnCancel;
	private EditText txtLabel;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_label_input_dialog);
		
	  btnCancel = (Button)findViewById(R.id.btnCancelLabel);	
	  btnSave = (Button)findViewById(R.id.btnSaveLabel);
	  txtLabel = (EditText)findViewById(R.id.txtLabel);  
	  btnCancel.setOnClickListener(new OnClickListener() {
		
		@Override
		public void onClick(View v) {
		    finish();	
		}
	});
	  
	btnSave.setOnClickListener(new OnClickListener() {
		
		@Override
		public void onClick(View v) {
		Bundle b = new Bundle();
		String newLabel = txtLabel.getText().toString(); 
		b.putString(NEW_LABEL, newLabel);
		Intent result = new Intent();
	    result.putExtras(b);
	    
	    setResult(Activity.RESULT_OK, result);
	    finish();
		
		}
	});
	}
}
