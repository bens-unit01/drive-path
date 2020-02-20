

/**
 ******************************************************************************
 * File Name          : main.c
 * Description        : Main program body
 ******************************************************************************
 *
 * COPYRIGHT(c) 2016 STMicroelectronics
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <usbd_core.h>
#include <usbd_cdc.h>
#include <stdbool.h> 
#include "usbd_cdc_interface.h"
#include "usbd_desc.h"
#include "main.h"
#include "usart.h" 
#include "Switchbot.h" 
#include "motors.h" 
#include "encoders.h" 
#include "util.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2S_HandleTypeDef hi2s3;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim1;  // TIM1 --> used for the encoders 
TIM_HandleTypeDef htim2;  // TIM2 --> used for motors pwm (3 motors ) 
TIM_HandleTypeDef htim3;  // TIM3 --> controlers, pids ... 
TIM_HandleTypeDef htim4;  // TIM4 --> used for debug via USB 
TIM_HandleTypeDef htim5;  // TIM5 --> used for micros() function  

//UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;


USBD_HandleTypeDef USBD_Device;
extern USBD_DescriptorsTypeDef VCP_Desc;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define SET_KP 0x70
#define SET_KI 0x71
#define SET_KD 0x72
#define CHANGE_SPEED  0x73

#define PI 3.14159265f
#define ENCODER_TIMEOUT 160   
static double radian = PI / 180;

int8_t m_estop = 1;

//uint8_t mode = CONTROLLER_NAVIGATION_MODE;
//uint8_t mode = CONTROLLER_RC_MODE;
uint8_t mode = CONTROLLER_AUTO_MODE;
int8_t modeOld = 0;
uint8_t button0 = 0;
float joy[4] = {0};
float u[6] = {0};

float error[3] = {0, 0, 0};
uint32_t debug[] = {0, 0, 0, 0};
float direction[3] = {1};
float scale = 100;    
int counter = 0;
uint8_t d_index = 1; 
float desired_speed[3] = {0}, actual_speed[3] = {0}, kp = 0.09, ki = 0.30, kd = 0.08, ks = 0.25;

float ds = 0; 
float debug_error = 0;
float debug_u = 0;
float previous_error[3] = {0};
float sum_integral[3] = {0};
float dt1 = 0.01; 
float slide_surf = 0;

int tim3_counter = 0; 
uint32_t tim3_cnt = 0; 
int b_ticks = 0, l_ticks = 0, r_ticks = 0; 
float f1 = 0, f2 = 0, f3 = 0; 

float lpulse = 0, ltime =0; 
float rpulse = 0, rtime =0; 
float bpulse = 0, btime =0; 

uint8_t m_auto_mode = MODE_IDLE;
uint16_t m_nb_ticks = 0;
int m_rot_direction = 1; 
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM5_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);



/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void usb_init(void)
{
	USBD_Init(&USBD_Device, &VCP_Desc, 0);
	USBD_RegisterClass(&USBD_Device, USBD_CDC_CLASS);
	USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_fops);
	USBD_Start(&USBD_Device);
}


void debug_rx(uint8_t *buf, int len)
{

	LOG("-- %d\t%d\t%d\t \n", buf[0], buf[1], buf[2]); 
	switch(buf[0]){

		case SET_KP:
			kp = buf[1] + buf[2] * 0.01;
			break;

		case SET_KI:
			ki = buf[1] + buf[2] * 0.01;
			break;

		case SET_KD:
			kd = buf[1] + buf[2] * 0.01;
			break;

		case CHANGE_SPEED:
			desired_speed[0] = buf[1] + buf[2] * 0.01; 	
			desired_speed[1] = buf[1] + buf[2] * 0.01; 	
			desired_speed[2] = buf[1] + buf[2] * 0.01; 	
			break;

		default:break;

	}
}

void Error_Handler(void)
{
	while(1) ;
}


void delay_ms(int d){
	HAL_Delay(d); 
}



void controller(int motor_id, float pulse)
{	

	switch (mode)
	{
		case 1: // kneeling RC driving, old mode 4
		case 3: // kneeling RC driving w/fixed gamma and variable theta
		case 4: // kneeling RC driving w/fixed gamma and variable theta
		case 31: // leaning backward before uprighting
		case 39:;  // leaning forward after downrighting
			//			actual_speed[0] = ((1 / pulse) * 9600) * radian;
			static float dt = 1 / 20.0;  // controller running at 20Hz 

			actual_speed[0] = ((1 / (rpulse * 39)) * 2000 * PI);
			actual_speed[1] = ((1 / (lpulse * 39)) * 2000 * PI);
			actual_speed[2] = ((1 / (bpulse * 39)) * 2000 * PI);
//			actual_speed[2] = ((1 / (bpulse * 70)) * 2000 * PI);
			//			actual_speed[0] = ((1 / (pulse * 39)) * 2000 * PI);
			direction[0] = (desired_speed[0] < 0) ? -1 : 1; 
			direction[1] = (desired_speed[1] < 0) ? -1 : 1; 
			direction[2] = (desired_speed[2] < 0) ? -1 : 1; 
	
			error[0] = (direction[0] * desired_speed[0]) - actual_speed[0];
			error[1] = (direction[1] * desired_speed[1]) - actual_speed[1];
			error[2] = (direction[2] * desired_speed[2]) - actual_speed[2];
			//                    error[2] = desired_speed[2] - actual_speed[2];


			sum_integral[0] += (error[0] * ki * dt);
			sum_integral[1] += (error[1] * ki * dt);
			sum_integral[2] += (error[2] * ki * dt);
			
			previous_error[0] = error[0]; 
			previous_error[1] = error[1]; 
			previous_error[2] = error[2]; 

			if( 0 == desired_speed[0]) 
			{
				sum_integral[0] = 0;  
				u[0] = 0;
				rpulse = 100000; 	
			} else {

				u[0] = (kp * error[0] + sum_integral[0] + kd * ((error[0] - previous_error[0]) / dt));   
			}			
			
			if( 0 == desired_speed[1]) 
			{
				sum_integral[1] = 0;  
				u[1] = 0;
				lpulse = 100000; 	
			} else {

				u[1] = (kp * error[1] + sum_integral[1] + kd * ((error[1] - previous_error[1]) / dt));   
			}
	
			if( 0 == desired_speed[2]) 
			{
				sum_integral[2] = 0;  
				u[2] = 0;
				bpulse = 100000; 	
			} else {

				u[2] = (kp * error[2] + sum_integral[2] + kd * ((error[2] - previous_error[2]) / dt));   
			}

			u[0] = u[0] * direction[0]; 
			u[1] = u[1] * direction[1]; 
			u[2] = u[2] * direction[2]; 

			break;

		default: // modes other than 1, 2, 3, 31, 39, 4, 41, and 49
			//  LOG("stateMachine mode "); LOG3(mode);LOG("\n");				
			for (uint8_t i = 0; i < 6; i++)
				u[i] = 0;
			break;

	}

	// motor saturation
	for (uint8_t i = 0; i < 6; i++)
	{
		if (u[i] < -ULIM)
			u[i] = -ULIM;
		else if (u[i] > ULIM)
			u[i] = ULIM;
	}

	// motor direction
	for (uint8_t i = 0; i < 6; i++)
		u[i] = UDIR[i] * u[i];


}





void stateMachine()
{
	modeOld = mode;

	button0 = parseUARTbuffer(joy);

	switch (button0)
	{

		case ESTOP:	// ESTOP motors
			LOG("ESTOP stateMachine mode \n"); 
			//	disableMotors();
			//m_estop = 1;
			break;
		case CLEAR_ESTOP: // disable ESTOP
			LOG("CLEAR_ESTOP stateMachine mode \n"); 
			enableMotors();
			//m_estop = 0;
			break;

		case ROTATE:
			desired_speed[1] = joy[0]  * 10;
			desired_speed[2] = joy[0]  * 10;
                         // adding motors threshold, assume it's 3		
			if( joy[0] != 0) {
				for(uint8_t i = 0; i < 3; i++) {
					if(desired_speed[i] < 0) 
					{ 
						desired_speed[i] -= 3;
					} else {
						desired_speed[i] += 3;
					} 

				}

			}	
			
			break;  
		case DRIVE:

			if( joy[0] != 0  || joy[1] != 0){
				desired_speed[0] = -((joy[1] - joy[0]) * 10);    // right motor  
				desired_speed[1] = (joy[1] + joy[0]) * 10;    // left motor  


			} else {

				desired_speed[0] = 0;
				desired_speed[1] = 0;
				desired_speed[2] = 0;
				actual_speed[0] = 0; 
				actual_speed[1] = 0; 
				actual_speed[2] = 0; 
				u[0] = 0;  
				u[1] = 0;  
				u[2] = 0;  
			}

		
			break; 
		case AUTO_MODE_FORWARD:
			// value in meters in u[0]. we get number of ticks 1m -> 143 ticks  
			m_nb_ticks = (uint16_t)( u[0] * 143); 
			m_auto_mode = MODE_FORWARD; 
			break;  
		case AUTO_MODE_ROTATE_LEFT:  
		case AUTO_MODE_ROTATE_RIGHT:
		m_rot_direction = ( mode == AUTO_MODE_ROTATE_LEFT)? -1 : 1; 	
			
			break; 
		default: 
			//LOG("stateMachine default ... \n");
			break;
	}


}


static void handle_encoders_stopped(void) 
{
           if( (millis() - btime) >= ENCODER_TIMEOUT) bpulse = 100000; 
           if( (millis() - ltime) >= ENCODER_TIMEOUT) lpulse = 100000; 
           if( (millis() - rtime) >= ENCODER_TIMEOUT) rpulse = 100000; 

}


static void handle_auto_forward(void)
{

	uint16_t current_nb_ticks = 0; 
	reset_encoders(); 
	while( current_nb_ticks < m_nb_ticks) {
		desired_speed[0] = -6; 
		desired_speed[1] = 6;
		delay_ms(20); 	
		current_nb_ticks = l_ticks;  
		handle_encoders_stopped(); 
		LOG(" cur %d l_tick %d \n", current_nb_ticks, l_ticks); 
	}

	m_auto_mode = MODE_IDLE; 
	desired_speed[0] = 0; 
	desired_speed[1] = 0;

}
static void handle_auto_rotate(void){

	uint16_t current_nb_ticks = 0; 
	int c = 0; 
	reset_encoders(); 
	while( current_nb_ticks < m_nb_ticks) {
		desired_speed[0] = 6; 
		desired_speed[1] = 6;
		desired_speed[2] = 6;
		delay_ms(20); 	
		current_nb_ticks = l_ticks;  
		handle_encoders_stopped(); 
		c++; 
		LOG(" cur %d l_tick %d c: %d \n", current_nb_ticks, l_ticks, c); 
	}
	m_auto_mode = MODE_IDLE; 
	desired_speed[0] = 0; 
	desired_speed[1] = 0;
	desired_speed[2] = 0;

}


static void handle_idle(void)
{
	mode = CONTROLLER_RC_MODE; 

}

static void handle_auto_mode(void)
{
	switch(m_auto_mode)
	{
		case MODE_FORWARD:
			handle_auto_forward(); 
			break; 
		case MODE_ROTATE:
			handle_auto_rotate(); 
			break; 
		case MODE_IDLE:
			handle_idle(); 	
			break; 
		default:break; 

	}

}

static void handle_logs(void)
{


		if((millis() % 200) == 0)
		{
//			GRP_LOG("  \t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f  \n", desired_speed[0], actual_speed[0], sum_integral[0], error[0], u[0], rpulse, lpulse, bpulse);
			HAL_Delay(25); 
		//LOG("... %f %f %f \n", desired_speed[0], desired_speed[1],desired_speed[2]); 
		}


}
/* USER CODE END 0 */

