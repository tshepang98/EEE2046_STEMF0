//********************************************************************
//*                    EEE2046F C template                           *
//*==================================================================*
//* WRITTEN BY: Tshepang Mathlore 	                 		                         *
//* DATE CREATED: 10/05/2023                                                   *
//* MODIFIED:                                                        *
//*==================================================================*
//* PROGRAMMED IN: Atollic True Studio                               *
//* TARGET:        STM32F0                                           *
//*==================================================================*
//* DESCRIPTION:                                                     *
//*                                                                  *
//********************************************************************
// INCLUDE FILES
//====================================================================
#include "stm32f0xx.h"
#include <stdint.h>
//====================================================================
// GLOBAL CONSTANTS
//====================================================================
#define DELAY1 0
#define DELAY2 500000
#define TRUE 1
#define FALSE 0
 
 
//====================================================================
// GLOBAL VARIABLES
//====================================================================
typedef uint8_t flag_t;
uint16_t bitpattern1 = 0;		// Sets the ODR to turn of LEDs
uint16_t bitpattern2 = 65535;	// set the first 8 bits of ODR to 11111111 which will display this pattern when called
uint16_t bitpattern3 = 170;     // set the first 8 bits of ODR to 10101010 which will display this pattern when called
flag_t startFlag = FALSE;
flag_t firstFlag = FALSE;
flag_t secondFlag = FALSE;
flag_t thirdFlag = FALSE;
 
uint16_t readInput;
//====================================================================
// FUNCTION DECLARATIONS
//====================================================================
void initGPIO(void);
void delay(void);
void firstDisplay(void);
void secondDisplay(void);
void thirdDisplay(void);
void checkPB(void);
void display(void);
//====================================================================
// MAIN FUNCTION
//====================================================================
int main (void)
{
	for(;;){
 
		initGPIO(); //enable registers
		display();
	}
}							// End of main
 
//====================================================================
// FUNCTION DEFINITIONS
//====================================================================
 
void initGPIO(void){
	RCC->AHBENR = 0x00000014;		// Reset the AHBENR before updating it
	//GPIOA->MODER = 0;
	RCC->AHBENR |= ((uint32_t)(1<<17) | (uint32_t)(1<<18));		// Connects the clock to PORT A and B
	GPIOA->MODER &=~ (GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2 | GPIO_MODER_MODER3); // Set PORT(0-4) to Output Mode
	GPIOA->PUPDR |= 0x00000055;		// Enable Pull Up registers for PORT A
	GPIOB->MODER |= 0x00005555;		// set Push Buttons to digital output PORTB
	GPIOB->ODR = 0;                 // Disable all LEDs
	readInput = GPIOA->IDR;
	GPIOA->IDR = 0;
}
 
void delay(void){
	for(int i = 0;i < DELAY1;i++);
	for(int j = 0;j < DELAY2;j++);
}
 
void firstDisplay(void){
 
		GPIOB->ODR = bitpattern1;
		delay();
		GPIOB->ODR = bitpattern2;
		delay();
		GPIOB->ODR = bitpattern3;
		delay();
}
 
void secondDisplay(void){
		for(int i = 0; i < 8; ++i){
			GPIOB->ODR = 0; 	// Reset the ODR register
			//delay();
			switch(i){
			case 0:
				checkPB();
				if (!secondFlag) break;
				GPIOB->ODR |= GPIO_ODR_0;
				delay();
				break;
			case 1:
				checkPB();
				if (!secondFlag) break;
				GPIOB->ODR |= GPIO_ODR_1;
				delay();
				break;
			case 2:
				checkPB();
				if (!secondFlag) break;
				GPIOB->ODR |= GPIO_ODR_2;
				delay();
				break;
			case 3:
				checkPB();
				if (!secondFlag) break;
				GPIOB->ODR |= GPIO_ODR_3;
				delay();
				break;
			case 4:
				checkPB();
				if (!secondFlag) break;
				GPIOB->ODR |= GPIO_ODR_4;
				delay();
				break;
			case 5:
				checkPB();
				if (!secondFlag) break;
				GPIOB->ODR |= GPIO_ODR_5;
				delay();
				break;
			case 6:
				checkPB();
				if (!secondFlag) break;
				GPIOB->ODR |= GPIO_ODR_6;
				delay();
				break;
			case 7:
				checkPB();
				if (!secondFlag) break;
				GPIOB->ODR |= GPIO_ODR_7;
				delay();
				break;
			}
		}
}
 
void thirdDisplay(void){
	int bits = 0;        // Hold the current bit-counter
	GPIOB->ODR = 0;		// initially all the LED are off
	while(thirdFlag){
		delay();
		GPIOB->ODR += 1;		// The counter must start counting up from 0 and should keep counting up in increments of 1 until the maximum value is displayed on the LEDS
		if (bits == 255){		// the maximum value for 8 LEDs
			bits = 0;			// reset the counter
			continue;
		}
		bits++;
		checkPB();
	}
}
 
void checkPB(void)
{
 
    if ((GPIOA -> IDR & GPIO_IDR_0) == 0)     // is SW0 pressed?
    {
        startFlag = TRUE;
        firstFlag = FALSE;
        secondFlag = FALSE;
        thirdFlag = FALSE;
    }
 
    else if (( GPIOA -> IDR & GPIO_IDR_1) == 0) // is SW1 pressed?
    {
        startFlag = TRUE;
        firstFlag = TRUE;
        secondFlag = FALSE;
        thirdFlag = FALSE;
    }
 
    else if (( GPIOA -> IDR & GPIO_IDR_2) == 0) // is SW2 pressed?
    {
        startFlag = TRUE;
        firstFlag = FALSE;
        secondFlag = TRUE;
        thirdFlag = FALSE;
    }
 
    else if (( GPIOA -> IDR & GPIO_IDR_3) == 0) // is SW3 pressed?
    {
        startFlag = TRUE;
        firstFlag = FALSE;
        secondFlag = FALSE;
        thirdFlag = TRUE;
    }
 
}
 
void display(void){
	checkPB();
	if (startFlag && !firstFlag && !secondFlag && !thirdFlag){
		GPIOB->ODR |= GPIO_ODR_0;
		delay();
	}
	if(firstFlag){
		firstDisplay();
	}if(secondFlag){
		secondDisplay();
	}if(thirdFlag){
		thirdDisplay();
	}
 
}
 
 
 
//********************************************************************
// END OF PROGRAM
//********************************************************************