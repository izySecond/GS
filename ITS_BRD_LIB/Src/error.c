/**
 ******************************************************************************
 * @file    error.c
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor für technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    17. Juli 2020
 * @brief   Einfache Error Handler, der User LED1 und LED2 blinken laesst.
 ******************************************************************************
 */
 
#include "error.h"
 
#include "stm32f4xx_nucleo_144.h"

void Error_Handler(void) {
	 GPIO_InitTypeDef  GPIO_InitStruct = {0,0,0,0,0};

	 // Enable Pin of LED1 and LED2 as output mode 
	 LED1_GPIO_CLK_ENABLE();
	 LED2_GPIO_CLK_ENABLE();
	 
	 GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	 GPIO_InitStruct.Pull  = GPIO_PULLUP;
	 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	 GPIO_InitStruct.Pin = LED1_PIN;
	 HAL_GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStruct);
 	 GPIO_InitStruct.Pin = LED2_PIN;
	 HAL_GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);
   
	 while (1) {
		 const int blinkDelay = 2 * 1000 * 1000;
		 HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
		 for (int i = 0; i <= blinkDelay; i++); // wait some time due to blinking
		 HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
		 for (int i = 0; i <= blinkDelay; i++); // wait some time due to blinking
	 }
}
 
 
 // EOF
 


