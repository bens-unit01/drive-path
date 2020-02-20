/*---------------------------------------------------------------------------------------------------------*/
/*                          Program Written By: SAAM OSTOVARI and Nick Morozovsky                          */
/*																			NDA SD2013-802																				     			   */
/*---------------------------------------------------------------------------------------------------------*/
/*								Please pay attention to the format of the value you are getting from the buffer 				 */		
/*															as well as the format in which you are printing it out										 */	
/*---------------------------------------------------------------------------------------------------------*/

//Including Nuvoton Libraries
#include <stdio.h>
#include <stdint.h>
#include "M051.h"
#include "Register_Bit.h"
#include "Common.h"
#include "Retarget.h"
#include "math.h"
//Including MiP Libraries
#include "UART.h"
#include "..\Robot_Values\Robot_Values.h"
#include "..\System_Clock\System_Clock.h"
#include "..\Switchbot\Switchbot.h"




 
unsigned long RX0Data[UART0BytesSent] = {0};						//UART0BytesSent is a value set in Robot Values that signifies the number of bytes that will be sent by UART0
unsigned long  RX1Data[UART1BytesSent] = {0};	
unsigned long temp1 = 0;
unsigned long junk = 0;
//uint16_t RX1Data[UART1BytesSent] = {0};						
int count0 = 0;
float timestamp0 = 0;
float timestamp1 = 0;
int count1 = 0;

int uart0_timeout = 0;
int uart1_timeout = 0;
static double radian = PI / 180;     

static void LOG( const char * str)
{
#ifdef DEBUG_MODE
	
   // printf(str);
 
#endif
}



void uart_put(uint8_t uart_port, uint8_t ch)
{
	
	switch(uart_port){
		case UART0 : 
			while ((UA0_FSR&TX_EMPTY) == 0x00); 	//check Tx Empty
	     UA0_THR = ch;
    	 if(ch == '\n')
	     {
          while ((UA0_FSR&TX_EMPTY) == 0x00); 
	        UA0_THR = '\r';
	      }
	  	break;
		case UART1 : 
		    while ((UA1_FSR&TX_EMPTY) == 0x00); 
	     UA1_THR = ch;
    	 if(ch == '\n')
	     {
          while ((UA1_FSR&TX_EMPTY) == 0x00); 
	        UA1_THR = '\r';
	      }	
		break;
	
	}
	
}
//int jj;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// parse UART0 buffer and set command variables

