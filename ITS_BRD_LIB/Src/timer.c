/**
  ******************************************************************************
  * @file    	timer.c 
  * @author  	Franz Korf
  * @version  V1.0
  * @date     2020-08-22
  * @brief    Timer related functions
	*           Adapted to discovery board
  ******************************************************************************
  */
#include "timer.h"

void initTimer(void){
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; /* Takt fuer Timer 2 einschalten */
    TIM2->CR1 = 0;                      /* Timer disabled                */
    TIM2->CR2 = 0;                      /*                               */
    TIM2->PSC = 0;                      /* Prescaler   (180MHz)          */
    TIM2->ARR = 0xffffffff;             /* Auto reload register          */
    TIM2->DIER  = 0;                    /* Interrupt ausschalten         */
    TIM2->CR1 = TIM_CR1_CEN;            /* Enable Timer                  */
}

// EOF
