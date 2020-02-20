/*---------------------------------------------------------------------------------------------------------*/
/*                          Program Written By: SAAM OSTOVARI and Nick Morozovsky                          */
/*																		NDA SD2013-802								  												     			   */
/*---------------------------------------------------------------------------------------------------------*/

//Including Nuvoton Libraries
#include <stdio.h>
#include <stdint.h>
#include "M051.h"
#include "Register_Bit.h"
#include "Common.h"
#include "Retarget.h"
//Including MiP Libraries
#include "Encoders.h"
#include "..\UART\UART.h"
#include "..\Robot_Values\Robot_Values.h"
#include "..\..\Libraries\MiP_Libraries\System_Clock\System_Clock.h"



#define BIT5    0x00000020
#define BIT4    0x00000010

//Declaring Encoder Variables
float encoderLcount = 0;
float encoderRcount = 0;
float encoderBcount = 0;


float encoderRcountOld = 0;
float encoderLcountOld = 0;
float encoderBcountOld = 0;

float encoderLvelWMA = 0;
float encoderRvelWMA = 0;
float encoderRvelLP = 0;
float encoderLvelLP = 0;

float encoderLold[] = {0,0,0};
float encoderRold[] = {0,0,0};
float encoderBold[] = {0,0,0};

uint16_t ticks_encoder01 = 0;
uint16_t ticks_encoder02 = 0;
uint16_t ticks_encoder03 = 0;
unsigned long iscr = 0;

