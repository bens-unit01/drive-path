package com.megamip.util;

import android.content.Context;
import android.util.Log;

import com.belkin.wemo.localsdk.WeMoDevice;
import com.belkin.wemo.localsdk.WeMoSDKContext;
import com.belkin.wemo.localsdk.WeMoSDKContext.NotificationListener;

public class MipWemoDevice implements NotificationListener {

	private WeMoSDKContext mWeMoSDKContext = null;
	private WeMoDevice mWemoDevice = null;

	private MipWemoDevice() {
		super();

	}

	public MipWemoDevice(Context context) {

		mWeMoSDKContext = new WeMoSDKContext(context);
		mWeMoSDKContext.addNotificationListener(this);
		mWeMoSDKContext.refreshListOfWeMoDevicesOnLAN();
	}

	@Override
	public void onNotify(String event, String udn) {

		log("onNotify ..." + event + " " + udn);

		if (event.equals(WeMoSDKContext.ADD_DEVICE)) {
			mWemoDevice = mWeMoSDKContext.getWeMoDeviceByUDN(udn);
			log("type: " + mWemoDevice.getType());

		}

	}

	public void turnOn() {
		if (mWemoDevice != null) {
			log("turning wemo device on");
			mWeMoSDKContext.setDeviceState(WeMoDevice.WEMO_DEVICE_ON,
					mWemoDevice.getUDN());
		} else {
			log("error, device not found");
		}

	}

	public void turnOff() {

		if (mWemoDevice != null) {
			log("turning wemo device off");
			mWeMoSDKContext.setDeviceState(WeMoDevice.WEMO_DEVICE_OFF,
					mWemoDevice.getUDN());
		} else {
			log("error, device not found");
		}

	}

	// this methods releases the wemoSDKContext
	public void stop() {

		mWeMoSDKContext.stop();

	}

	private void log(String str) {
		Log.d("A3", str);
	}
}