int main(void)
{

	HAL_Init();
	SystemClock_Config();

	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	MX_GPIO_Init();
	MX_TIM2_Init();   // timer for motors pwm 
	MX_TIM3_Init();   // timer for the controller 
	MX_TIM5_Init();   // TIM5 is used by the micros() function  
	MX_USART1_UART_Init();
	MX_USART3_UART_Init();
	MX_UART4_UART_Init();


	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);  // enable encoders 
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10, GPIO_PIN_RESET);  // turn on LED 

	enableMotors();

 
	m_auto_mode = MODE_ROTATE;  
	m_nb_ticks = 39; 
	
	while (1)
	{
		handle_auto_mode(); 
		handle_encoders_stopped(); 
		handle_logs(); 
	}

}



// motor timer = 20Hz 
void TIM3_IRQHandler(void)
{
	tim3_counter++; 
	__HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE); 
	stateMachine();
	controller(0, rpulse); 
	driveMotors(u);

	HAL_TIM_IRQHandler(&htim3);
}


void reset_encoders(){
	lpulse = 0; 
	rpulse = 0;
	bpulse = 0; 
	b_ticks = 0; 
	l_ticks = 0; 
	r_ticks = 0; 
}



// back wheel encoder 
void EXTI2_IRQHandler(void)
{
	bpulse= millis() - btime;
	btime = millis(); 
	b_ticks++; 
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);

}

