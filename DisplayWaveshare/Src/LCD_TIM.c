/**
 ******************************************************************************
 * @file    LCD_TIM.c
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

#include "../IncLocal/LCD_TIM.h"
#include "../IncLocal/LCD_Tools.h"
#include "../IncLocal/LCD_GPIO.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>

static void MX_TIM4_Init(void);
static void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle);

void TIM4_Init(void){
	MX_TIM4_Init();
}

static TIM_HandleTypeDef htim4;

static void MX_TIM4_Init(void){
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim4.Instance = TIM4;
  // htim4.Init.Prescaler = 720-1; KORF da f103 un F429 unterschiedliche CLK Trees und Frequenzen F103: 72MHz; F429 180MHz
  htim4.Init.Prescaler = 900-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 1000-1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK){
    LCDErrHandler(__FILE__, __LINE__);
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK){
    LCDErrHandler(__FILE__, __LINE__);
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4) != HAL_OK){
    LCDErrHandler(__FILE__, __LINE__);
  }
  HAL_TIM_MspPostInit(&htim4);
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* tim_pwmHandle){
  if(tim_pwmHandle->Instance==TIM4){
    __HAL_RCC_TIM4_CLK_ENABLE();
  }
}

static void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle){
  GPIO_InitTypeDef GPIO_InitStruct = {0,0,0,0,0};
  if(timHandle->Instance==TIM4) {
    // TIM4 GPIO Configuration  PD15 --> TIM4_CH4 
    GPIO_InitStruct.Pin = LCD_BL_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
    HAL_GPIO_Init(LCD_BL_GPIO_Port, &GPIO_InitStruct);
  }
}

void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* tim_pwmHandle){
  if(tim_pwmHandle->Instance==TIM4) {
    __HAL_RCC_TIM4_CLK_DISABLE();
  }
} 

void PWM_SetValue(uint16_t value) {
	  TIM_OC_InitTypeDef sConfigOC;
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = value;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  	HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);  
}

// EOF
