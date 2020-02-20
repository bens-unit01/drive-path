/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */
#ifndef REV
#define REV

#include "nrf_gpio.h"

#define LED_left          20
#define LED_head	      12
#define LED_right   	  22
#define LED_tail          23

#define	RGB_RED 		8
#define RGB_GREEN 		7
#define RGB_BLUE 		6
#define Switch 			5

//jason board
// #define	IRM_head						1					//IRM receive head
// #define	IRM_tail						19					//IRM receive tail 
// #define	IRM_left						21					//IRM receive left
// #define	IRM_right						16					//IRM receive right


//new board
#define	IRM_right						21					//IRM receive right
#define	IRM_tail						19					//IRM receive tail 
#define	IRM_left						16					//IRM receive left
#define	IRM_head						2					//IRM receive head

//#define	IRM_down						3					//IRM receive pointed down
//#define	IRM_forward						1					//IRM receive forward

#define RX_PIN_NUMBER  9//16    // UART RX pin number.
#define TX_PIN_NUMBER  10//8//6//17    // UART TX pin number.
#define CTS_PIN_NUMBER 18    // UART Clear To Send pin number. Not used if HWFC is set to false
#define RTS_PIN_NUMBER 19    // Not used if HWFC is set to false
#define HWFC           false // UART hardware flow control


/* yes, thrust is inverted */
#define THRUST_MIN	0xe1
#define THRUST_MAX	0x00

#define YAW_MIN		0x00
#define YAW_MAX		0xe1
#define PITCH_MIN	0x00
#define PITCH_MAX	0xe1
#define ROLL_MIN	0x00
#define ROLL_MAX	0xe1

struct qr_cmd {
	uint16_t thrust;					/* left joystick, up/down */
	uint16_t yaw;					/* left joystick, left/right */
	uint16_t pitch;					/* right joystick, up/down */
	uint16_t roll;					/* right joystick, left/right */
	uint16_t aux1;
	uint16_t aux2;
	uint16_t aux3;
	uint16_t aux4;

};



#endif  // REV
