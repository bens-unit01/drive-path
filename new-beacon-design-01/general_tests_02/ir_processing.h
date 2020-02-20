/*
 * ir_processing.h
 *
 *  Created on: Dec 28, 2015
 *      Author: user
 */

#ifndef IR_PROCESSING_H_
#define IR_PROCESSING_H_

#include <stdint.h>
#include <stdbool.h>


#define RAWBUF  101  // Maximum length of raw duration buffer

typedef struct
{
	// The fields are ordered to reduce memory over caused by struct-padding
	uint8_t rcvstate;        // State Machine state
	uint8_t recvpin;         // Pin connected to IR data from detector
	uint8_t blinkpin;
	uint8_t blinkflag;       // true -> enable blinking of pin on IR processing
	uint8_t rawlen;          // counter of entries in rawbuf
	unsigned int timer;           // State timer, counts 50uS ticks.
	unsigned int rawbuf[RAWBUF];  // raw data
	uint8_t overflow;        // Raw buffer overflow occurred
} irparams_t;

// ISR State-Machine : Receiver States
#define STATE_IDLE      2
#define STATE_MARK      3
#define STATE_SPACE     4
#define STATE_STOP      5
#define STATE_OVERFLOW  6

extern volatile irparams_t irparams;

#define MARK   0
#define SPACE  1

#define USECPERTICK    50
#define _GAP            5000
#define GAP_TICKS       (_GAP/USECPERTICK)

#endif /* IR_PROCESSING_H_ */
