/*---------------------------------------------------------------------------------------------------------*/
/*                      Program Written By: Nick Morozovsky, based on template by SAAM OSTOVARI            */
/*																			NDA SD2013-802																				     			   */
/*---------------------------------------------------------------------------------------------------------*/
/*  Note: System clock uses PLL instead of the external 12MHz	like all my other test programs 				 		 */
/*---------------------------------------------------------------------------------------------------------*/

//Including Libraries
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "M051.h"
#include "Register_Bit.h"

#include "Common.h"
#include "Retarget.h"
#include "Macro_SystemClock.h"
#include "Macro_Timer.h"
//Including MiP libraries 
#include "..\..\Libraries\MiP_Libraries\System_Clock\System_Clock.h"
#include "..\..\Libraries\MiP_Libraries\UART\UART.h"
#include "..\..\Libraries\MiP_Libraries\SPI\SPI.h"
#include "..\..\Libraries\MiP_Libraries\IMU\IMU.h"
#include "..\..\Libraries\MiP_Libraries\Timers\Timers.h"
#include "..\..\Libraries\MiP_Libraries\GPIO\GPIO.h"
#include "..\..\Libraries\MiP_Libraries\PWM\PWM.h"
#include "..\..\Libraries\MiP_Libraries\ADC\ADC.h"
#include "..\..\Libraries\MiP_Libraries\Encoders\Encoders.h"
#include "..\..\Libraries\MiP_Libraries\Motor_Drive\Motor_Drive.h"
#include "..\..\Libraries\MiP_Libraries\Estimator\Estimator.h"
//#include "Controller\Controller.h"
#include "..\..\Libraries\MiP_Libraries\Robot_Values\Robot_Values.h"
#include "..\..\Libraries\MiP_Libraries\Interrupt_Priority\Interrupt_Priority.h"
#include "..\..\Libraries\MiP_Libraries\Switchbot\Switchbot.h"

//static int loop = 0; 

#define PI 3.14159265 
static double radian = PI / 180;

static uint32_t Timer_Count = 0;
static float debug_float[10] = {0};
//static uint32_t Timer_Count1 = 0;
uint32_t debug[] = {0, 0, 0, 0};
float direction[3] = {1};
float scale = 100;    
int counter = 0;
uint8_t d_index = 1; 
//float desired_speed[3] = {0}, actual_speed[3] = {0}, kp = 20, ki = 0.2, kd = 0.5, ks = 2.0;
float desired_speed[3] = {0}, actual_speed[3] = {0}, kp = 5, ki = 0, kd = 0, ks = 0.25;
//float desired_speed[3] = {0}, actual_speed[3] = {0}, kp = 5, ki = 1.9, kd = 0.01;
//float desired_speed[3] = {0}, actual_speed[3] = {0}, kp = 5, ki = 2.34, kd = 0, ks = 2.0;
//float desired_speed[3] = {0}, actual_speed[3] = {0}, kp = 5, ki = 3, kd = 3, ks = 2.0;
float ds = 0; 
float debug_error = 0;
float debug_u = 0;
float previous_error[3] = {0};
float sum_error[3] = {0};
float dt1 = 0.01; 
float slide_surf = 0;

