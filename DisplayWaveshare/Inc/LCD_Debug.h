/**
 ******************************************************************************
 * @file    LCD_Debug.h
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor für technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    2020-07-29
 * @brief   This module contains some debug code. To reduce code size,
 *          each debug function can be switched on and off via a define.
 *
 ******************************************************************************
 */

#ifndef _LCD_DEBUG_H_
#define _LCD_DEBUG_H_

#include "LCD_general.h"

// Defines, to switch each debug function on and off.
// #define DEBUG_LCD
#define DEBUG_MSG           
#define REDUCDED_DEBUG_MSG   // Reduced debug output message to reduce code size
// #define DEBUG_TOUCH




#ifdef DEBUG_LCD
/**
  * @brief  Coordinates that will be passed to functions
	*         of module LCD_driver will be stored in a ring buffer.
	*         This function resets the ring buffer.
	*         This function should never be called directly, but 
	*         always via macro RESET_DM.
  * @retval None
  */
extern void resetDM(void);

/**
  * @brief  Coordinates that will be passed to functions
	*         of module LCD_driver will be stored in a ring buffer.
	*         This function writes the coordinates of a function
	*         call to the ring buffer.
	*         This function should never be called directly, but 
	*         always via macro COORDINATE_TO_DM.
	* @param  tl Top left coordinate
	* @param  br Bottom right coordinate
	
  * @retval None
  */
extern void addCoordinatesToDM(Coordinate tl, Coordinate br);
#define RESET_DM(X)				     resetDM()
#define COORDINATE_TO_DM(X,Y)  addCoordinatesToDM((X), (Y))

#else
#define RESET_DM(X)  
#define COORDINATE_TO_DM(X,Y)
#endif

#ifdef DEBUG_MSG
/**
  * @brief  This function writes a debug message to LCD display.
	*         This function should never be called directly, but 
	*         always via macro DEBUG.
	* @param  file File name 
	* @param  line Line in file
	* @param  fmt  Format sting corresponding to the one of printf
	* @param  ...  Arguments for format string	
  * @retval None
  */
#ifdef REDUCDED_DEBUG_MSG
extern void printDebugMsg(char *file, int line);
#define DEBUG(__info,...)   printDebugMsg(__FILE__, __LINE__)
#else
extern void printDebugMsg(char *file, int line, const char * fmt, ...);
#define DEBUG(__info,...)   printDebugMsg(__FILE__, __LINE__, __info,##__VA_ARGS__)
#endif
#else
#define DEBUG(__info,...)  
#endif

#ifdef DEBUG_TOUCH
/**
  * @brief  Data read out of the touch pad will be stored in a 
	*         ring buffer. This function writes these data into 
  *         the ring buffer.
	*         This function should never be called directly, but 
	*         always via macro UPDATE_TP_CMD.
	* @param  cmd  Command written to the touch pad.
	* @param  data Return value of the command.
	
  * @retval None
  */
extern void updateTPcmd(uint8_t cmd, uint16_t data);
#define UPDATE_TP_CMD(X,Y)   updateTPcmd((X),(Y))

#else
#define UPDATE_TP_CMD(X,Y)  
#endif

#endif
// EOF
