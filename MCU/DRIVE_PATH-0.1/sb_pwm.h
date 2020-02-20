/*
 * sb_pwm.h
 *
 *  Created on: Mar 10, 2015
 *      Author: user
 */

#ifndef SB_PWM_H_
#define SB_PWM_H_

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "app_timer.h"
#include "nrf_pwm.h"
#include "nrf_gpio.h"

void pwm_init(void);
void CalcDriveValues(int16_t fwd_bwd, int16_t lft_rgt);

#endif /* SB_PWM_H_ */
