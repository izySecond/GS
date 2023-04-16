/**
 ******************************************************************************
 * @file    stdinout_USART.c
 * @author  Tobias De Gasperis
 *          HAW-Hamburg
 *          Labor für technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    25. April 2022
 * @brief   Initialisierung des USART-Ports und Bereitstellung der Funktionen
 *					fuer stdin und stdout via serieller Schnittstelle. Basierend auf
 *          dem STDIN USART Template von ARM LIMITED
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2013 - 2015 ARM LIMITED
 *
 *  All rights reserved.
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  - Neither the name of ARM nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
 
#include "Driver_USART.h"
 
//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------
 
// <h>STDIN USART Interface
 
//   <o>Connect to hardware via Driver_USART# <0-255>
//   <i>Select driver control block for USART interface
#define USART_DRV_NUM           3
 
//   <o>Baudrate <1=>9600 <2=>14400 <3=>19200 <4=>56000 <5=>115200 <6=>256000
#define BAUDRATE          5
#if    (BAUDRATE == 2)
#define USART_BAUDRATE          14400
#elif  (BAUDRATE == 3)
#define USART_BAUDRATE          19200
#elif  (BAUDRATE == 4)
#define USART_BAUDRATE          56000
#elif  (BAUDRATE == 5)
#define USART_BAUDRATE          115200
#elif  (BAUDRATE == 6)
#define USART_BAUDRATE          256000
#else
#define USART_BAUDRATE          9600
#endif

//   <o>DataBits <5=>5 <6=>6 <7=>7 <8=>8 <9=>9
#define DATA_BITS 8
#if    (DATA_BITS == 5)
#define ARM_USART_DATA_BITS_5               (5UL << ARM_USART_DATA_BITS_Pos)    ///< 5 Data bits
#elif  (DATA_BITS == 6)
#define ARM_USART_DATA_BITS_6               (6UL << ARM_USART_DATA_BITS_Pos)    ///< 6 Data bit
#elif  (DATA_BITS == 7)
#define ARM_USART_DATA_BITS_7               (7UL << ARM_USART_DATA_BITS_Pos)    ///< 7 Data bits
#elif  (DATA_BITS == 9)
#define ARM_USART_DATA_BITS_9               (1UL << ARM_USART_DATA_BITS_Pos)    ///< 9 Data bits
#else
#define ARM_USART_DATA_BITS_8               (0UL << ARM_USART_DATA_BITS_Pos)    ///< 8 Data bits (default)
#endif

//   <o>StopBits <4=>0.5 <1=>1 <3=>1.5 <2=>2  
#define STOP_BITS 1

#if    (STOP_BITS == 2)
#define ARM_USART_STOP_BITS_2               (1UL << ARM_USART_STOP_BITS_Pos)    ///< 2 Stop bits
#elif  (STOP_BITS == 3)
#define ARM_USART_STOP_BITS_1_5             (2UL << ARM_USART_STOP_BITS_Pos)    ///< 1.5 Stop bits
#elif  (STOP_BITS == 4)
#define ARM_USART_STOP_BITS_0_5             (3UL << ARM_USART_STOP_BITS_Pos)    ///< 0.5 Stop bits
#else
#define ARM_USART_STOP_BITS_1               (0UL << ARM_USART_STOP_BITS_Pos)    ///< 1 Stop bit (default)
#endif




//   <o>Parity <0=>None <1=>Even <2=>ODD
#define PARITY 0
#if    (PARITY == 1)
#define ARM_USART_PARITY_EVEN               (1UL << ARM_USART_PARITY_Pos)       ///< Even Parity
#elif  (PARITY == 2)
#define ARM_USART_PARITY_ODD                (2UL << ARM_USART_PARITY_Pos)       ///< Odd Parity
#else
#define ARM_USART_PARITY_NONE               (0UL << ARM_USART_PARITY_Pos)       ///< No Parity (default)
#endif
 
// </h>
 
 
#define _USART_Driver_(n)  Driver_USART##n
#define  USART_Driver_(n) _USART_Driver_(n)
 
extern ARM_DRIVER_USART  USART_Driver_(USART_DRV_NUM);
#define ptrUSART       (&USART_Driver_(USART_DRV_NUM))
 
 
/**
  Initialize stdin
 
  \return          0 on success, or -1 on error.
*/
int stdinout_init (void) {
  int32_t status;
 
  status = ptrUSART->Initialize(NULL);
  if (status != ARM_DRIVER_OK) return (-1);
 
  status = ptrUSART->PowerControl(ARM_POWER_FULL);
  if (status != ARM_DRIVER_OK) return (-1);
 
  status = ptrUSART->Control(ARM_USART_MODE_ASYNCHRONOUS |
                             ARM_USART_DATA_BITS_8       |
                             ARM_USART_PARITY_NONE       |
                             ARM_USART_STOP_BITS_1       |
                             ARM_USART_FLOW_CONTROL_NONE,
                             USART_BAUDRATE);
  if (status != ARM_DRIVER_OK) return (-1);
 
  status = ptrUSART->Control(ARM_USART_CONTROL_RX, 1);
  if (status != ARM_DRIVER_OK) return (-1);
	
	status = ptrUSART->Control(ARM_USART_CONTROL_TX, 1);
  if (status != ARM_DRIVER_OK) return (-1);
 
  return (0);
}
 
 
/**
  Get a character from stdin
 
  \return     The next character from the input, or -1 on read error.
*/
int stdin_getchar (void) {
  uint8_t buf[1];
 
  if (ptrUSART->Receive(buf, 1) != ARM_DRIVER_OK) {
    return (-1);
  }
  while (ptrUSART->GetRxCount() != 1);
  return (buf[0]);
}

/**
  Put a character to the stdout
 
  \param[in]   ch  Character to output
  \return          The character written, or -1 on write error.
*/
int stdout_putchar (int ch) {
  uint8_t buf[1];
 
  buf[0] = ch;
  if (ptrUSART->Send(buf, 1) != ARM_DRIVER_OK) {
    return (-1);
  }
  while (ptrUSART->GetTxCount() != 1);
  return (ch);
}
