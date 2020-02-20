/*
 * ir_processing.c
 *
 *  Created on: Dec 28, 2015
 *      Author: Raouf
 *  Inspired by : https://github.com/z3t0/Arduino-IRremote/blob/master/IRremote.cpp
 *
 *
 */




#include "ir_processing.h"
void ir_read(){


		//uint8_t  irdata = (uint8_t)digitalRead(irparams.recvpin);
		uint8_t  irdata = 0;

		irparams.timer++;  // One more 50uS tick
		if (irparams.rawlen >= RAWBUF)  irparams.rcvstate = STATE_OVERFLOW ;  // Buffer overflow

		switch(irparams.rcvstate) {
			//......................................................................
			case STATE_IDLE: // In the middle of a gap
				if (irdata == MARK) {
					if (irparams.timer < GAP_TICKS)  {  // Not big enough to be a gap.
						irparams.timer = 0;

					} else {
						// Gap just ended; Record duration; Start recording transmission
						irparams.overflow                  = false;
						irparams.rawlen                    = 0;
						irparams.rawbuf[irparams.rawlen++] = irparams.timer;
						irparams.timer                     = 0;
						irparams.rcvstate                  = STATE_MARK;
					}
				}
				break;
			//......................................................................
			case STATE_MARK:  // Timing Mark
				if (irdata == SPACE) {   // Mark ended; Record time
					irparams.rawbuf[irparams.rawlen++] = irparams.timer;
					irparams.timer                     = 0;
					irparams.rcvstate                  = STATE_SPACE;
				}
				break;
			//......................................................................
			case STATE_SPACE:  // Timing Space
				if (irdata == MARK) {  // Space just ended; Record time
					irparams.rawbuf[irparams.rawlen++] = irparams.timer;
					irparams.timer                     = 0;
					irparams.rcvstate                  = STATE_MARK;

				} else if (irparams.timer > GAP_TICKS) {  // Space
						// A long Space, indicates gap between codes
						// Flag the current code as ready for processing
						// Switch to STOP
						// Don't reset timer; keep counting Space width
						irparams.rcvstate = STATE_STOP;
				}
				break;
			//......................................................................
			case STATE_STOP:  // Waiting; Measuring Gap
			 	if (irdata == MARK)  irparams.timer = 0 ;  // Reset gap timer
			 	break;
			//......................................................................
			case STATE_OVERFLOW:  // Flag up a read overflow; Stop the State Machine
				irparams.overflow = true;
				irparams.rcvstate = STATE_STOP;
			 	break;
		}


}



