/**
 ******************************************************************************
 * @file    LCD_Demos.h
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor fuer technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 *
 * @date    2020-08-03
 * @brief   This module contains the demos of Waveshare.
 *
 ******************************************************************************
 */

#ifndef __LCD_DEMOS_H
#define __LCD_DEMOS_H

/**
  * @brief This function displays the welcome picture of Waveshare.
  *
  * @retval none
  */
void GUI_show(void);

/**
  * @brief  Display draw board on LCD.
  * @retval None
  */
extern void TP_Dialog(void);

/**
  * @brief  Update of the draw board.
  * @retval None
  */
extern void TP_DrawBoard(void);

#endif

// EOF
