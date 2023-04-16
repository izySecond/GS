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
#include "input.h"

int save_num(int current_int, char c) {
	
	if (current_int == -1) {
		current_int = c - '0';
	} else {
		current_int = (current_int * 10) + (c-'0');
  }
  return current_int;
}
	
char get_char(void){
	char input_char = getKeyPadInput();
	return input_char;
	}