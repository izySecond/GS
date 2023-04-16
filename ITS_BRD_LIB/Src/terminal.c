/**
 ******************************************************************************
 * @file    terminal.c
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor für technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    27. Juli 2020
 * @brief   Dieses Modul implementiert ein einfaches Terminal für das LCD
 *          Display. Gleichzeitig können mehrere Terminals betrieben werden.
 *          Aufgrund mangelnder HW Unterstuetzung wird Scrolling nicht 
 *          unterstuetzt. Stattdessen wird in der obersten Zeile weiter-
 *          geschrieben, wenn die letzte Spalte der untersten Zeile be-
 *          schrieben wurde.
 *
 ******************************************************************************
 */

#include "terminal.h"
#include "LCD_GUI.h"
#include "error.h"
#include "lcd.h"

Terminal genTerm(Coordinate topLeft, Coordinate bottomRight, COLOR	bgColor, COLOR fgColor, sFONT *font) {
   Terminal erg = {.topLeft = topLeft, .bottomRight = bottomRight, .rows = 0, .cols = 0, 
		   .bgColor = bgColor, .fgColor = fgColor, .font = font,
		   .curRow = 0, .curCol = 0, .writeInACircle = false};
	 
	 erg.cols = (bottomRight.x - topLeft.x) / font->Width;
	 erg.rows = (bottomRight.y - topLeft.y) / font->Height;
		
   // Test Parameter
	if ((topLeft.x >= bottomRight.x) || (topLeft.y >= bottomRight.y) || (erg.cols == 0) ||
		  (LCD_WIDTH  <= bottomRight.x)|| (LCD_HEIGHT <= bottomRight.y)|| (erg.rows == 0)){				 
			lcdPrintS("genTerm: Fehlerhafte Terminalgroesse.");
		  Error_Handler();
	 }

	 clearTerm(&erg); // Zeichne Hintergrund
	 return erg;
}

void clearTerm(Terminal *term) {	
   GUI_drawRectangle(term->topLeft, term->bottomRight, term->bgColor, true, DOT_PIXEL_1X1);
	 term->curCol = 0;
	 term->curRow = 0;
}

void setTextColorTerm(COLOR fgColor, Terminal *term) {
	 term->fgColor = fgColor;
}

void setPosTerm(uint8_t row, uint8_t column, Terminal *term) {
	 // Test Parameter
	 if ((row  >= term->rows) || (column  >= term->cols)){				 
			lcdPrintS("setPosTerm: Parameter row oder column falsch.");
		  Error_Handler();
	 }
	 // Setze Position fuer das naechste Zeichen
	 term->curRow = row;
	 term->curCol = column;
}

void printChar(char c, Terminal *term){
	 if ((0 == term->curRow) && (0 == term->curCol) && (!term->writeInACircle)) {
		  clearTerm(term);
	 }
	 Coordinate crd = {term->topLeft.x + term->curCol * term->font->Width, 
			               term->topLeft.y + term->curRow * term->font->Height};
   GUI_disChar(crd, c, term->font, term->bgColor, term->fgColor);
	 // gehe eine Position weiter
	 term->curCol = (term->curCol + 1) % term->cols;
	 if (0 == term->curCol) {
			term->curRow = (term->curRow + 1) % term->rows;
	 }
}

static void printNewLine(Terminal *term){
	 do {
		  printChar(' ', term);
	 } while(0!= term->curCol);
}

void printTerm(char * text, Terminal *term) {
	 while (0 != *text){
		  if ('\n' == *text) {
			   printNewLine(term);
		  } else {
			   printChar(*text, term);
			}
			text++;
   }
}

int getCurRow(Terminal *term) {
	 return term->curRow;
}

int getNoOfRows(Terminal *term) {
	 return term->rows;
}

// EOF