// left wheel encoder  --> u[1]
void EXTI3_IRQHandler(void)
{
	lpulse= millis() - ltime;
	ltime = millis(); 
	l_ticks++; 
	
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);

}


// right wheel encoder  --> u[0]
void EXTI4_IRQHandler(void)
{
	rpulse= millis() - rtime;
	rtime = millis(); 
	r_ticks++; 
	
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);

}





/** System Clock Configuration
*/
void SystemClock_Config(void)
{

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct; 
	__HAL_RCC_PWR_CLK_ENABLE();

	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 168;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
		|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


/* TIM2 init function */
void MX_TIM2_Init(void)
{

	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_OC_InitTypeDef sConfigOC;

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 24;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 200;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim2);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);

	HAL_TIM_PWM_Init(&htim2);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);

	HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2);

	HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3);

	HAL_TIM_MspPostInit(&htim2);

}




/* TIM3 init function */
void MX_TIM3_Init(void)
{


	// APB1 timers clocks = 84 Mhz 

	htim3.Instance = TIM3;
	//htim3.Init.Prescaler = 84;
	htim3.Init.Prescaler = 8400;
	//	htim3.Init.Prescaler = 40000;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 500;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	htim3.Init.RepetitionCounter = 0; 
	HAL_StatusTypeDef st = HAL_TIM_Base_Init(&htim3);


	st = HAL_TIM_Base_Start_IT(&htim3); 

	HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);



}

