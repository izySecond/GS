/**
 ******************************************************************************
 * @file    LCD_Tools.h
 *
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
 
#ifndef _LCD_TOOLS_H_
#define _LCD_TOOLS_H_

#include <stdint.h>

/**
  * @brief  This function implements a rough delay in ms range.
	*
	* @param  xms	Delay 
  * @retval None
  */
extern void Driver_Delay_ms(uint32_t xms);

/**
  * @brief  This function implements a rough delay in us range.
	*
	* @param  xms	Delay 
  * @retval None
  */
extern void Driver_Delay_us(uint32_t xus);

/**
  * @brief  A basis error handler that flashes two onboard LEDs.
	*
  * @retval None
  */
extern void LCDErrHandler(char * unused_1, int unused_2);

/**
  * @brief  Standard integer to string conversion
	*
  * @param no Value that should be converted
  * @param buf Buffer to store the generated string.
  * @retval The buffer that contains the generated string
  */
extern char* itoa(int no, char buf[]);



#endif
// EOF
