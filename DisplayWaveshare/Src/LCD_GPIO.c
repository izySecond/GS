/*****************************************************************************
* | File      	:	LCD_GPIO.c
* | Author      : Franz Korf
* | Function    : Interface to GPIOs used by the LCD und Touch
* | Info        :
*   
*----------------
* |	This version:   V1.0
* | Date        :   2020-07-16
* | Info        :   Basic version
*
******************************************************************************/

/**************************Intermediate driver layer**************************/

#include "../IncLocal/LCD_GPIO.h"

static void MX_GPIO_Init(void);

void GPIO_Init(void){
	MX_GPIO_Init();
}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
		using HAL Interface
*/

void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0,0,0,0,0};
	
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();   
  __HAL_RCC_GPIOE_CLK_ENABLE();

  HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TP_CS_GPIO_Port, TP_CS_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Pin = TP_BUSY_Pin;
  HAL_GPIO_Init(TP_BUSY_GPIO_Port, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = TP_IRQ_Pin;
  HAL_GPIO_Init(TP_IRQ_GPIO_Port, &GPIO_InitStruct);
	
  /*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Pin = LCD_DC_Pin;
  HAL_GPIO_Init(LCD_DC_GPIO_Port, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = LCD_RST_Pin;
  HAL_GPIO_Init(LCD_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Pin = TP_CS_Pin;
  HAL_GPIO_Init(TP_CS_GPIO_Port, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = LCD_CS_Pin;
  HAL_GPIO_Init(LCD_CS_GPIO_Port, &GPIO_InitStruct);
}

// EOF