/* TIM5 init function */
void MX_TIM5_Init(void)
{

	// APB1 timers clocks = 84 Mhz 

	htim5.Instance = TIM5;
	htim5.Init.Prescaler = 84;
	htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim5.Init.Period = 0xffff;
	htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	htim5.Init.RepetitionCounter = 0; 
	HAL_StatusTypeDef st = HAL_TIM_Base_Init(&htim5);
	HAL_TIM_Base_Start_IT(&htim5); 

	HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM5_IRQn);

  

}




/* USART3 init function */
void MX_USART3_UART_Init_c(void)
{

	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart3);

}


/** Configure pins as 
 * Analog 
 * Input 
 * Output
 * EVENT_OUT
 * EXTI
 PC9   ------> I2S_CKIN
 */
void MX_GPIO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;


	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);




	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin : PC9 */
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*
	   Configure GPIO pin : PD2 
	   */
	GPIO_InitStruct.Pin = GPIO_PIN_2;  
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	/*
	   Configure GPIO pin : PD10  LED1
	   Configure GPIO pin : PD11  LED2
	   */
	GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	// pin PA3 for motor 1 direction M1DIR -- back motor 
	/*Configure GPIO pin : PA3 */
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	// pin PC4 for motor 2 direction M2DIR
	// pin PC5 for motor 3 direction M3DIR
	// pin PC13 to enable motor 1  
	// pin PC14 to enable motor 2  
	// pin PC15 to enable motor 3  
	/*Configure GPIO pins : PC13 */
	/*Configure GPIO pin : PC5 */
	GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


	// pin PE6  to enable encoders --> ENCODER_EN
	/*Configure GPIO pins : PE6 */
	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	// external interrupts setup  

	// back wheel encoder 
	GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4; 
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	//GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);

	HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);

	HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);


}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
 * @brief Reports the name of the source file and the source line number
 * where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */

}

#endif

/**
 * @}
 */ 

/**
 * @}
 */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

