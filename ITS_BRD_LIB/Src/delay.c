/**
 ******************************************************************************
 * @file    delay.c
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor f�r technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    2020-08-20
 * @brief   Dieses Modul implementiert einen Wrapper f�r die Delay Funktionen.
 *
 ******************************************************************************
 */

#include "delay.h"
#include "stm32f4xx_hal.h"

void delay(unsigned int delta){
	 if (delta == 0) return;
	 HAL_Delay(delta-1); // HAL_Delay verz�gert um eine Millisekunde laenger
}
// EOF
