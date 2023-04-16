/*****************************************************************************
* | File      	:	LCD_Driver.h
* | Author      : Franz Korf
* | Function    : Definition of LCD_Driver.c that are only visible in this project.
* | Info        :
*   Image scanning:
*      Please use progressive scanning to generate images or fonts
*----------------
* |	This version:   V1.0
* | Date        :   2020-07-16
* | Info        :   Basic version
*
******************************************************************************/

/**************************Intermediate driver layer**************************/

#ifndef __LCD_DRIVER_H
#define __LCD_DRIVER_H

#include "LCD_general.h"
#include <stdbool.h>

/********************************************************************************
Different Scanning modes
********************************************************************************/	
#define L2R_U2D       0	//The display interface is displayed , left to right, up to down
#define L2R_D2U       1
#define R2L_U2D       2
#define R2L_D2U       3
#define U2D_L2R       4
#define U2D_R2L       5
#define D2U_L2R       6
#define D2U_R2L       7

#define CURRENT_SCAN_DIR D2U_L2R  // Screen scan direction used in this project

/**
  * @brief  This function initializes the LCD display. Please note that
  *         it does not initialize the touch functionality.
	* @param  Brightness of the display
  * @retval None
  */
void LCD_init(uint16_t LCD_BLval);

/**
  * @brief  This function fills a rectangle. If the rectangle extends 
  *         beyond the edges of the display, protruding pixels are 
	*         not shown.
	*
	* @param  tl     Top left pixel of the rectangle. It must be a valid 
	*                coordinate.
	* @param  width  Width of the rectangle
	* @param  height Height of the rectangle
	* @param  color   Color of the rectangle
  * @retval None
  */
void LCD_fillRectangleMC (Coordinate tl, LENGTH width,	LENGTH height, COLOR color);

/**
  * @brief  This function fills a rectangle according to a bit map. The bit
	*         map is given in the format of STM32 fonts.
	*         If the rectangle extends beyond the edges of the display, protruding
	*         pixels are not shown.
	*
	* @param  tl     Top left pixel of the rectangle. It must be a valid 
	*                coordinate.
	* @param  width  Width of the rectangle
	* @param  height Height of the rectangle
	* @param  fg     Foreground color
	* @param  bg     Background color
	* @param  table  Bit map
  * @retval None
  */
void LCD_fillRectangleBW (Coordinate tl, LENGTH width,	LENGTH height, COLOR bg, COLOR fg, const uint8_t *table);

/**
  * @brief  This function fills the entire display with one color.
	*
	* @param  color  color
	* @param  bg     Fill color
  * @retval None
  */
void LCD_clear(COLOR  Color);

/**
  * @brief  This function returns true if the display operates in landscape mode.

  * @retval True for landscape mode, otherwise false.
  */
extern bool LCD_isHorizontalMode(void);

#endif

// EOF