/*int parseUART0buffer(float joy[])
{
	
	// checksum is calculated by adding message bytes, chopping off overflow bits, and taking the 2's complement (subtracting from 2^8 = 256)
	// check checksum by adding all bytes, chopping off overflow bits, and comparing to 0
	int checksum = 0;
	for (jj = 0; jj < UART0BytesSent; jj++)
		checksum = checksum + (int) RX0Data[jj];
	checksum = checksum & 0xFF;
	if (checksum == 0) // checks out
	{
		for (jj = 0; jj < 4; jj++)
			joy[jj] = ((float)(((int) RX0Data[1+jj]) - JOYSTICK_ZERO)) / JOYSTICK_SCALE;
		return (int) RX0Data[0];
	}
	else // checksum doesn't match, don't update joysticks or button
		return 0; // means no button pressed
} //*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// parse UART0 buffer and set command variables

int parseUARTbuffer(float joy[])
{
	int button = 0;
  float m = 0;
	joy[0] = 0;
	joy[1] = 0;
	joy[2] = 0;

// Handling UART0	- serial connection to BLE
   m = (millis() - timestamp0);
	if( m < 0) {  // bug fix where the millis() restart counting after ~ 3min, maybe we should change millis() implementation ?	          
	  RX0Data[0] = 0;
	} else if ( m < UART_TIMEOUT)
	{
		switch(RX0Data[0])
		{
			
			case 0x50: // push to talk has been pressed
				LOG("P");
				break;
			case ESTOP: // emergency stop
				button = ESTOP;
				break;
			case CLEAR_ESTOP: // reset emergency stop
				button = CLEAR_ESTOP;
				break;
			case DRIVE: // drive, any mode
			LOG("parseUART0buffer case 0x78 \n");
				// 0 is stop, 01-32 is forward, 33-64 is backwards
				// 0 is straight, 65-96 is right, 97-128 is left
			//-----------------
			//	   uart_put(UART1, RX0Data[1]);
			//     uart_put(UART1, RX0Data[2]);
			//---------------
		//	if((RX0Data[1] == 0x78) ||  (RX0Data[2] == 0x78)) return 1;
			    int theta = RX0Data[1];
			    if( (theta < 0) || (theta > 120) ) return 0;
			    //float vb = 0.4;
			    float vb = 400;
                            theta = (theta * 3) - 90;
			    
			    float angle1 = (150 - theta) * radian;          
                            float angle2 = (30 - theta) * radian;           
                            float angle3 = (270 - theta)  * radian;          

                            double v1 = vb * cos(angle1);                
                            double v2 = vb * cos(angle2 );               
                            double v3 = vb * cos(angle3 );               
			    
			       joy[0] = (float) v1;  
			       joy[1] = (float) v2;
			       joy[2] = (float) v3;
	         //   printf("%3d\t%.3f\t%.3f\t%.3f  \n", theta * 3, joy[0], v2, v3);   
				
				break;
			case 0x82: ;
				   uint8_t direction = RX0Data[1];
			     float speed = 0;
					 /*
           if( direction >= 50){
					     speed  = -(direction - 50) / 50.0f; 
					 } else {
					      speed  = (direction) / 50.0f; 
					 }
					 */
					  if( direction >= 50){
					     speed  = -(direction - 50); 
					 } else {
					      speed  = (direction); 
					 }
					 
					
					   joy[0] =  speed * 20;  
					    
			 //      joy[1] =  speed;
			  //     joy[2] =  speed;
  		       //        printf(" %.3f \n", speed); 		 
			     break;
					 
				case 0x83: ;
					  uint8_t sp = RX0Data[1];
					 joy[0] =  sp * 9.0f;  
					 
					 break;
			case NOTF_GET_NEXT_BEACON:
			case NOTF_DP_CLOSEST_BEACON:
				   uart_put(UART1, RX0Data[0]);
			     uart_put(UART1, RX0Data[1]);
      	   uart_put(UART1, 0);
			     RX0Data[0] = 0;      // we reset the command data to avoid resending it again
			   break;
			case NOTF_NORDIC_MB_TEST:	
				 	 uart_put(UART1, RX0Data[0]);
			     uart_put(UART1, RX0Data[1]);
      	   uart_put(UART1, RX0Data[2]);
			     RX0Data[0] = 0;      // we reset the command data to avoid resending it again
			    break;
			 default:	break;
		}
	} 
	
	
	// Handling UART1 - serial connection to Android board 
	   m = (millis() - timestamp1);
	
	if( m < 0) {  // bug fix where the millis() restart counting after ~ 3min, maybe we should change millis() implementation ?	          
	  RX1Data[0] = 0;
	} else if ( m < UART_TIMEOUT)
	{
		switch(RX1Data[0])
		{
			case 0x50: // push to talk has been pressed
				LOG("P");
				break;
			case ESTOP: // emergency stop
				button = ESTOP;
				break;
			case CLEAR_ESTOP: // reset emergency stop
				button = CLEAR_ESTOP;
				break;
			case DRIVE: // drive, any mode
			LOG("parseUART1buffer case 0x78 \n");
			
				// 0 is stop, 01-32 is forward, 33-64 is backwards
				// 0 is straight, 65-96 is right, 97-128 is left
				
			
				break;
			case DP_NORDIC_MB_TEST:	
			case DP_STOP: 	
			case DP_GOTO_BEACON:
      case DP_CHANGE_RANGE:	
      case DP_GET_CLOSEST_BEACON:      				
          uart_put(UART0, RX1Data[0]);
			    uart_put(UART0, RX1Data[1]);
			    uart_put(UART0, 0);
			    RX1Data[0] = 0;      // we reset the command data to avoid resending it again
    			break;

			case NOTF_DP_TARGET_REACHED:    				
                            uart_put(UART1, RX1Data[0]);
			    uart_put(UART1, 0);
			    uart_put(UART1, 0);
			    RX1Data[0] = 0;      // we reset the command data to avoid resending it again
    			break;	
		case 0x83: ;
				//	  uint8_t sp = RX1Data[1];
				//	 joy[0] =  sp * 9.0f;  
			   break;
		case SET_KP: 
			    joy[0] = RX1Data[1] + RX1Data[2] * 0.01;
			    button = SET_KP;
			   break;
		
                 case SET_KI: 
			    joy[0] = RX1Data[1] + RX1Data[2] * 0.01;
			    button = SET_KI;
			   break;	
		
                  case SET_KD: 
		   	 joy[0] = RX1Data[1] + RX1Data[2] * 0.01;
			    button = SET_KD; 
			   break;					
 
                 case CHANGE_SPEED: 
		   	 joy[0] = RX1Data[1] * 10 + RX1Data[2];
			    button = CHANGE_SPEED; 
			   break;					
       				
			default:break;
		}
	}
	

	return button;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function to set up and initialize UART0. Must pass desired Baud Rate to this function
