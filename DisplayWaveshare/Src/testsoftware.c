/**
 ******************************************************************************
 * @file    testsoftware.c
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor für technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    2020-08-08
 * @brief   This module contains some testcode for the LCD display.
 *
 ******************************************************************************
 */
 
#define TEST_LED_PORT   GPIOC
#define TEST_LED_PIN    GPIO_PIN_0

#include "testsoftware.h"
#include "stm32f4xx_hal.h"

#include "LCD_Debug.h"
#include "LCD_GUI.h"
#include "fontsFLASH.h"

void setTestLedOn(void){
	HAL_GPIO_WritePin(TEST_LED_PORT, TEST_LED_PIN, GPIO_PIN_SET);
}

void setTestLedOff(void){
	HAL_GPIO_WritePin(TEST_LED_PORT, TEST_LED_PIN, GPIO_PIN_RESET);
}

void toogleTestLed(void){
	if (TEST_LED_PIN == (TEST_LED_PORT->IDR & TEST_LED_PIN)) {
	   HAL_GPIO_WritePin(TEST_LED_PORT, TEST_LED_PIN, GPIO_PIN_RESET);
	} else {
		 HAL_GPIO_WritePin(TEST_LED_PORT, TEST_LED_PIN, GPIO_PIN_SET);
	}
}

void initTestLed(void){
	 __HAL_RCC_GPIOC_CLK_ENABLE();
   GPIO_InitTypeDef GPIO_InitStruct = {0,0,0,0,0};
   GPIO_InitStruct.Pull = GPIO_NOPULL;
	 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   GPIO_InitStruct.Pin = TEST_LED_PIN;
   // HAL_GPIO_Init(TEST_LED_PORT, &GPIO_InitStruct); 	 
   HAL_GPIO_Init(TEST_LED_PORT, &GPIO_InitStruct); 	 
	 setTestLedOff();
}

static void drawDotsOnTheEdge(void){
   Coordinate testPoint = {0,160};
   GUI_drawPoint(testPoint, RED,DOT_PIXEL_8X8,DOT_FILL_RIGHTUP);
   testPoint.x = LCD_WIDTH - DOT_PIXEL_8X8;
   GUI_drawPoint(testPoint, RED,DOT_PIXEL_8X8,DOT_FILL_RIGHTUP);
   testPoint.x = 200; testPoint.y = 0;
   GUI_drawPoint(testPoint, RED,DOT_PIXEL_8X8,DOT_FILL_RIGHTUP);
   testPoint.y = LCD_HEIGHT - DOT_PIXEL_8X8;
   GUI_drawPoint(testPoint, RED,DOT_PIXEL_8X8,DOT_FILL_RIGHTUP);
}

void testDrawRectangle(void){
	 sFONT * font = &Font16FLASH;
	 Coordinate left,  right;
	
	 // rectangles full screen
	 GUI_clear(WHITE);
	 left.x = 10; left.y = 10;
	 GUI_disStr(left, "Rectangles of screen size ", font, WHITE, BLACK);
	 left.y = 30;
	 GUI_disStr(left, "Size of red dots 8X8 pixel", font, WHITE, BLACK);
	 left.x = 0; left.y = 0;
	 right.x = LCD_WIDTH - 1; right.y = LCD_HEIGHT - 1;
	 HAL_Delay(3000);
	
	 // solid rectangle
	 GUI_clear(WHITE);
	 RESET_DM();
	 GUI_drawRectangle(left, right, GREEN, true, DOT_PIXEL_4X4);
	 HAL_Delay(2000);
	
	 // Rectangles with a border
	 for(DOT_PIXEL pix = DOT_PIXEL_1X1 ; pix <= DOT_PIXEL_8X8 ; pix++) {
		 	GUI_clear(WHITE);
      drawDotsOnTheEdge();
		  RESET_DM();
	    GUI_drawRectangle(left, right, GREEN, false, pix);
		  HAL_Delay(2000);
      drawDotsOnTheEdge();		  
	    HAL_Delay(2000);  
	 }
	 
	 // filled rectangles & rectangles with a border
	 left.x  = 40;  left.y  = 40;
	 right.x = 390; right.y = 280;
	 for(char filled = 0; filled < 2; filled++) {	  
		  for(DOT_PIXEL pix = DOT_PIXEL_1X1 ; pix <= DOT_PIXEL_8X8 ; pix++) {
			   GUI_clear(WHITE);
				 GUI_drawPoint(left, RED, pix, DOT_FILL_RIGHTUP);
				 Coordinate testPoint = {right.x - pix + 1, right.y - pix + 1};
 				 GUI_drawPoint(testPoint, RED, pix, DOT_FILL_RIGHTUP);
         HAL_Delay(2000);	
		     RESET_DM();
	       GUI_drawRectangle(left, right, GREEN, filled, pix);
		     HAL_Delay(2000);
				 GUI_drawPoint(left, RED, pix, DOT_FILL_RIGHTUP);
 				 GUI_drawPoint(testPoint, RED, pix, DOT_FILL_RIGHTUP);
         HAL_Delay(2000);
			}
	 }	
}

