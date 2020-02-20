

/*---------------------------------------------------------------------------------------------------------*/
/*
Author : Raouf 
 */

#ifndef __Encoders_H__
#define __Encoders_H__
#include "stm32f4xx_hal.h"

float get_pulse(uint8_t pulse_id);
float get_speed(uint8_t pulse_id); 
void reset_encoders(void); 

#endif  
