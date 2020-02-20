package com.megamip.voice;

import android.content.Context;

public interface SBUsbDevice {

	public void addUsbListener(UsbListener usbListener);

	public void write(byte[] data);

	public void writeRaw(byte[] data);
	
	public void disconnect(DeviceType deviceType);

	public boolean isDeviceConnected(Context context, DeviceType deviceType);

	public boolean isAllUsbConnected(Context context);


	public enum DeviceType {
		NUTINY, XYZ_SENSOR
	}
	public interface UsbListener {

		public void onNotify(UsbEvent e);

	}

	public class UsbEvent {
		private Object sender;
		private byte[] data;

		public Object getSender() {
			return sender;
		}

		public byte[] getData() {
			return data;
		}

		public UsbEvent() {
			super();
		}

		public UsbEvent(Object sender, byte[] data) {
			super();

			this.sender = sender;
			this.data = data;
		}

	}

}