//Use printf to output over UART0

void Init_Uart0(uint32_t BaudRate)
{
	
	/* Step 1. Set specific GPIOs to UART */ 
   P3_MFP &= ~(P31_TXD0 | P30_RXD0);   
   P3_MFP |= (TXD0 | RXD0);    					//P3.0 --> UART0 RX and P3.1 --> UART0 TX
	/* Step 2. Enable and Select UART clock sources*/
   APBCLK |= UART0_CLKEN;  							// Enable UART0 clock
	 CLKSEL1 = ((CLKSEL1 & (~UART_CLK)) | UART_12M); //Set Uart clock source to 2MHz External clock
	 CLKDIV &= ~(15<<8); 	       					//UART Clock DIV Number = 0;
	/* Step 3. Select Operation mode */
   IPRSTC2 |= UART0_RST;   							//Reset UART0
   IPRSTC2 &= ~UART0_RST; 							//Reset end
   UA0_FCR |= TX_RST;      							//Tx FIFO Reset
   UA0_FCR |= RX_RST;      							//Rx FIFO Reset
	 UA0_LCR &= ~PBE;     	 							//Parity Bit Disable
	 UA0_LCR &= ~WLS;
   UA0_LCR |= WL_8BIT;     							//8 bits Data Length 
   UA0_LCR &= NSB_ONE;     							//1 stop bit
 /* Step 4. Set BaudRate to 115200*/
   UA0_BAUD |= DIV_X_EN;   							//Mode2:DIV_X_EN = 1
   UA0_BAUD |= DIV_X_ONE;  							//Mode2:DIV_X_ONE = 1
	/* For XTAL = 12 MHz */	
   UA0_BAUD |= ((12000000 / BaudRate) -2);	//Set BaudRate to 115200;  UART_CLK/(A+2) = 115200, UART_CLK=12MHz
  /* Enable Interrupt */ 								//Hardware interrupt for UART to know when it is getting data and when it finish. a heads up signal 
	 UA0_IER	|= (RDA_IEN);
	 NVIC_ISER |= UART0_INT;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//UART0 Interrupt called everytime data is recieved by UART1. It prints data out through UART0

 void UART0_IRQHandler(void)
{
	int UART0BuffCount=0;
	int UART0BuffEmpty=1;
  
	UART0BuffCount = (((UA0_FSR)<<18)>>26);												//Outputs the number of bytes in the UART1 RX buffer
	if(UART0BuffCount>=UART0BytesSent){count0=1; timestamp0 = millis();}
	//printf("%d\n",UART0BuffCount);
	
	if(count0==1){
		uart0_timeout = 0;
		//printf("Buff Count: %d\t\t",UART0BuffCount);
		RX0Data[UART0BytesSent-UART0BuffCount] = UA0_RBR;												//Reads the oldest byte of data in the buffer. 		  		
		//printf("UART0 Data: %lu\t\t", RX0Data[UART0BytesSent-UART0BuffCount]);
		//printf("%lu\n",RX0Data[UART0BytesSent-UART0BuffCount]);
		UART0BuffEmpty = ((UA0_FSR & RX_EMPTY)>>14);								//Outputs 1 when buffer is empty
		//printf("Empty(y=1,n=0): %d\n",UART0BuffEmpty);
		if(UART0BuffEmpty ==1){count0=0;}
	}
	
	// detecting if something gets stuck on the Rx register (paquets less than 3 bytes) 
	uart0_timeout++;
	if(uart0_timeout >= 100){
		uart0_timeout = 0;
		junk = UA0_RBR;  // reseting the Rx register 
	}
}

// UART1 commented out to try to save space
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function to set up and initialize UART1. Must pass desired Baud Rate to this function
//A write function still needs to be written. printf does not work with UART1

void Init_Uart1(uint32_t BaudRate)
{
	// Step 1. Set specific GPIOs to UART
   P1_MFP &= ~(P13_AIN3_TXD1 | P12_AIN2_RXD1);   
   P1_MFP |= (TXD1 | RXD1);    				//P3.0 --> UART0 RX and P3.1 --> UART0 TX
	// Step 2. Enable and Select UART clock sources
   APBCLK |= UART1_CLKEN;  						// Enable UART0 clock
	 CLKSEL1 = ((CLKSEL1 & (~UART_CLK)) | UART_12M); //Set Uart clock source to 2MHz External clock
	 CLKDIV &= ~(15<<8); 	       				//UART Clock DIV Number = 0;
	// Step 3. Select Operation mode
   IPRSTC2 |= UART1_RST;   						//Reset UART0
   IPRSTC2 &= ~UART1_RST; 						//Reset end
   UA1_FCR |= TX_RST;      						//Tx FIFO Reset
   UA1_FCR |= RX_RST;      						//Rx FIFO Reset
	 UA1_LCR &= ~PBE;     	 						//Parity Bit Disable
	 UA1_LCR = (UA1_LCR & (~WLS)) | WL_8BIT;  //8 bits Data Length
   UA1_LCR &= NSB_ONE;                      //1 stop bit
 // Step 4. Set BaudRate to 115200
   UA1_BAUD |= DIV_X_EN;   						//Mode2:DIV_X_EN = 1
   UA1_BAUD |= DIV_X_ONE;  						//Mode2:DIV_X_ONE = 1
	// For XTAL = 12 MHz 
   UA1_BAUD |= ((12000000 / BaudRate) -2);	//Set BaudRate to 115200;  UART_CLK/(A+2) = 115200, UART_CLK=12MHz
  // Enable Interrupt 							//hardware interrupt for uart to know when it is getting data and when it finish. a heads up signal 
	 UA1_IER	|= (RDA_IEN);
	 NVIC_ISER |= UART1_INT;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//UART1 Interrupt called everytime data is recieved by UART1. It prints data out through UART0
 
void UART1_IRQHandler(void)
{
 	int UART1BuffCount=0;
	int UART1BuffEmpty=1;

	UART1BuffCount = (((UA1_FSR)<<18)>>26);												//Outputs the number of bytes in the UART1 RX buffer
	if(UART1BuffCount>=UART1BytesSent){count1=1; timestamp1 = millis();}
	//printf("%d\n",UART1BuffCount);
	if(count1==1){
		uart1_timeout = 0;
	//	printf("Buff Count: %d\t\t",UART1BuffCount);
		RX1Data[(UART1BytesSent-UART1BuffCount)] = UA1_RBR;												//Reads the oldest byte of data in the buffer. 		  		
	//	printf("UART1 Data: %d\t\t", RX1Data[(UART1BuffCount)]);
		UART1BuffEmpty = ((UA1_FSR & RX_EMPTY)>>14);								//Outputs 1 when buffer is empty
	//	printf("Empty(y=1,n=0): %d \n",UART1BuffEmpty);
		if(UART1BuffEmpty ==1){count1=0; }
		
	}
	
	// detecting if something gets stuck on the Rx register (paquets less than 3 bytes) 
		uart1_timeout++;
	if(uart1_timeout >= 100){
		uart1_timeout = 0;
		junk = UA1_RBR;  // reseting the Rx register 
	}
	
} //


/*/////////////////////////////////////////////////////////////////////////////////////////////////////////
//ALTERNATE 
UART1 Interrupt called everytime data is recieved by UART1. It prints data out through UART0
void UART1_IRQHandler(void)
{
 	int UART1BuffCount=0;
	int UART1BuffEmpty=1;
	int i=0;

	printf("\n-------------INTERRUPT TRIPPED-------------\n");
	UART1BuffCount = (((UA1_FSR)<<18)>>26);							//Outputs the number of bytes in the UART1 RX buffer
	printf("Buff Count: %d\t\t",UART1BuffCount);
	RXData[(UART1BuffCount)] = UA1_RBR;													//Reads the oldest byte of data in the buffer. 		  		
	printf("UART1 Data: %d\t\t", RXData[(UART1BuffCount)]);
	UART1BuffEmpty = ((UA1_FSR & RX_EMPTY)>>14);				//Outputs 1 when buffer is empty
	printf("Empty(y=1,n=0): %d\n",UART1BuffEmpty);
}
*/
