package com.megamip.voice;

import java.math.BigInteger;
import java.util.Arrays;

import android.util.Log;

public class MovementInput extends MipInput {

	private static final String TAG = "A3";

	public static final String BACK_GESTURE = "82";
	public static final String HOLD_GESTURE = "72";
	public static final String NEXT_GESTURE = "69";
	public static final String LEFT_SWIPE = "108";
	public static final String RIGHT_SWIPE = "114";
	public static final String UP_SWIPE = "117";
	public static final String DOWN_SWIPE = "100";
	public static final String SIMPLE_TOUCH = "115";

	// constructors

	public MovementInput() {
		super();

	}

	public MovementInput(byte[] input) {

		try {
			action = new Byte(input[2]).toString();
			
			if (action.equals(SIMPLE_TOUCH)) {
				args = new String[3];
				args[0] = String.format("%d", (int) input[3] & 0xFF); // we take the unsigned value 
				args[1] = String.format("%d", (int) input[4] & 0xFF);
				args[2] = String.format("%d", (int) input[5] & 0xFF);
				// Log.d(TAG,
				// "arg1: "+args[0]+" arg2: "+args[1]+" arg3: "+args[2]);
			}

			// Log.i(TAG,
			// "MovementInput constructor input : "
			// + String.format("%040x", new BigInteger(1, input)));
		} catch (ArrayIndexOutOfBoundsException ex) {

			action = "82";
			Log.e(TAG, "MovementInput constructor - error : " + ex.getMessage());
		}

		// String[] x = {""};
		// args = x;
	}

	// toString()

	@Override
	public String toString() {
		return "MovementInput [action=" + action + ", args="
				+ Arrays.toString(args) + "]";
	}

}
