/**
 ******************************************************************************
 * @file    keypad.h
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor f�r technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    27. Juli 2020
 * @brief   Dieses Modul implementiert ein einfaches Keypad f�r das LCD
 *          Display. Weitere Details siehe terminal.c.
 *
 ******************************************************************************
 */

#ifndef _HAW_KEYPAD_H
#define	_HAW_KEYPAD_H

#include <stdbool.h>
#include <terminal.h>

	
/**
  * @brief  Diese Funktion zeichnet ein KeyPad mit Terminal.
  * @retval Das Terminal des KeyPads.
  */
Terminal makeKeyPad(void);

/**
  * @brief  Dies Funktion wartet auf den naechsten gueltigen Tastendruck und
  *         gibt das Symbol der gedrueckten Taste zur�ck.
  * @retval ASCII Zeichen der gedr�ckten Taste . Im Fehlerfall: 0
  */
char getKeyPadInput(void);

#endif	/* KEYPAD_H */
// EOF
