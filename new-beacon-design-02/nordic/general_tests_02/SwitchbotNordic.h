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

//#define DEBUG_MODE
#ifdef DEBUG_MODE
#define LOG  printf
#define PGM_LOG printf
#define RX_PIN_NUMBER   2 //4 // UART RX pin number.
#define TX_PIN_NUMBER   0 //5// UART TX pin number.
#else
#define LOG(...)
#define PGM_LOG(...)
#define RX_PIN_NUMBER   2 // UART RX pin number.
#define TX_PIN_NUMBER   1 // UART TX pin number.
#endif

#ifdef DEBUG_MODE

#else
#endif



#define LED_left          20
#define LED_head	  12
#define LED_right   	  12//22
#define LED_tail          17 //23

#define	RGB_RED 	    7
#define RGB_GREEN 	    19
#define RGB_BLUE 		6
#define Switch 			5

 
//Andrew K. motors-board v1.0
 #define	IRM_right		21	//IRM receive right  R6
 #define	IRM_tail		24	//IRM receive tail   R3
 #define	IRM_left		25	//IRM receive left   R2
 #define	IRM_head		26	//IRM receive head   R4

#define CTS_PIN_NUMBER 10    // UART Clear To Send pin number. Not used if HWFC is set to false
#define RTS_PIN_NUMBER 8    // Not used if HWFC is set to false
#define HWFC           false//false // UART hardware flow control


#define TX_MEDIA_BOX    3 // UART TX pin number

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
int  calc_coord(uint8_t IR[], int *Head, int *Range );
void filter_data(uint32_t IR[], uint8_t index);
void filter_data_2d(uint16_t IR[9][4]);

#endif  // REV
