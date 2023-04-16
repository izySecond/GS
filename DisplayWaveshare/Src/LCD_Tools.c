/**
 ******************************************************************************
 * @file    LCD_Tools.c
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor für technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    2020-07-16
 * @brief   This module contains basic helper functions.
 *
 ******************************************************************************
 */

#include "../IncLocal/LCD_Tools.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h"

void Driver_Delay_ms(uint32_t xms){
    HAL_Delay(xms);
}

void Driver_Delay_us(uint32_t xus) {
	int j;
  for(j=xus; j > 0; j--);
}

void LCDErrHandler(char * file, int line){
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
		 const int blinkDelay = 4 * 1000 * 1000;
		 HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
		 Driver_Delay_us(blinkDelay); // wait some time due to blinking
		 HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
		 Driver_Delay_us(blinkDelay); // wait some time due to blinking
	 }
}

char* itoa(int no, char buf[]){
	 int64_t number = no; // due to asymmetrical range of values
	 char *pos = buf;
	 if (number < 0) {
		  *pos++ = '-';
		   number = number * (-1);
	 }
	 int shifter = number;
   do{ // gehe an das Endes des strings
      ++pos;
      shifter = shifter/10;
   } while(shifter);
   *pos = '\0';
	 do{ // drucke die Zahl von hinten
      *--pos = '0' + number%10;
      number = number/10;
   } while(number);	
	 return buf;
}

// EOF
