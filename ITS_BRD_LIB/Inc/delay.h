/**
 ******************************************************************************
 * @file    delay.h
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor für technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    2020-08-20
 * @brief   Dieses Modul implementiert einen Wrapper für die Delay Funktionen.
 *
 ******************************************************************************
 */

#ifndef _DELAY_H
#define	_DELAY_H
	
/**
  * @brief  Diese Funktion verzögert die Ausführung um delta ms.
  *
  * @param delta Zeitspanne, die verzögert wird (im ms).
  */
void delay(unsigned int delta);

#endif
// EOF
