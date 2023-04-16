/**
 ******************************************************************************
 * @file    testsoftware.h
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

#ifndef _TESTSOFTWARE_H
#define _TESTSOFTWARE_H

/**
  * @brief  Initialisation of an test-LED.
  * @retval None
  */
extern void initTestLed(void);

/**
  * @brief  Switch on test-LED.
  * @retval None
  */
extern void setTestLedOn(void);

/**
  * @brief  Switch off test-LED.
  * @retval None
  */
extern void setTestLedOff(void);

/**
  * @brief  Switch test-LED on and off.
  * @retval None
  */
extern void toogleTestLed(void);

/**
  * @brief  Test code for GUI_drawLine.
  * @retval None
  */
extern void testDrawLines(void);

/**
  * @brief  Test code for GUI_drawRectangle.
  * @retval None
  */
extern void testDrawRectangle(void);

#endif
// EOF
