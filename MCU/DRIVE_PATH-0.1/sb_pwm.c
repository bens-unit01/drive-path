/*
 * pwm.c
 *
 *  Created on: Mar 10, 2015
 *      Author: user
 */
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "app_timer.h"
#include "nrf_pwm.h"
#include "nrf_gpio.h"
#include "nordic_common.h"
#include "SwitchbotNordic.h"


static bool RightMotForward;
void
pwm_init (void)
{
  nrf_pwm_config_t pwm_config = PWM_DEFAULT_CONFIG
  ;

  nrf_gpio_cfg_output (AIN1);
  nrf_gpio_cfg_output (AIN2);
  nrf_gpio_cfg_output (BIN1);
  nrf_gpio_cfg_output (BIN2);
  nrf_gpio_cfg_output (STDBY);
  pwm_config.mode = PWM_MODE_LED_255;
  pwm_config.num_channels = 2;
  pwm_config.gpio_num[0] = PWMA;
  pwm_config.gpio_num[1] = PWMB;

  // Initialize the PWM library
  nrf_pwm_init (&pwm_config);

}

void
CalcDriveValues (int16_t fwd_bwd, int16_t lft_rgt)
{
  int16_t rightmotor_speed;
  int16_t leftmotor_speed;

  leftmotor_speed = 800;
  rightmotor_speed = 800;

  //Scale Drive values fwd_bwd
  //0x01 (forward slowest) - 0x20 (forward fastest) OR
  //0x21 (backward slowest) - 0x40 (backward fastest)

  //simple_uart_put(lft_rgt);

  if (fwd_bwd >= 0x00 && fwd_bwd <= 0x20)
    {
      leftmotor_speed += (fwd_bwd * 7);
      rightmotor_speed += (fwd_bwd * 7);

    }
  else
    { //must be backwards
      leftmotor_speed -= ((fwd_bwd - 0x20) * 7);
      rightmotor_speed -= ((fwd_bwd - 0x20) * 7);
    }

  //Scale Drive values lft_rgt

  if (lft_rgt != 0)
    {
      //0x41 (right spin slowest) - 0x60 (right spin fastest) OR
      //0x61 (left spin slowest) - 0x80 (left spin fastest)
      if (lft_rgt >= 0x40 && lft_rgt <= 0x60)  //turn to right
	{
	  leftmotor_speed = (leftmotor_speed + ((lft_rgt - 0x40) * 8));
	  rightmotor_speed = (rightmotor_speed - ((lft_rgt - 0x40) * 8));

	}
      else
	{ //must be left turn
	  leftmotor_speed = (leftmotor_speed - ((lft_rgt - 0x60) * 8));
	  rightmotor_speed = (rightmotor_speed + ((lft_rgt - 0x60) * 8));
	}
    }

  if (rightmotor_speed >= 800) //Right Motor Forward
    {

      nrf_gpio_pin_set (AIN1);
      nrf_gpio_pin_clear (AIN2);
      nrf_pwm_set_value (0, (rightmotor_speed - 800));
    }
  else
    { //reverse

      nrf_gpio_pin_set (AIN2);
      nrf_gpio_pin_clear (AIN1);
      nrf_pwm_set_value (0, (800 - rightmotor_speed));
    }

  if (leftmotor_speed >= 800) //Left Motor Forward
    {
      nrf_gpio_pin_set (BIN1);
      nrf_gpio_pin_clear (BIN2);
      nrf_pwm_set_value (1, (leftmotor_speed - 800));

    }
  else
    { //reverse
      leftmotor_speed = (800 - leftmotor_speed) + 10;
      //leftmotor_speed = (leftmotor_speed >= 255)?
      nrf_gpio_pin_set (BIN2);
      nrf_gpio_pin_clear (BIN1);
      nrf_pwm_set_value (1, leftmotor_speed );

    }



  nrf_gpio_pin_set(STDBY);


}

