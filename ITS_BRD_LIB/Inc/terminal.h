/**
 ******************************************************************************
 * @file    terminal.h
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor für technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    27. Juli 2020
 * @brief   Dieses Modul implementiert ein einfaches Terminal für das LCD
 *          Display. Weitere Details siehe terminal.c.
 *
 ******************************************************************************
 */

#ifndef _TERMINAL_H
#define	_TERMINAL_H
#include <stdbool.h>
#include "LCD_general.h"
#include "fonts.h"

// Beschreibung eines Terminals
typedef struct {
	sFONT *font;        // Font
	Coordinate topLeft; // linke obere Ecke des Terminals
	Coordinate bottomRight;
	COLOR	bgColor;			// Hintergrundfarbe
	COLOR fgColor;      // Schriftfarbe
	uint8_t rows;       // Anzahl Zeilen
	uint8_t cols;       // Anzahl Spalten
	// Position für das naechste zu druckende Zeichen
	uint8_t curRow;
	uint8_t curCol;
	bool writeInACircle; // true : Schreibe weiter in Zeile 0, wenn die letzte Zeite voll ist
	                     // false : Lösche das Terminal, wenn alle Zeilen voll geschrieben sind.
} Terminal;

#define INVALID_TERM {.font = NULL,     .topLeft = {0,0}, .bottomRight = {0,0}, \
	                    .bgColor = BLACK, .fgColor = BLACK,  .rows = 0, \
	                    .cols = 0,        .curRow = 0,       .curCol = 0, \
                      .writeInACircle = true}

/**
  * @brief  Diese Funktion legt ein Terminal an. Mehrere Terminals koennen parallel
  *         betrieben werden.
  * @param  topLeft     Obere linke Koordinate des Terminals
  * @param  bottomRight Untere rechte Koordinate des Terminals 
  * @param  bgColor     Hintergrundfarbe 
  * @param  fgColor     Textfarbe 
  * @param  font        Fonts des Textes
  * @retval Struktur, die das Terminal beschreibt
  */
extern Terminal genTerm(Coordinate topLeft, Coordinate bottomRight, COLOR	bgColor, COLOR fgColor, sFONT *font);

/**
  * @brief  Diese Funktion setzt die Farbe des Texts eines Terminals.
  * @param  fgColor Textfarbe 
  * @param  term    Das Terminal, dessen Textfarbe veraendert wird.
  * @retval none
  */
extern void setTextColorTerm(COLOR fgColor, Terminal *term);

/**
  * @brief  Diese Funktion setzt die Position der Ausgabe des naechsten Zeichens.
  * @param  row    Zeile der Zeichenausgabe 
  * @param  column Spalte der Zeichenausgabe 
  * @param  term   Das Terminal, dessen aktuelle Zeichenposition veraendert wird.
  * @retval none
  */
extern void setPosTerm(uint8_t row, uint8_t column, Terminal *term);

/**
  * @brief  Diese Funktion gibt einen Text auf einem Terminal aus. 
	*         Zeilenumbrueche (\n) werden verarbeitet.
  * @param  text  Text, der ausgegeben wird. 
  * @param  term  Das Terminal, auf dem der Text ausgegeben wird.
  * @retval none
  */
extern void printTerm(char * text, Terminal *term);

/**
  * @brief  Diese Funktion loescht das Terminal. 
  * @param  term  Das Terminal, das geloescht werden soll.
  * @retval none
  */
extern void clearTerm(Terminal *term);

/**
  * @brief  Diese Funktion liefert die aktuelle Zeile eines Terminals. 
  * @param  term  Das entsprechende Terminal
  * @retval aktuelle Zeile
  */
extern int getCurRow(Terminal *term);

/**
  * @brief  Diese Funktion liefert die Zeilenanzahl eines Terminals. 
  * @param  term  Das entsprechende Terminal
  * @retval Anzahl Zeilen
  */
extern int getNoOfRows(Terminal *term);

void printChar(char c, Terminal *term);

#endif
// EOF