float lpulse = 0, ltime =0; 
float rpulse = 0, rtime =0; 
float bpulse = 0, btime =0; 
//int p00old = 0;
//int p01old = 0;
//int p44old = 0;
//int p45old = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//Initialize External Interuppts which Initalizes Encoder Readings
void Init_Encoders(void){
	Init_Port0_Ext_Int();													//Initialize External Interrupt on Port 0
	Init_Port4_Ext_Int();													//Initialize External Interrupt on Port 4
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//Setting Up External Interrupt on P0.0 and set P0.1 as an input for Quadrature Encoder
void Init_Port0_Ext_Int(void)
{
	
	P0_MFP = (P0_MFP & (~P01_AD1_RTS1)) | P01;		//Set P0.1 as external interrupt function.
	P0_MFP |= P01_SCHMITT;     										//Initial Schmitt Trigger function on P0.1
	P0_PMD |= Px1_PMD;	 			 										//Set P0.1 pin to Quasi-bidirectional mode
	P0_IMD &= IMD1_EDG;				 										//Set to Edge Trigger Interrupt
	P0_IEN |= IR_EN1;				 	 										//Set to Rising Edge Mode */
	P01_DOUT |= 0x1;                    //Set P0.1 output value high
	
	NVIC_ISER |= GP01_INT;		 										//Enable GP01_INT Interrupts 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//Setting Up External Interrupt on P4.4 and set P4.5 as an input for Quadrature Encoder
void Init_Port4_Ext_Int(void)
{
	P4_MFP = (P4_MFP & (~P44_CS )) | P44;					//Set up P4.4 as external interrupt function.
	P4_MFP |= P44_SCHMITT;     										//Initial Schmitt Trigger function on P4.4
	P4_PMD |= Px4_PMD;	  												//Set P4.4 pins to Quasi-bidirectional mode
	P4_IMD &= IMD4_EDG;				  									//Set to Edge Trigger Interrupt	
	P4_IEN |= IR_EN4;			 	  										//Set to Rising Edge Mode bidirectional mode
	P44_DOUT |= 0x1;                    //Set P4.4 output value high 
	
	P4_MFP = (P4_MFP & (~P45_ALE)) | P45;					//Set up P4.5 as external interrupt function.
	P4_MFP |= P45_SCHMITT;     										//Initial Schmitt Trigger function on P4.5
	P4_PMD |= Px5_PMD;	  												//Set P4.5 pins to Quasi-bidirectional mode
	P4_IMD &= IMD5_EDG;				  									//Set to Edge Trigger Interrupt							
	P4_IEN |= IR_EN5;			 	  										//Set to Rising Edge Mode
	P45_DOUT |= 0x1;                    //Set P4.5 output value high

	NVIC_ISER |= GP234_INT;	 	 										//Enable GP234_INT Interrupt
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//Update Encoder Variable with New Encoder Values. Averages Left and Right Encoder Counts
float Encoder_Update(){
  float encoder;
	encoder = (encoderLcount+encoderRcount)/2;  	//Using the Average of the Left and Right Encoder Values in the Estimator
  return encoder;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//Reset Encoder Variables for Safeties
void reset_Encoder(){
    encoderLcount=0; encoderRcount=0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Encoder velocity estimation using weighted moving average filter
float encVelWMA_R(void)
{
	encoderRvelWMA = ( 4*(encoderRcount-encoderRold[0])/dt + 2*(encoderRcount-encoderRold[1])/(2*dt) + (encoderRcount-encoderRold[2])/(3*dt) ) / 7;
  encoderRold[2] = encoderRold[1];
	encoderRold[1] = encoderRold[0];
  encoderRold[0] = encoderRcount;
  return encoderRvelWMA;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Encoder velocity estimation using weighted moving average filter
float encVelWMA_L(void)
{
	encoderLvelWMA = ( 4*(encoderLcount-encoderLold[0])/dt + 2*(encoderLcount-encoderLold[1])/(2*dt) + (encoderLcount-encoderLold[2])/(3*dt) ) / 7;
  encoderLold[2] = encoderLold[1];
	encoderLold[1] = encoderLold[0];
  encoderLold[0] = encoderLcount;
  return encoderLvelWMA;
}


float encVelWMA_B(void)
{
	float vel = ( 4*(encoderBcount-encoderBold[0])/dt + 2*(encoderBcount-encoderBold[1])/(2*dt) + (encoderBcount-encoderBold[2])/(3*dt) ) / 7;
  encoderBold[2] = encoderBold[1];
	encoderBold[1] = encoderBold[0];
  encoderBold[0] = encoderBcount;
  return vel;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Encoder velocity estimation using weighted moving average filter
float encVelLP_R(void)
{
	encoderRvelLP = encoderRvelLP + encVelLPC*((encoderRcount-encoderRcountOld)/dt - encoderRvelLP);
	encoderRcountOld = encoderRcount;
  return encoderRvelLP;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Encoder velocity estimation using weighted moving average filter
float encVelLP_L(void)
{
	encoderLvelLP = encoderLvelLP + encVelLPC*((encoderLcount-encoderLcountOld)/dt - encoderLvelLP);
	encoderLcountOld = encoderLcount;
  return encoderLvelLP;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//Functions to Return Specific Values
float get_encoderLcount(void){return encoderLcount;}
float get_encoderRcount(void){return encoderRcount;}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//Function to Print Out Debugging Info for Encoders
void Outputs4Debugging_Encoders(void)
{
	//printf("%f\t%f\t",encoderRcount,encoderLcount);
//	printf("%6d \t %.1f \t",ticks_encoder01 , lpulse);
	//printf("%6d \t %6d \t %6d  %f \n",ticks_encoder01 , ticks_encoder02, ticks_encoder03, lpulse);
//	printf("%.3f \t %.3f \t %.3f   \n", bpulse, rpulse, lpulse );
		printf("%.3f  \n", rpulse );
	//printf("%f\t%f\t",encoderRvelWMA,encoderLvelWMA);
	//printf("%f\t%f\t \n",encoderRvelLP,encoderLvelLP);
//	printf("%.1f \t  %.1f \t \n",ltime, millis());
	//printf("%f\t%f\t",encoderLcount,encoderLvelLP);
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
////INTERUPT FUNCTIONS

//Port0,1 Interrupt Function
void GPIOP0P1_IRQHandler(void)  // back encoder 
{

	ticks_encoder01++ ;
	encoderBcountOld++;
  bpulse= millis()-btime;
  btime = millis(); 

	P0_ISRC = P0_ISRC;														//Notifies processer that Interrupt has triggered	
  
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
void reset_pulse(){
 lpulse = 0; 
 rpulse = 0;
 bpulse = 0; 
}
float get_pulse(uint8_t pulse_id){

	float pulse = 0;	
	switch(pulse_id){

		case 0: 
			if( (millis() - rtime) > 150) {
				pulse = 100000;
			} else {
				pulse = rpulse;
			}

			break;

		case 1: 
			if( (millis() - ltime) > 150) {
				pulse = 100000;
			} else {
				pulse = lpulse;
			}


			break;
		case 2: 

			if( (millis() - btime) > 150) {
				pulse = 100000;
			} else {
				pulse = bpulse;
			}
			break;

	}
	return pulse;
}
//Port2,3,4 Interrupt Function
void GPIOP2P3P4_IRQHandler(void)
{

	if( P4_ISRC & BIT5)  // right encoder 
	{

		ticks_encoder03++ ;
		rpulse= millis()-rtime;
		rtime = millis(); 
	}

	if( P4_ISRC & BIT4)  // left encoder 
	{  
		lpulse= millis()- ltime;
		ltime = millis(); 
		ticks_encoder02++ ;
	}

	P4_ISRC = P4_ISRC; 	 //Notifies processer that Interrupt has triggered

}
