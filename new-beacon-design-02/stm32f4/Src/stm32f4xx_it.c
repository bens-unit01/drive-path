#include <stdbool.h>
#include "stm32f4xx_hal.h"
//#include "cmsis_os.h"

void NMI_Handler(void)
{
	while (1) ;
}

void HardFault_Handler(void)
{
	while (1) ;
}

void MemManage_Handler(void)
{
	while (1) ;
}

void BusFault_Handler(void)
{
	while (1) ;
}

void UsageFault_Handler(void)
{
	while (1) ;
}

/* SVC_Handler() handled by FreeRTOS */

void DebugMon_Handler(void)
{
}

/* PendSV_Handler() handled by FreeRTOS */

void SysTick_Handler(void)
{
	HAL_IncTick();
	//osSystickHandler();
}

void OTG_HS_IRQHandler(void)
{
	 extern PCD_HandleTypeDef hpcd;
	 HAL_PCD_IRQHandler(&hpcd);
}



void TIM4_IRQHandler(void)
{
	extern TIM_HandleTypeDef TimHandle;
	HAL_TIM_IRQHandler(&TimHandle);

}


void TIM5_IRQHandler(void)
{
	
        extern uint32_t TIM5OF;
	extern TIM_HandleTypeDef htim5;
	TIM5OF++; 
	__HAL_TIM_CLEAR_IT(&htim5, TIM_IT_UPDATE); 
	HAL_TIM_IRQHandler(&htim5);

}
