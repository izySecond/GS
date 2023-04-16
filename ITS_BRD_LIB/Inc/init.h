/**
 ******************************************************************************
 * @file    init.h
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor für technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.1
 *
 * @date    17. Juli 2020
 * @brief   Initialisierungsfunktionen des ITSboard.
 *	11.11.2020 Tobias Jaehnichen initLCDTouch hinzugefuegt
 ******************************************************************************
 */
 

#ifndef _INIT_H
#define	_INIT_H

/**
  * @brief Initialisierung des ITSboards.
	*        Diese Funktion muss direkt am Anfang von main aufgerufen werden.
  * @param  None
  * @retval None
  */
void initITSboard(void);

/**
  * @brief Initialisierung des LCD und Touch.
  * @param  None
  * @retval None
  */
void initLCDTouch(void);
#endif

// EOF
