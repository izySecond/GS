/**
 ******************************************************************************
 * @file    LCD_general.h
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor für technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    2020-08-03
 * @brief   Types, constants and basis checks for LCD display
 *
 ******************************************************************************
 */

#ifndef __LCD_GENERAL_H
#define __LCD_GENERAL_H

#include <stdint.h>

/********************************************************************************
function:
		Define the full screen height length of the display
********************************************************************************/
#define LCD_X_MAXPIXEL  480  //LCD width maximum memory 
#define LCD_Y_MAXPIXEL  320  //LCD height maximum memory
#define LCD_X	 0
#define LCD_Y	 0

#define LCD_WIDTH  (LCD_X_MAXPIXEL - 2 * LCD_X)  //LCD width
#define LCD_HEIGHT (LCD_Y_MAXPIXEL - 2 * LCD_Y)  //LCD height

/********************************************************************************
function:
		Basic types 
********************************************************************************/

#define	COLOR					uint16_t		//The variable type of the color (unsigned short) 
#define	POINT					uint16_t		//The type of coordinate
#define	LENGTH				uint16_t		//The type of coordinate

#define INVALID_POINT	0xFFFF

typedef struct {
	POINT x;
	POINT y;
} Coordinate;

/********************************************************************************
function:
		Basic check of a Coordinate
********************************************************************************/

#define VALID_COORDINATE(c) (((c).x < LCD_WIDTH) && ((c).y < LCD_HEIGHT))

#endif

// EOF
