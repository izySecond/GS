/*****************************************************************************
* | File      	:	LCD_Touch.h
* | Author      :   Waveshare team
* | Function    :	LCD Touch Pad Driver and Draw
* | Info        :
*   Image scanning
*      Please use progressive scanning to generate images or fonts
*----------------
* |	This version:   V1.0
* | Date        :   2017-08-16
* | Info        :   Basic version
*
******************************************************************************/
#ifndef __LCD_TOUCH_H_
#define __LCD_TOUCH_H_

#include <stdbool.h>
#include "LCD_general.h"

/**
  * @brief  Diese Funktion kalibriert das Touch Pad.
  * @retval None
  */
extern void TP_Adjust(void);

/**
  * @brief  Diese Funktion initialisiert das Touch Pad.
	* @param  TPcal TP wird kalibriert
  * @retval None
  */
extern void TP_Init(bool TPcal);

/**
  * @brief  Die Funktion ueberprueft, ob das Touch Pad gedrueckt ist
  *         und speichert die Koordinaten.
  * @retval Touch gerade gedrueckt.
  */
extern bool TP_pressed(void);

/**
  * @brief  Die Funktion liefert die Koordinate des letzten gültigen Touch.
  * @retval Die Koordinate des letzten Touch.
	*         Falls die Auswertung des Touch aufgrund zu starker Abweichungen
	*         fehlerhaft ist, wird eine ungültige Koordinate zurück gegegeben.
  */
extern Coordinate TP_get(void);

#endif

// EOF

