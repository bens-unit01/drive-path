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

#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wpointer-sign"

#define DEBUG_MODE
#ifdef DEBUG_MODE
#define LOG  printf
#define PGM_LOG printf
#else
#define LOG(...)
#define PGM_LOG(...)
#endif


#define LED_left          20
#define LED_head	  12
#define LED_right   	  12//22
#define LED_tail          17 //23

#define	RGB_RED 	    7
#define RGB_GREEN 	    19
#define RGB_BLUE 		6
#define Switch 			5

//jason board
// #define	IRM_head						1					//IRM receive head
// #define	IRM_tail						19					//IRM receive tail 
// #define	IRM_left						21					//IRM receive left
// #define	IRM_right						16					//IRM receive right


 #define	IRM_head		16	//IRM receive head   R4
 #define	IRM_tail		27	//IRM receive tail   R3
 #define	IRM_left		26	//IRM receive left   R2
 #define	IRM_right		18	//IRM receive right  R6



 #define	IRM_head_far		14 //IRM receive head   R4
 #define	IRM_tail_far		13	//IRM receive tail   R3
 #define	IRM_left_far		10	//IRM receive left   R2
 #define	IRM_right_far		11	//IRM receive right  R6


//#define	IRM_down						3					//IRM receive pointed down
//#define	IRM_forward						1					//IRM receive forward

//  --- rx-tx for dev board
//#define RX_PIN_NUMBER  16//16    // UART RX pin number.
//#define TX_PIN_NUMBER  17//8//6//17    // UART TX pin number.
//------------------------------------

// --- rx-tx for SwitchBot ----

//#define DEV_BOARD
#ifdef DEV_BOARD
//.........................................
#define RX_PIN_NUMBER  11 //22 // 16 //16    // UART RX pin number.
#define TX_PIN_NUMBER  9 // 23 //17 //8//6//17    // UART TX pin number.
#define CTS_PIN_NUMBER 10    // UART Clear To Send pin number. Not used if HWFC is set to false
#define RTS_PIN_NUMBER 8    // Not used if HWFC is set to false
#define HWFC           true //false // UART hardware flow control
//...........................................
#else
//............................................
#define RX_PIN_NUMBER   22 // 16 //16    // UART RX pin number.
#define TX_PIN_NUMBER   23 //17 //8//6//17    // UART TX pin number.
#define CTS_PIN_NUMBER  20    // UART Clear To Send pin number. Not used if HWFC is set to false
#define RTS_PIN_NUMBER  20   // Not used if HWFC is set to false
#define HWFC           false //false // UART hardware flow control
//..............................................
#endif

#define TX_MEDIA_BOX    3 // UART TX pin number


//UART_PSELRTS = 8;   	// P0.08  RTS
//UART_PSELTXD = 9;   	// P0.09  TXD
//UART_PSELCTS = 10;  	// P0.10  CTS
//UART_PSELRXD = 11;  	// P0.11  RXD

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


#define NOTF_GET_STATUS 0x51
#define NOTF_SET_STATUS 0x52
#define NOTF_ACTIVATE_ADB 0x53
#define STAND_UP        0x61
#define KNEEL               0x62
#define LEAN            0x63
#define ESTOP           0x65
#define CLEAR_ESTOP     0x66
#define DRIVE               0x81

bool calculate_speed(const uint8_t IR[],const int MaxRange,  int16_t * fwd_bwd,  int16_t * lft_rgt, uint8_t debug_table[] );
int calc_coord(uint8_t IR[], int *Head, int *Range );


#endif  // REV
