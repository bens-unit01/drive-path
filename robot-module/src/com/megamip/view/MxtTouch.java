package com.megamip.view;

import android.graphics.Point;



public class MxtTouch {
	
	
	
	
	
	
	public MxtTouch() {
		super();
		
	}
	
	
	

	public interface TouchDownListener {

		public void onNotify(TouchEvent e);

	}

	public interface TouchUpListener {

		public void onNotify(TouchEvent e);

	}

	public interface TouchSwipeListener {

		public void onNotify(TouchEvent e);

	}

	public interface TouchSwipeUpListener {

		public void onNotify();

	}

	public interface TouchSwipeDownListener {

		public void onNotify();

	}

	public class TouchEvent {
		private Object sender;
		private Point position;

		public Object getSender() {
			return sender;
		}

		public Point getPosition() {
			return position;
		}

		public TouchEvent() {
			super();
		}

		public TouchEvent(Object sender, Point position) {
			super();

			this.position = new Point(position.x, position.y);
			this.sender = sender;

		}

	}

}
