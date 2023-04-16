/**
 ******************************************************************************
 * @file    main.c
 * @author  Valentina TM
 * @version 0.1
 *
 * @date    17. Juli 2020
 * @brief   Versuch einen Taschenrechner zu bauen
 *
 ******************************************************************************
 */

#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "init.h"
#include "delay.h"
#include "LCD_GUI.h"
#include "LCD_Demos.h"
#include "lcd.h"
#include "fontsFLASH.h"
#include "LCD_Touch.h"
#include "error.h"
#include "keypad.h"
#include "terminal.h"
#include "calculator.h"
#include "input.h"
#include "operator.h"
#include "output.h"
#include "stack.h"
#include <ctype.h>
#include <stdint.h>
#include "errors.h"
#define NOTABLETOUSECALC -8
#define NO_NUMBER -1
#define STACKSTART 0
#define FOREVER 1


#define MAX 5

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */

char get_char(void);

int main(void){
	// Init Board
	initITSboard();                 // Initialisierung des ITS Boards
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch
	if (!checkVersionFlashFonts()) {
	    // Ueberpruefe Version der Fonts im Flash passt nicht zur Software Version
		Error_Handler();
	}
	// Initialize calculator
	Terminal keyPadTerm = makeKeyPad();
  
	
// Initialize stack
	Stack stack;
  stack.size = STACKSTART;
  int current_num = NO_NUMBER;
	
	// start of superloop
	  while (FOREVER) {
			
		// receiving char from Terminal
    char c = get_char();
		get_output(c, &keyPadTerm);
	
			//
    if (isdigit(c)) {
      current_num = save_num(current_num, c);
			continue;
    }

    if (!isdigit(c)) {
      if (NO_NUMBER != current_num) {
				push(&stack, current_num);				
				current_num = NO_NUMBER;
				
			}
        
      int is_error = determine_oparator(&stack, c, &keyPadTerm);
			if (is_error){
				error_output(is_error, &keyPadTerm);
			}
      }
    }
	}