int i;
int j;
float u1y;
float u2y;
float u3y;
float xhat[14];
float ref[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float refOld[14] = {0};
float u[6] = {0};
float pots[4];
float potsLP[4] = {0,0,0,0};
float potVelLP[4] = {0,0,0,0};
float potVelWMA[4] = {0,0,0,0};
float potsOld[4][numPotVelWMAsamples] = {{0}};
float potsMFold[4][numPotMFsamples] = {{0}};
float potsMed[numPotMFsamples] = {0};
float temp = 0;
float gyroLP = 0;
float ustar[6] = {0,0,0,0,0,0};
float joy[4] = {0};
float yawRef = 0;
int8_t button0 = 0;
int8_t button1 = 0;
int8_t mode = 3; // probably want to switch to 3 after initial testing
int8_t modeOld = 0;
int8_t m_estop = 1;
//float timeStamp = 0;
//float execTime = 0;
//float loopStart = 0;
//float beforeDebug = 0;
//float afterDebug = 0;
const double UDIR[] = {1.0, -1.0, -1.0, -1.0, 1.0, -1.0};					// was not previously defined in V1 (all positive)


int nGains = 6; // number of gains for equilibrium manifold, gains defined below

float t;
float tread = 0;
float treadd = 0;
float ts_01 = 0; 
float ts_02 = 0; 

/////////////////////////////////////////////////////////////////////////////////////////////////////////    
static  void delay_ms(float delay){
	float ts = millis();
	while((millis() - ts) < delay);

}


static void LOG( const char * str)
{
#ifdef DEBUG_MODE
	printf((const char *)str);
#endif
}


static void LOG2(int input)
{
#ifdef DEBUG_MODE
	char str[24];
	snprintf(str, sizeof str, "%lu", (unsigned long) input);
	LOG(str);
#endif
}

static void LOG3(int input)
{
#ifdef DEBUG_MODE
	char str[24];
	snprintf(str, sizeof str, "%d",  input);
	LOG(str);
#endif
}


void stateMachine()
{
	modeOld = mode;
	
	button0 = parseUARTbuffer(joy);

	
	switch (button0)
	{

		case ESTOP:	// ESTOP motors
			LOG("stateMachine mode "); LOG3(mode);LOG("\n");
			motorESTOP();
			m_estop = 1;
			break;
		case CLEAR_ESTOP: // disable ESTOP
	 	  LOG("-- stateMachine mode "); LOG3(mode);LOG("\n");
			motorEnable();
			m_estop = 0;
			break;
	        case SET_KP:
			kp = joy[0];
		  joy[0] = 0;
		  break;
		case SET_KI:
			ki = joy[0];
		  joy[0] = 0;
		  break;
		case SET_KD:
			kd = joy[0];
		  joy[0] = 0;
		  break;
		  
		case CHANGE_SPEED:
			ds = joy[0];
		        joy[0] = 0;
		  break;
	
		default: 
			//LOG("stateMachine default ... \n");
			break;
	}
	

}



void controller()
{	
static float old_u0 = 0, old_u1 =0, old_u2 = 0; 
	switch (mode)
	{
		case 1: // kneeling RC driving, old mode 4
		case 3: // kneeling RC driving w/fixed gamma and variable theta
		case 31: // leaning backward before uprighting
		case 39: // leaning forward after downrighting
			switch (mode)
			{
				case 1:
				case 3: 

			if(m_estop == 1) {
			  sum_error[0] = 0; 
			  sum_error[1] = 0; 
			  sum_error[2] = 0; 
			} 


//			 direction[0] =(joy[0] > 0)? 1 : -1;
//			 direction[1] =(joy[1] > 0)? 1 : -1;
//			 direction[2] =(joy[2] > 0)? 1 : -1;
			
			
			 if(joy[0] == 0){
//     			         direction[0] = 0;
				 sum_error[0] = 0;
			 }			 
	
			 if(joy[1] == 0){
//     			         direction[1] = 0;
				 sum_error[1] = 0;
			 }		

			 if(joy[2] == 0){
//     			         direction[2] = 0;
				 sum_error[2] = 0;
			 }		

                         counter++;

 		          //desired_speed[0] = 400 * radian;
// 		          desired_speed[0] = ds * radian;
//		  	desired_speed[0] = joy[0] * radian;   
//		  	desired_speed[1] = joy[1] * radian;   
//		  	desired_speed[2] = joy[2] * radian;   
  	
			 actual_speed[0] = ((1 /get_pulse(0)) * 9600) * radian;
			 actual_speed[1] = ((1 /get_pulse(1)) * 9600) * radian;
			 actual_speed[2] = ((1 /get_pulse(2)) * 9600) * radian;

		
			float error[3] = {0, 0, 0};
                        //error[0] = desired_speed[0] - actual_speed[0];
                        error[0] = desired_speed[0] - actual_speed[0];
                        error[1] = desired_speed[1] - actual_speed[1];
                        error[2] = desired_speed[2] - actual_speed[2];
			
		  // debug data
			debug_float[0] = desired_speed[0];
			debug_float[1] = actual_speed[0];
			debug_float[2] = error[0];
			
			
			sum_error[0] += error[0];
			sum_error[1] += error[1];
			sum_error[2] += error[2];


	  debug_float[3] = (kp * error[0] + ki * sum_error[0] + kd * (error[0] - previous_error[0])) / scale; 
	  u[0] =  direction[0] * (debug_float[3] + ((desired_speed[0] * 0.35f)/(400.0f * radian))); 
	  u[1] =  direction[1] * (((kp * error[1] + ki * sum_error[1] + kd * (error[1] -	previous_error[1])) / scale) + ((desired_speed[1] * 0.35f)/(400.0f * radian))); 
	  u[2] =  direction[2] * (((kp * error[2] + ki * sum_error[2] + kd * (error[2] - previous_error[2])) / scale) + ((desired_speed[2] * 0.35f)/(400.0f * radian))); 

	                  debug_float[4] = u[0]; 

				
			break;

	   	default: // modes other than 1, 2, 3, 31, 39, 4, 41, and 49
				  LOG("stateMachine mode "); LOG3(mode);LOG("\n");				
			for (i = 0; i < 6; i++)
				u[i] = 0;
			break;
		}
	}
	
	// motor saturation
	for (i = 0; i < 6; i++)
	{
		if (u[i] < -ULIM)
			u[i] = -ULIM;
		else if (u[i] > ULIM)
			u[i] = ULIM;
	}
	
	
	// motor direction
	for (i = 0; i < 6; i++)
		u[i] = UDIR[i] * u[i];
	
}



void test_01(void) {

// rotation 

	desired_speed[0] = 300 * radian;
	desired_speed[1] = 300 * radian;
	desired_speed[2] = 300 * radian;

	direction[0] = 1;
	direction[1] = 1;
	direction[2] = 1;

	float rot1 = 90, rot2 = 90, trans = 1;
	float theta;
	int indicator = 0;
	float dt2 = 1 / 68693, ratio = 165 / 37.5, theta_desired1 = ratio * rot1 * radian, theta_desired2 = ratio * rot2 * radian;
	float velocity = 0;

	dt2 = dt2 * 1000.0f;

	theta = 0;
	while (theta < (theta_desired1)) {
		indicator++;
		velocity = (actual_speed[0] + actual_speed[1] + actual_speed[2]) / 3;
		theta += velocity * 0.000042;
	}
	printf(" dt2 %f %f %f %d \n ", dt2, theta, theta_desired1, indicator);

	desired_speed[0] = 0 * radian;
	desired_speed[1] = 0 * radian;
	desired_speed[2] = 0 * radian;

	printf("rotation end %d \n", indicator);

// translation 
     delay_ms(1000); 
     
	desired_speed[0] = 300 * radian;
	desired_speed[1] = 300 * radian;
	desired_speed[2] = 0 * radian;

	direction[0] = -1;
	direction[1] = 1;

	float distance = 0;
	float vt1 = 0, vt2 = 0, vt = 0;

	while (distance < trans) {
		indicator++;
		vt1 = 0.375 * actual_speed[0];
		vt2 = 0.375 * actual_speed[1];
		vt = vt1 * cos(PI / 3) + vt2 * cos(PI / 3);
		distance += vt * 0.000042;
	}

	printf("translation end ------------------------------------- %d \n",
			indicator);
// rotation -------------- 
	desired_speed[0] = 0 * radian;
	desired_speed[1] = 0 * radian;
	desired_speed[2] = 0 * radian;

    delay_ms(1000); 
    
	desired_speed[0] = 300 * radian;
	desired_speed[1] = 300 * radian;
	desired_speed[2] = 300 * radian;

	direction[0] = 1;
	direction[1] = 1;
	direction[2] = 1;

	theta = 0;
	while (theta < (theta_desired2)) {
		indicator++;
		velocity = (actual_speed[0] + actual_speed[1] + actual_speed[2]) / 3;
		theta += velocity * 0.000042;
	}

	desired_speed[0] = 0 * radian;
	desired_speed[1] = 0 * radian;
	desired_speed[2] = 0 * radian;

	printf("2nd  rotation end ---------------- %d \n", indicator);

}

int main(void)
{   
	float a1 = -57.0;
	Init_System_Clocks_PLL();				//Initialize System Clocks
	Init_Uart0(115200);							//Initialize UART0 and set to baud rate of 115200
	Init_Uart1(115200);	
	Init_GPIO_Output(3,6);			//Initialize GPIO as output out on (Port,Pin) for onboard LED
	Init_Motors();				//Initialize all Motors with PWM and Direction GPIO pins on Switchbot Carrier Board
	motorESTOP();							// turn off all motor drivers
        Init_Interrupt_Priority(9);			// set interrupt priorities
	//Init_Interrupt_Priority(10);			// set interrupt priorities
	Init_Timer2_ISR();   		//Initalize Timer 2 ISR. ISR can be located in Timer.c library file or in this main file. Currently set to 10ms
	Init_System_Millis_Timer();
        Init_Encoders(); 
 
	Reset_millis();
	m_estop = 0;
        motorEnable(); 
ts_02 = millis();

delay_ms(1000); 

//	Init_Digital_IMU2();							//Initialize IMU	
//	Initialize_Estimator();		
        
        test_01();	
	
	//maskK(r);
        //a1 = millis();
	// printf("App started ...................... %f  \n", a1 );
  while(1)
  { 

		 debug[0]++;
		if( (millis() - ts_01) > 100){
			  debug[1]++;
			if(debug[1] > 62) debug[1] = 0; 
		   ts_01 = millis(); 
		
		   	 printf("\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t\n",  debug_float[0], debug_float[1], debug_float[2], debug_float[3], debug_float[4]);
		}
		
		if( (millis() - ts_01) < 0) ts_01 = millis();
   }
	
	 
}
			 
/////////////////////////////////////////////////////////////////////////////////////////////////////////                                                                                      

////Timer2 ISR routine (10ms)
void TMR2_IRQHandler(void)
{
	TISR2 |= TMR_TIF; 																		// Clear timer2 interrupt flag

	Timer_Count++;																							
	// heart beat LED
	if (m_estop == 0) // flash slowly if ESTOP is off
		if (Timer_Count%150 > 75)	digitalWrite(3,6,0);
		else	digitalWrite(3,6,1);
	else						// flash quickly if ESTOP is on
		if (Timer_Count%20 > 10)	digitalWrite(3,6,0);
		else	digitalWrite(3,6,1);

	IMU_Update3();
	stateMachine();		// set mode from user input (over UART0) and state estimate
	controller();			// calculate motor commands from mode, user input, reference, and state estimate	
	driveMotors(u);		// update motor speed commands

	
}


