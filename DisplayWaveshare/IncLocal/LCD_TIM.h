/**
 ******************************************************************************
 * @file    LCD_TIM.h
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor für technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    2020-07-16
 * @brief   This module implements the interface to the HW timer used by
 *          the LCD display.
 *
 ******************************************************************************
 */

#ifndef __LCD_TIM_H
#define __LCD_TIM_H

#include <stdint.h>

/**
  * @brief  Initialisation of TIM4 according to the 
	*         requirements of the LCD display.
	*         TIM4 will be used in PWM to drive the 
	*         the brightness of the display.
  * @retval None
  */
extern void TIM4_Init(void);

/**
  * @brief  Setup of PWM for driving brightness of the display.
	* @param  value Pulse width
  * @retval None
  */
extern void PWM_SetValue(uint16_t value);

#endif

// EOF
