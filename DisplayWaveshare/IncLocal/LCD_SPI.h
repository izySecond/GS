/**
 ******************************************************************************
 * @file    LCD_SPI.h
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor für technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    2020-07-16
 * @brief   This module implements the interface to the SPI bus used by
 *          the LCD display.
 *
 ******************************************************************************
 */

#ifndef __LCD_SPI_H
#define __LCD_SPI_H

#include <stdint.h>
/**
  * @brief  Initialisation of the SPI bus used by the LCD.
  * @retval None
  */							
void SPIinit(void);

/**
  * @brief  Sets the speed of the SPI bus to the one of 
	*         the touch pad chip
  * @retval None
  */	 
void SPIsetTPSpeed(void);

/**
  * @brief  Sets the speed of the SPI bus to the one of 
	*         the LCD chip
  * @retval None
  */
void SPIsetLCDSpeed(void);

/**
  * @brief  This function writes one byte to the SPI bus.
	* @param  value  Byte to be written 
  * @retval None
  */
uint8_t SPI4W_Write_Byte(uint8_t value);

/**
  * @brief  This function reads one byte from the SPI bus.
  * @retval Byte read
  */
uint8_t SPI4W_Read_Byte(void);

#endif

// EOF