void testDrawLines(void){
	 sFONT * font = &Font16FLASH;
	 Coordinate left,  right;
	
	 // Dot as line
	 // The line must cover the point exactly and vice versa. So no red or black border.
	 GUI_clear(WHITE);
	 left.x = 10; left.y = 10;
	 GUI_disStr(left, "Test of lines that are one point in size", font, WHITE, BLACK);
	 left.y = 30;
	 GUI_disStr(left, "Black line must cover red point exactly", font, WHITE, BLACK);
   left.y = 50;
	 GUI_disStr(left, "and vice versa. So no red or black edge.", font, WHITE, BLACK);
	
	 left.x = 20; left.y =100;
	 for(DOT_PIXEL pix = DOT_PIXEL_1X1 ; pix <= DOT_PIXEL_8X8 ; pix++) {
		  right = left;
		  RESET_DM();
		  GUI_drawLine(left, right, BLACK, WHITE, (pix % 2) == 0,  pix);
		  HAL_Delay(2000);
		  GUI_drawPoint(left, RED, pix, DOT_FILL_AROUND);
		  HAL_Delay(2000);
		  GUI_drawLine(left, right, BLACK, WHITE, (pix % 2) == 0,  pix); 
      left.x  += 25;
	 }
	 
	 for (char solid = 0; solid < 2; solid++) {
	    // horizontal line solid and dotted
	    GUI_clear(WHITE);
	    left.x = 10; left.y = 10;
	    GUI_disStr(left, "Test of horizontal and vertical lines.", font, WHITE, BLACK);
	    left.y = 30;
	    GUI_disStr(left, "Red dots mark the end points of a line.", font, WHITE, BLACK);
      left.x  = 50;  left.y  = 50;
      right.x = 300; right.y = 50;

	    for(DOT_PIXEL pix = DOT_PIXEL_1X1 ; pix <= DOT_PIXEL_8X8 ; pix++) {
		     RESET_DM();
		     GUI_drawLine(left, right, BLACK, WHITE, solid,  pix);
		     HAL_Delay(2000);
		     GUI_drawPoint(left, RED, pix, DOT_FILL_AROUND);
		     GUI_drawPoint(right, RED, pix, DOT_FILL_AROUND);
		     HAL_Delay(2000);
		     GUI_drawLine(left, right, BLACK, WHITE, solid,  pix);
         left.y  += 20;
         right.y += 20;
	    }
      // vertical line solid and dotted
	    GUI_clear(WHITE);
	    left.x =  10; left.y =  10;
      right.x = 10; right.y = 300;
	    for(DOT_PIXEL pix = DOT_PIXEL_1X1 ; pix <= DOT_PIXEL_8X8 ; pix++) {
		     RESET_DM();
		     GUI_drawLine(left, right, BLACK, WHITE, solid,  pix);
		     HAL_Delay(2000);
		     GUI_drawPoint(left, RED, pix, DOT_FILL_AROUND);
		     GUI_drawPoint(right, RED, pix, DOT_FILL_AROUND);
	  	   HAL_Delay(2000);
		     GUI_drawLine(left, right, BLACK, WHITE, solid,  pix);
         left.x  += 30;
         right.x += 30;
	    }	 
   }
	 
	 // Test diagonals
	 GUI_clear(WHITE);
	 left.x = 10; left.y = 10;
	 GUI_disStr(left, "Test of diagonals.", font, WHITE, BLACK);
	 left.y = 30;
	 GUI_disStr(left, "Red dots mark the end points of a line.", font, WHITE, BLACK);
	 left.y = 50;
   GUI_disStr(left, "1st diagonal form left to right.", font, WHITE, BLACK);
	 HAL_Delay(3000);

   // solid lines only
	 for(DOT_PIXEL pix = DOT_PIXEL_1X1 ; pix <= DOT_PIXEL_8X8 ; pix++) {
		  GUI_clear(WHITE);
		  RESET_DM();
		  left.x  = 0;   left.y  = 0;
      right.x = 479; right.y = 319;
		  GUI_drawLine(left, right, BLACK, WHITE, true,  pix);
		  HAL_Delay(2000);
		  GUI_drawPoint(left, RED, pix, DOT_FILL_AROUND);
		  GUI_drawPoint(right, RED, pix, DOT_FILL_AROUND);
		  HAL_Delay(2000);
		  GUI_drawLine(right, left, GREEN, WHITE, true,  pix);
		  HAL_Delay(2000);
		  GUI_drawLine(left, right, BLACK, WHITE, true,  pix);
		  HAL_Delay(2000);
      left.x  = 479; left.y  = 0;
      right.x = 0;   right.y = 319;
		  GUI_drawLine(left, right, BLACK, WHITE, true,  pix);
		  HAL_Delay(2000);
		  GUI_drawPoint(left, RED, pix, DOT_FILL_AROUND);
		  GUI_drawPoint(right, RED, pix, DOT_FILL_AROUND);
    	HAL_Delay(2000);
		  GUI_drawLine(right, left, GREEN, WHITE, true,  pix);
		  HAL_Delay(2000);
		  GUI_drawLine(left, right, BLACK, WHITE, true,  pix);
		  HAL_Delay(2000);
	 }
	 
	 // Test different slopes
	 GUI_clear(WHITE);
	 left.x = 40; left.y = 10;
	 GUI_disStr(left, "Test of different slopes.", font, WHITE, BLACK);
	 HAL_Delay(3000); 
   // solid lines only
	 for(DOT_PIXEL pix = DOT_PIXEL_1X1 ; pix <= DOT_PIXEL_8X8 ; pix++) {
		  for(char deleteLine = 0; deleteLine < 2; deleteLine++){
		     left.x  = 40;  left.y  = 300;
	       right.x = 400; right.y = 300;		 
		     GUI_clear(WHITE);
		     RESET_DM();
		     while(right.x >= 40){
			      GUI_drawLine(left, right, BLACK, WHITE, true,  pix);
		        HAL_Delay(1000);
					  if (deleteLine) {
							 GUI_drawLine(left, right, WHITE, BLACK, true,  pix);
						}
				    if(right.y > 30){
					     right.y -= 30;
			      } else {
				       right.x -= 40;
			      }
			   }
		  }
	 }

	 // horizontal line test boarder top
	 left.x  = 2;   left.y  = 2;
   right.x = 478; right.y = 2;
	 for(DOT_PIXEL pix = DOT_PIXEL_1X1 ; pix <= DOT_PIXEL_8X8 ; pix++) {
		  GUI_clear(WHITE);
		  RESET_DM();
		  GUI_drawLine(left, right, GREEN, WHITE, true,  pix);
		  HAL_Delay(2000);
      GUI_clear(WHITE);
	    RESET_DM();
		  GUI_drawLine(left, right, GREEN, WHITE, false, pix);
		  HAL_Delay(2000); 
	 }	
	 
	 // horizontal line test boarder bottom
	 left.x  = 2;   left.y  = 318;
   right.x = 478; right.y = 318;
	 for(DOT_PIXEL pix = DOT_PIXEL_1X1 ; pix <= DOT_PIXEL_8X8 ; pix++) {
		  GUI_clear(WHITE);
		  RESET_DM();
		  GUI_drawLine(left, right, GREEN, WHITE, true,  pix);
		  HAL_Delay(2000);
      GUI_clear(WHITE);
	    RESET_DM();
		  GUI_drawLine(left, right, GREEN, WHITE, false, pix);
		  HAL_Delay(2000); 
	 }	 	 

	 // vertical line test boarder left
	 left.x  = 2;   left.y  = 2;
   right.x = 2; right.y = 318;
	 for(DOT_PIXEL pix = DOT_PIXEL_1X1 ; pix <= DOT_PIXEL_8X8 ; pix++) {
		  GUI_clear(WHITE);
		  RESET_DM();
		  GUI_drawLine(left, right, GREEN, WHITE, true,  pix);
		  HAL_Delay(2000);
      GUI_clear(WHITE);
	    RESET_DM();
		  GUI_drawLine(left, right, GREEN, WHITE, false, pix);
		  HAL_Delay(2000); 
	 }		 
	
	 // vertical line test boarder right
	 left.x  = 476; left.y  = 2;
   right.x = 476; right.y = 318;
	 for(DOT_PIXEL pix = DOT_PIXEL_1X1 ; pix <= DOT_PIXEL_8X8 ; pix++) {
		  GUI_clear(WHITE);
		  drawDotsOnTheEdge();
		  RESET_DM();
		  GUI_drawLine(left, right, GREEN, WHITE, true,  pix);
		  HAL_Delay(2000);
      GUI_clear(WHITE);
	    RESET_DM();
		  GUI_drawLine(left, right, GREEN, WHITE, false, pix);
		  HAL_Delay(2000); 
	 }
}

// EOF
