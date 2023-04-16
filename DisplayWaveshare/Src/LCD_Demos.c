/**
 ******************************************************************************
 * @file    LCD_Demos.c
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

#include "LCD_Demos.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "LCD_Debug.h"
#include "fontsFLASH.h"
#include "../IncLocal/LCD_Driver.h"

void GUI_show(void){
    GUI_clear(WHITE);
    if(LCD_isHorizontalMode()) { //Horizontal screen display		
			  Coordinate left, right;
        left.x = 0; left.y = 10;  right.x = LCD_WIDTH - 1; right.y = 10;  GUI_drawLine(left, right, RED, LCD_BACKGROUND, true,  DOT_PIXEL_2X2);
                    left.y = 20;                           right.y = 20;  GUI_drawLine(left, right, RED, LCD_BACKGROUND, false, DOT_PIXEL_1X1);
                    left.y = 300;                          right.y = 300; GUI_drawLine(left, right, RED, LCD_BACKGROUND, false, DOT_PIXEL_1X1);
                    left.y = 310;                          right.y = 310; GUI_drawLine(left, right, RED, LCD_BACKGROUND, true,  DOT_PIXEL_2X2);
			
        left.x =10; left.y = 30;  right.x = LCD_WIDTH - 10; right.y = LCD_HEIGHT - 30; GUI_drawRectangle(left, right, BLUE, false, DOT_PIXEL_1X1);
			  left.x =20; left.y = 40;  right.x = LCD_WIDTH - 20; right.y = 60;              GUI_drawRectangle(left, right, BLUE, true, DOT_PIXEL_1X1);

			  // draw olympic rings
			  Coordinate cnt;
			  const LENGTH Cr = 20;
	      cnt.x = 190;                  cnt.y = 240;      GUI_drawCircle(cnt, Cr, BLUE,   false, DOT_PIXEL_2X2);
        cnt.x = 190 + (2.5 * Cr);                       GUI_drawCircle(cnt, Cr, BLACK,  false, DOT_PIXEL_2X2);
        cnt.x = 190 + (5 * Cr);                         GUI_drawCircle(cnt, Cr, RED,    false, DOT_PIXEL_2X2);
        cnt.x = 190 + (2.5 * Cr) / 2; cnt.y = 240 + Cr; GUI_drawCircle(cnt, Cr, YELLOW, false, DOT_PIXEL_2X2);
        cnt.x = 190 + (7.5 * Cr) / 2;                   GUI_drawCircle(cnt, Cr, GREEN,  false, DOT_PIXEL_2X2);	

				cnt.x = 50;             cnt.y = 250; GUI_drawCircle(cnt, 30, CYAN, true, DOT_PIXEL_1X1);
        cnt.x = LCD_WIDTH - 50;              GUI_drawCircle(cnt, 30, CYAN, true, DOT_PIXEL_1X1);

        Coordinate tl;
				tl.x = 80; tl.y = 80;  GUI_disStr(tl, "WaveShare Electronic", &Font24FLASH, LCD_BACKGROUND, BLUE);
                   tl.y = 120; GUI_disStr(tl, "3.5inch TFTLCD", &Font20FLASH, RED, BLUE);
                   tl.y = 150; GUI_disNum(tl, 1234567890, &Font16FLASH, LCD_BACKGROUND, BLUE);
    } else { //Vertical screen display
				// Vertical sceen display not supported.
			  DEBUG("Vertical screen not supported. Please use original Waveshare SW");
    }
}

/*******************************************************************************
function:
		Domo Draw Board of Waveshare
*******************************************************************************/

void TP_Dialog(void){
	  const LENGTH maxCol = LCD_WIDTH - 1;
    LCD_clear(LCD_BACKGROUND);
    // DEBUG("Drawing...\n");
    //Horizontal screen display
    if(LCD_isHorizontalMode()) {
        //Clear screen
			  Coordinate tl = {maxCol - 60, 0};
        GUI_disStr(tl, "CLEAR", &Font16FLASH, RED, BLUE);
        //adjustment
				tl.x = maxCol - 120; tl.y = 0;
        GUI_disStr(tl, "AD", &Font24FLASH, RED, BLUE);
        //choose the color
				Coordinate left, right;
				
        left.x = maxCol - 50; 
        right.x = maxCol;	
				left.y = 20;  right.y = 70;  GUI_drawRectangle(left, right, BLUE, true, DOT_PIXEL_1X1);
        left.y = 80;  right.y = 130; GUI_drawRectangle(left, right, GREEN, true, DOT_PIXEL_1X1);
        left.y = 140; right.y = 190; GUI_drawRectangle(left, right, RED, true, DOT_PIXEL_1X1);
        left.y = 200; right.y = 250; GUI_drawRectangle(left, right, YELLOW, true, DOT_PIXEL_1X1);
        left.y = 260; right.y = 310; GUI_drawRectangle(left, right, BLACK, true, DOT_PIXEL_1X1);
    } else { //Vertical screen display
			  // Vertical sceen display not supported.
			  DEBUG("Vertical screen not supported. Please use original Waveshare SW");
    }
}

void TP_DrawBoard(void){
	 const LENGTH maxCol = LCD_WIDTH - 1;
	 const DOT_PIXEL brushWidth = DOT_PIXEL_2X2;
	 static COLOR brushColor = BLUE;
   if (!TP_pressed()) return;
	 // Touch screen pressed
   Coordinate crd = TP_get();
	 if (!VALID_COORDINATE(crd)) return;
	 // TP provided valid coordinate	
   if(LCD_isHorizontalMode()) { // Horizontal screen
      if (crd.x > (maxCol - 60) && crd.y < 16) {		//Clear Board
         TP_Dialog();
      } else if(crd.x > (maxCol - 120) && crd.x < (maxCol - 80) &&
                crd.y < 24) { //afresh adjustment
         TP_Adjust();
         TP_Dialog();
      } else if(crd.x > (maxCol - 50) && crd.x < maxCol &&
                crd.y > 20 && crd.y < 70) {
         brushColor = BLUE;
      } else if(crd.x > (maxCol - 50) && crd.x < maxCol &&
                crd.y > 80 && crd.y < 130) {
         brushColor = GREEN;
      } else if(crd.x > (maxCol - 50) && crd.x < maxCol &&
                crd.y > 140 && crd.y < 190) {
         brushColor = RED;
      } else if(crd.x > (maxCol - 50) && crd.x < maxCol &&
                crd.y > 200 && crd.y < 250) {
         brushColor = YELLOW;
      } else if(crd.x > (maxCol - 50) && crd.x < maxCol &&
                crd.y > 260 && crd.y < 310) {
         brushColor = BLACK;
      } else { // On canvas
	       if (crd.x < (maxCol - 50)) {
            GUI_drawPoint(crd, brushColor, brushWidth, DOT_FILL_RIGHTUP);
				 }
      }
	 } else { // Vertical screen
		  // Vertical sceen display not supported.
		  DEBUG("Vertical screen not supported. Please use original Waveshare SW");
   }
}

// EOF
