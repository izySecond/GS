
/**
  ******************************************************************************
  * @file    	timer.h 
  * @author  	Franz Korf
  * @version  V1.0
  * @date     2020-08-22
  * @brief    Timer for calculation
  ******************************************************************************
  */

#ifndef _TIMER_H
#define _TIMER_H

#include <stdint.h>
#include <stm32f4xx_hal.h>

#define TICKS_PER_US 90 /* Ticks pro Mikrosekunde */

/**
* @brief This function initializes TIM2
* @param None
* @retval None
*/
extern void initTimer(void);

/**
* @brief This function gets the current time stamp.
* @param None
* @retval timerTicks
*/
inline uint32_t getTimeStamp(void){
	return (TIM2->CNT);
}

#endif
// EOF
