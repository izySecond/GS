/**
 ******************************************************************************
 * @file    lcd.h
 * @author  Silke Behn
 *          HAW-Hamburg
 *          Labor für technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    16. Juli 2020
 * @brief   Funktionen zum ansteuern des Waveshare 4 inch 480*320 Resistive Touch TFT
 ******************************************************************************
 */

#ifndef _LCD_H
#define	_LCD_H

#include "LCD_general.h"


/**
* @brief Gibt einen String auf dem LCD aus. Wenn der String zu lang für eine Zeile ist, 
*        wird er über mehrere Zeilen ausgegeben. Alle Zeichen, die sich auf dem Display 
*        hinter dem String befinden, werden geloescht.
* @param txt  String, der ausgegeben werden soll
* @retval None
*/
void lcdPrintS(char txt[]);

/**
* @brief Gibt einen String auf dem LCD aus. Wenn der String zu lang für eine Zeile ist, 
*        wird er über mehrere Zeilen ausgegeben. 
*        Vor der Ausgabe werden genau die Zeichen auf dem Display geloescht, die anschließend 
*        von dem neuen String überschrieben werden
* @param txt  String, der ausgegeben werden soll
* @retval None
*/
void lcdPrintReplS(char txt[]);

/**
* @brief Gibt eine Zahl auf dem Bildschirm aus
* @param zahl Ganzzahl, die ausgegeben werden soll
* @retval None
*/  
void lcdPrintInt(int zahl);


/**
* @brief Gibt den String auf dem LCD aus (siehe lcdPrintS), anschließend wird eine neue 
*        Zeile begonnen.
* @param txt String, der ausgegeben werden soll
* @retval None
*/
void lcdPrintlnS(char txt[]);

/**
* @brief Gibt ein Zeichen auf dem LCD aus.
* @param c Zeichen, dass ausgegeben werden soll
* @retval None
*/
void lcdPrintC(char c);

/**
* @brief Eine Schriftgroesse wird eingestellt
* @param font Schriftgroesse erlaubt sind (8,12,16,20,24)
* @retval None
*/
void lcdSetFont(int font);

/**
* @brief Der Cursor wird auf die Position x/y gesetzt
* @param x x-Koordinate
*        y y-Koordinate
* @retval None
*/
void lcdGotoXY( POINT x, POINT y);




#endif
// EOF

