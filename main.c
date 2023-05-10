#include "stm32f0xx.h"

#define DELAY1 1000000
#define DELAY2 1000000

typedef uint8_t flag_t;

#define TRUE 1
#define FALSE 0

volatile uint32_t delayCount;

flag_t startFlag = FALSE;
flag_t firstFlag = FALSE;
flag_t secondFlag = FALSE;
flag_t thirdFlag = FALSE;

void initGPIO(void) {
    // Enable GPIOA and GPIOB peripheral clocks
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;

    // Configure LED pins (B0-B7) as outputs
    GPIOB->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 |
                    GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0 |
                    GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 |
                    GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0;

    // Configure push button pins (A0-A3) as inputs
    GPIOA->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 |
                      GPIO_MODER_MODER2 | GPIO_MODER_MODER3);

    // Enable pull-up resistors for push buttons
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_0 | GPIO_PUPDR_PUPDR1_0 |
                    GPIO_PUPDR_PUPDR2_0 | GPIO_PUPDR_PUPDR3_0;
}

void delay(void) {
    for (delayCount = 0; delayCount < DELAY1; delayCount++) {
        for (uint32_t i = 0; i < DELAY2; i++) {
            // Delay loop
        }
    }
}

void firstDisplay(void) {
    while (startFlag == TRUE) {
        GPIOB->ODR = 0xAA;  // Display bitpattern1
        delay();

        GPIOB->ODR = 0x55;  // Display bitpattern2
        delay();

        GPIOB->ODR = 0xFF;  // Display bitpattern3
        delay();
    }
}

void secondDisplay(void) {
    while (startFlag == TRUE) {
        GPIOB->ODR = 0x01;  // Display Step 1
        delay();

        GPIOB->ODR = 0x02;  // Display Step 2
        delay();

        GPIOB->ODR = 0x04;  // Display Step 3
        delay();

        GPIOB->ODR = 0x08;  // Display Step 4
        delay();

        GPIOB->ODR = 0x10;  // Display Step 5
        delay();

        GPIOB->ODR = 0x20;  // Display Step 6
        delay();

        GPIOB->ODR = 0x40;  // Display Step 7
        delay();

        GPIOB->ODR = 0x80;  // Display Step 8
        delay();
    }
}

void thirdDisplay(void) {
    uint8_t counter = 0;

    while (startFlag == TRUE) {
        GPIOB->ODR = counter;  // Display counter value
        delay();

        counter++;

        if (counter == 0) {
            // Reset the counter if it reaches its maximum value
            counter = 0;
        }
    }
}
void checkPB(void) {
    // Check the status of SW0, SW1, SW2, and SW3 buttons
    // Update the corresponding flags (startFlag, firstFlag, secondFlag, thirdFlag) accordingly

    // Assuming GPIOA pins are used for the push buttons
    uint8_t sw0 = GPIOA->IDR & GPIO_IDR_IDR_0_Msk; // Read SW0 status
    uint8_t sw1 = GPIOA->IDR & GPIO_IDR_IDR_1_Msk; // Read SW1 status
    uint8_t sw2 = GPIOA->IDR & GPIO_IDR_IDR_2_Msk; // Read SW2 status
    uint8_t sw3 = GPIOA->IDR & GPIO_IDR_IDR_3_Msk; // Read SW3 status

    // Update the flags based on the button status
    startFlag = (sw0 == 0) ? TRUE : FALSE;
    firstFlag = (sw1 == 0) ? TRUE : FALSE;
    secondFlag = (sw2 == 0) ? TRUE : FALSE;
    thirdFlag = (sw3 == 0) ? TRUE : FALSE;
}


void display(void) {
    if (startFlag == TRUE) {
        // Call the appropriate display function based on the active flag
        if (firstFlag == TRUE) {
            firstDisplay();
        }
        else if (secondFlag == TRUE) {
            secondDisplay();
        }
        else if (thirdFlag == TRUE) {
            thirdDisplay();
        }
        else {
            // No active flag, do nothing or handle the default case
        }
    }
    else {
        // startFlag is FALSE, do nothing or handle the case when the display should be turned off
    }
}


int main(void) {
    initGPIO();  // Initialize GPIO

    // Main loop
    while (1) {
        // Check if SW0, SW1, SW2, or SW3 is pressed
        checkPB();

        // Display the appropriate LED pattern based on the flags
        display();
    }

    return 0;
}
