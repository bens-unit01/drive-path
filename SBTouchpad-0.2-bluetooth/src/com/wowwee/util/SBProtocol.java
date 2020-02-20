package com.wowwee.util;

public class SBProtocol {
	
	
	public static final byte START_BYTE         = (byte) 0xFE; 
	public static final byte END_BYTE           = (byte) 0xFF;  
	public static final byte NOTF_VOICE_RECORD  = 0x50;
	public static final byte NOTF_GET_STATUS    = 0x51;
	public static final byte NOTF_SET_STATUS    = 0x52;
	public static final byte NOTF_ACTIVATE_ADB  = 0x53;  // activate adb through wifi 
	public static final byte MOVE_FORWARD       = 0x10; 
	public static final byte MOVE_BACKWARD      = 0x11; 
	public static final byte TURN_LEFT          = 0x20; 
	public static final byte TURN_RIGHT         = 0x21;
	public static final byte LEAN_FORWARD       = 0x30;
	public static final byte LEAN_BACKWARD      = 0x31;
	
	// body cons codes
//	public static final byte BODY_CON           = 0x30;
	public static final byte DANCE              = 0x60;
	public static final byte STAND_UP           = 0x61;
//	public static final byte TRACK              = 0x62;
	public static final byte KNEEL              = 0x62;
	public static final byte LEAN               = 0x63;   
	public static final byte DRIVE              = 0x78;  
    public static final byte ESTOP              = 0x65;
    public static final byte CLEAR_ESTOP        = 0x66;
	
/*
   speed byte: 0x00 - 0x20 is forwards slow to full speed, 
   0x21 - 0x40 is reverse slow to full speed
   direction byte: 0x41 - 0x60 is right turn gentle arc to sharp turn, 
   0x61 - 0x80 is left turn gentle arc to sharp turn
 * */	
	// encoded commands 
    public static final byte[] ENCODED_DRIVE_FORWARD  = {DRIVE, 0x10, 0}; 
    public static final byte[] ENCODED_DRIVE_BACKWARD = {DRIVE, 0x30, 0}; 
    public static final byte[] ENCODED_TURN_LEFT      = {DRIVE, 0, 0x70}; 
    public static final byte[] ENCODED_TURN_RIGHT     = {DRIVE, 0, 0x50}; 
    public static final byte[] ENCODED_LEAN_FORWARD   = {LEAN, (byte) 0xB1 , 0}; 
    public static final byte[] ENCODED_LEAN_BACKWARD  = {LEAN, 0x4D, 0x50}; 
    public static final int    DRIVE_NB_STEPS         = 10; 

}
