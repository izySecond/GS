/**
 ******************************************************************************
 * @file    LCD_Debug.c
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
 
#include "LCD_Debug.h"
#include <stdio.h>
#include <stdarg.h>
#include "LCD_GUI.h"
#include "fontsFLASH.h"

#define POINT_MEM_SIZE     1000
#define TP_MEM_SIZE         100
#define SIZE_ERR_MSG				200

#ifdef DEBUG_LCD
struct {
	 Coordinate topLeft;
	 Coordinate bottomRight;
} pointDM[POINT_MEM_SIZE];

int nextFreePosDM = 0;

void resetDM(void){
	 for (int i = 0; i < POINT_MEM_SIZE; i++) {
		 pointDM[i].topLeft.x     = 0xFFFF;
		 pointDM[i].topLeft.y     = 0xFFFF;
		 pointDM[i].bottomRight.x = 0xFFFF;
		 pointDM[i].bottomRight.y = 0xFFFF;
	 }
	 nextFreePosDM = 0;
}

void addCoordinatesToDM(Coordinate tl, Coordinate br){
 	 pointDM[nextFreePosDM].topLeft = tl;
	 pointDM[nextFreePosDM].bottomRight = br;
	 nextFreePosDM = (nextFreePosDM + 1) % POINT_MEM_SIZE;
}
#endif

#ifdef DEBUG_MSG
#ifdef REDUCDED_DEBUG_MSG
void printDebugMsg(char *file, int line) {
	 Coordinate tl = {10,10};
	 GUI_disStr(tl, "Debug message in file" , &Font16FLASH, WHITE, RED);	
	 tl.y = tl.y + 3 * Font16FLASH.Height;
	 GUI_disStr(tl, file , &Font16FLASH, WHITE, RED);	
	 tl.y = tl.y + 3 * Font16FLASH.Height;
	 GUI_disStr(tl, "line " , &Font16FLASH, WHITE, RED);	
	 tl.x = tl.x + 5 * Font16FLASH.Width;
	 GUI_disNum(tl, line , &Font16FLASH, WHITE, RED);		
}
#else
static char errMsg[SIZE_ERR_MSG];

void printDebugMsg(char *file, int line, const char * fmt, ...) {
	 Coordinate tl = {10,10};
	 snprintf(errMsg, sizeof(errMsg), "Debug %s line %d:", file, line);
	 GUI_disStr(tl, errMsg, &Font16FLASH, WHITE, RED);	
	
	 tl.y = 4 * Font16FLASH.Height;

   va_list arg_ptr;
   va_start (arg_ptr, fmt); /* format string */	
	 vsnprintf(errMsg, sizeof(errMsg), fmt, arg_ptr);
   va_end (arg_ptr);
	
	 GUI_disStr(tl, errMsg, &Font16FLASH, WHITE, RED);	
}
#endif
#endif

#ifdef DEBUG_TOUCH
unsigned int TPListPos = 0;
uint32_t TPCmds[TP_MEM_SIZE];

void updateTPcmd(uint8_t cmd, uint16_t data) {
	 uint32_t v = cmd;
	 v = v << (3*8);
	 v = v | (uint32_t) data;
	 TPCmds[TPListPos] = v;
	 TPListPos = (TPListPos + 1) % TP_MEM_SIZE;
}
#endif

// EOF
