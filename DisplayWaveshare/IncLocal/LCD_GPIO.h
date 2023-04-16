/*****************************************************************************
* | File      	:	LCD_GPIO.h
* | Author      : Franz Korf
* | Function    : Interface to GPIOs used by the LCD und Touch
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-07-16
* | Info        :   Basic version
*
******************************************************************************/

/**************************Intermediate driver layer**************************/

#ifndef __LCD_GPIO_H
#define __LCD_GPIO_H

#include "stm32f4xx_hal.h"

//#include <stdbool.h>

// All GPIOs used by LCD and Touch via Arduino Shield

// In case of an error, LED1 and LED2 of Nucleo 144 Board blink.

#define SPI1_SCK_Pin GPIO_PIN_5
#define SPI1_SCK_GPIO_Port GPIOA
#define SPI1_MISO_Pin GPIO_PIN_6
#define SPI1_MISO_GPIO_Port GPIOA
#define SPI1_MOSI_Pin GPIO_PIN_7
#define SPI1_MOSI_GPIO_Port GPIOA

#define TP_BUSY_Pin GPIO_PIN_10
#define TP_BUSY_GPIO_Port GPIOB
#define TP_IRQ_Pin GPIO_PIN_13
#define TP_IRQ_GPIO_Port GPIOE
#define TP_CS_Pin GPIO_PIN_14
#define TP_CS_GPIO_Port GPIOF

#define LCD_BL_Pin GPIO_PIN_15
#define LCD_BL_GPIO_Port GPIOD
#define LCD_DC_Pin GPIO_PIN_13
#define LCD_DC_GPIO_Port GPIOF
#define LCD_RST_Pin GPIO_PIN_12
#define LCD_RST_GPIO_Port GPIOF
#define LCD_CS_Pin GPIO_PIN_14
#define LCD_CS_GPIO_Port GPIOD

// Setup GPIOs 
extern void GPIO_Init(void);

#endif

// EOF
