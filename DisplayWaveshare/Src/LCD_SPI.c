/**
 ******************************************************************************
 * @file    LCD_SPI.c
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor für technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    2020-07-16
 * @brief   This module implements the interface to the SPI bus used by
 *          the LCD display.
 *
 ******************************************************************************
 */

#include "../IncLocal/LCD_SPI.h"
#include "../IncLocal/LCD_Tools.h"
#include "../IncLocal/LCD_GPIO.h"

#include <stdbool.h>

#include "stm32f4xx_hal.h"

// This module supports two SPI interfaces.
// The first one uses the HAL of STM32CubeF4 firmware.
// The second one accesses HW directly. This Intface was used
// by the "Old TI board."
// To reduce Code size, a define statement selects the interface.

// #define USE_SPI_HAL

#define SPI_BAUDRATEPRESCALER_TP        SPI_BAUDRATEPRESCALER_128     // KORF 64 sollte auch reichen
#define SPI_BAUDRATEPRESCALER_LCD       SPI_BAUDRATEPRESCALER_16      

/* SPI1 init code */
#ifdef USE_SPI_HAL
// SPI initialisation with HAL

static SPI_HandleTypeDef hspi1;

static void MX_SPI1_Init(void) {	
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_LCD;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK){
	   LCDErrHandler(__FILE__, __LINE__); 
	}
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle) {
  GPIO_InitTypeDef GPIO_InitStruct = {0,0,0,0,0};
  if(spiHandle->Instance==SPI1){
    /* USER CODE END SPI1_MspInit 0 */
    __HAL_RCC_SPI1_CLK_ENABLE(); // SPI1 clock enable
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI1; // KORF Setze Alternative Function für SPI1
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Pin = SPI1_SCK_Pin;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;  // KORF CLK auf inactive mode setzen
    HAL_GPIO_Init(SPI1_SCK_GPIO_Port, &GPIO_InitStruct);
		GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Pin = SPI1_MOSI_Pin;
    HAL_GPIO_Init(SPI1_MOSI_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = SPI1_MISO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; // KORF muss output sein GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI1; // KORF Setze Alternative Function für SPI1
    HAL_GPIO_Init(SPI1_MISO_GPIO_Port, &GPIO_InitStruct);
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle){ 
  if(spiHandle->Instance==SPI1){
    __HAL_RCC_SPI1_CLK_DISABLE();
    HAL_GPIO_DeInit(SPI1_MISO_GPIO_Port, SPI1_MISO_Pin);
    HAL_GPIO_DeInit(SPI1_MOSI_GPIO_Port, SPI1_MOSI_Pin);
    HAL_GPIO_DeInit(SPI1_SCK_GPIO_Port,  SPI1_SCK_Pin);
  }
} 

#else
// SPI initialisation without HAL

#define _GPIO_Mode_IN      0x00 /*!< GPIO Input Mode */
#define _GPIO_Mode_OUT     0x01 /*!< GPIO Output Mode */
#define _GPIO_Mode_AF      0x02 /*!< GPIO Alternate function Mode */
#define _GPIO_OType_PP     0x00
#define _GPIO_OType_OD     0x01
#define _GPIO_Speed_2MHz   0x00 /*!< Low speed */
#define _GPIO_Speed_25MHz  0x01 /*!< Medium speed */
#define _GPIO_Speed_50MHz  0x02 /*!< Fast speed */
#define _GPIO_Speed_100MHz 0x03  /*!< High speed on 30 pF (80 MHz Output max speed on 15 pF) */
#define _GPIO_PuPd_NOPULL  0x00
#define _GPIO_PuPd_UP      0x01
#define _GPIO_PuPd_DOWN    0x02

static void SPIInitGPIOsWithoutHAL(void){
	// enable peripherals
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	//PA5 as SPI1_SCK
	SPI1_SCK_GPIO_Port->MODER   = (SPI1_SCK_GPIO_Port->MODER   & ~(3u   << (5*2))) | (_GPIO_Mode_AF     << (5*2));
	SPI1_SCK_GPIO_Port->OSPEEDR = (SPI1_SCK_GPIO_Port->OSPEEDR & ~(3u   << (5*2))) | (_GPIO_Speed_50MHz << (5*2));
	SPI1_SCK_GPIO_Port->OTYPER  = (SPI1_SCK_GPIO_Port->OTYPER  & ~(1    << (5)))   | (_GPIO_OType_PP    << (5));
	SPI1_SCK_GPIO_Port->PUPDR   = (SPI1_SCK_GPIO_Port->PUPDR   & ~(3    << (5*2))) | (_GPIO_PuPd_DOWN   << (5*2));
	SPI1_SCK_GPIO_Port->AFR[0]  = (SPI1_SCK_GPIO_Port->AFR[0]  & ~(0xfU << (5*4))) | (GPIO_AF5_SPI1     << (5*4));
	
	// PA6 as SPI1_MISO
	SPI1_MISO_GPIO_Port->MODER   = (SPI1_MISO_GPIO_Port->MODER   & ~(3u   << (6*2))) | (_GPIO_Mode_AF     << (6*2));
	SPI1_MISO_GPIO_Port->OSPEEDR = (SPI1_MISO_GPIO_Port->OSPEEDR & ~(3u   << (6*2))) | (_GPIO_Speed_50MHz << (6*2));
	SPI1_MISO_GPIO_Port->OTYPER  = (SPI1_MISO_GPIO_Port->OTYPER  & ~(1    << (6)))   | (_GPIO_OType_PP    << (6));
	SPI1_MISO_GPIO_Port->PUPDR   = (SPI1_MISO_GPIO_Port->PUPDR   & ~(3    << (6*2))) | (_GPIO_PuPd_NOPULL << (6*2));
	SPI1_MISO_GPIO_Port->AFR[0]  = (SPI1_MISO_GPIO_Port->AFR[0]  & ~(0xfU << (6*4))) | (GPIO_AF5_SPI1     << (6*4));
	
	//PA7 as SPI1_MOSI
	SPI1_MOSI_GPIO_Port->MODER   = (SPI1_MOSI_GPIO_Port->MODER   & ~(3u   << (7*2)))  | (_GPIO_Mode_AF     << (7*2));
	SPI1_MOSI_GPIO_Port->OSPEEDR = (SPI1_MOSI_GPIO_Port->OSPEEDR & ~(3u   << (7*2)))  | (_GPIO_Speed_50MHz << (7*2));
	SPI1_MOSI_GPIO_Port->OTYPER  = (SPI1_MOSI_GPIO_Port->OTYPER  & ~(1    << (7)))    | (_GPIO_OType_PP    << (7));
	SPI1_MOSI_GPIO_Port->PUPDR   = (SPI1_MOSI_GPIO_Port->PUPDR   & ~(3u   << (7*2)))  | (_GPIO_PuPd_NOPULL << (7*2));
	SPI1_MOSI_GPIO_Port->AFR[0]  = (SPI1_MOSI_GPIO_Port->AFR[0]  & ~(0xfU << (7*4)))  | (GPIO_AF5_SPI1     << (7*4));	
}

static void SPIInitDevWithoutHAL(uint32_t baudrate){
	baudrate = baudrate & SPI_CR1_BR_Msk;
	
	// SPI Reset
	SPI1->CR1 = SPI1->CR1 & (~(1 <<  6));
	
	// Set SPI Parameter
	SPI1->CR1 = (0 << 15)  |  // BIDIMODE: 2-line unidirectional data mode selected
			        (0 << 14)  |  // BIDIOE:   --
			        (0 << 13)  |  // CRCEN:    CRC calculation disabled
			        (0 << 12)  |  // CRCNEXT:  --
			        (0 << 11)  |  // DFF:      8-bit data frame format
			        (0 << 10)  |  // RXONLY:   Full duplex
			        (1 <<  9)  |  // SSM:      Software slave management enabled
			        (1 <<  8)  |  // SSI       not used when bit 9 == 0
			        (0 <<  7)  |  // LSBFIRST: LSB transmitted first
			        (0 <<  6)  |  // SPE:      Peripheral enabled      // Expliziter Wechsel
			        (baudrate) |  // BR[0:2]   7 : 256   6 : 128
			        (1 <<  2)  |  // MSTR:     Master configuration    // Expliziter Wechsel
			        (0 <<  1)  |  // CPOL:     CK to 1 when idle
			        (0 <<  0);    // CPHA:     The second clock transition is the first data capture edge
	
	// Set CR2
	SPI1->CR2 = (0 <<  7) |  // TXEIE:    TXE interrupt masked
							(0 <<  6) |  // RXNEIE:   RXNE interrupt masked
							(0 <<  5) |  // ERRIE:    Error interrupt is masked
							(0 <<  4) |  // FRF   		SPI Motorola mode
							(0 <<  2) |  // SSOE:     SS output is enabled
							(0 <<  1) |  // TXDMAEN:  Tx buffer DMA disabled
							(0 <<  0);   // RXDMAEN:  Rx buffer DMA disabled

   // Disable I2S
	 SPI1->I2SCFGR =  SPI1->I2SCFGR & (~SPI_I2SCFGR_I2SMOD);
	
	 // Enable SPI
		SPI1->CR1 = SPI1->CR1 | (1 <<  6); // SPE:      Peripheral enabled
		
	// just some busy waiting to get SPI stable
	// for(int j=0; j<2000000; j++ ){ } delay to long for TP_Read_TwiceADC, it generates an error 
	
	return;
}
#endif


/********************************************************************************
function:	Wrapper to distinguish initialisation with or without HAL
note:
   Without HAL: Initialisation of SPI1 similar to the code of the first TI Board
********************************************************************************/
void SPIinit(void) {
#ifdef USE_SPI_HAL
		MX_SPI1_Init();
	  __HAL_SPI_ENABLE(&hspi1);
#else
		SPIInitGPIOsWithoutHAL();
		SPIInitDevWithoutHAL(SPI_BAUDRATEPRESCALER_LCD);
#endif
}

/********************************************************************************
function:	Wrapper to switch SPI bus spped for touch pad.
note:
   Distinguish between speed setup with or without HAL.
********************************************************************************/

void SPIsetTPSpeed(void) {
	 //A cycle of at least 400ns.
#ifdef USE_SPI_HAL
	 hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_TP;
	 HAL_SPI_Init(&hspi1);
	 __HAL_SPI_ENABLE(&hspi1);
#else
	 uint32_t sr1 = SPI1->SR;
	 SPIInitDevWithoutHAL(SPI_BAUDRATEPRESCALER_TP);
	 uint32_t sr2 = SPI1->SR;
	 if ((sr1 & (SPI_SR_BSY_Msk|SPI_SR_RXNE_Msk)) || 
			 (sr2 & (SPI_SR_BSY_Msk|SPI_SR_RXNE_Msk))) {
				 LCDErrHandler(__FILE__, __LINE__);
	 }
#endif
}

/********************************************************************************
function:	Wrapper to switch SPI bus spped for LCD.
note:
   Distinguish between speed setup with or without HAL.
********************************************************************************/
void SPIsetLCDSpeed(void) {
	 //LCD SPI speed = 18 MHz
#ifdef USE_SPI_HAL
   hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_LCD;
   HAL_SPI_Init(&hspi1);
   __HAL_SPI_ENABLE(&hspi1);
#else
   SPIInitDevWithoutHAL(SPI_BAUDRATEPRESCALER_LCD);
#endif
}

static uint32_t check_SPI_SR(uint32_t validFlags){
	 uint32_t sr = SPI1->SR & 0x000000FF;
	 validFlags = validFlags & 0x000000FF;
	 uint32_t inValidFlags = sr & ~validFlags;
	
	 if (0x00 == inValidFlags){
		 // keine ungültigen Flags gesetzt
		 return sr;	
	 }
	 // ungültige Flags gesetzt
	 LCDErrHandler(__FILE__, __LINE__);
	 return sr;
}

uint8_t SPI4W_Write_Byte(uint8_t value){
	  while(0x00 == (check_SPI_SR(SPI_SR_TXE_Msk) & SPI_SR_TXE_Msk)){};
	
    SPI1->DR = (uint32_t) value;
	
	  while(0x00 == (check_SPI_SR(SPI_SR_BSY_Msk|SPI_SR_TXE_Msk|SPI_SR_RXNE_Msk) & SPI_SR_RXNE_Msk)){}; // Wait for the receiving area to be empty
		
		while(SPI1->SR & SPI_FLAG_BSY){}; //Wait for not busy
			
		return (uint8_t) SPI1->DR; 
}

uint8_t SPI4W_Read_Byte(void){
	return SPI4W_Write_Byte(0xFF);
}

// EOF
