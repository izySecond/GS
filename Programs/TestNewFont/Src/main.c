/**
  ******************************************************************************
  * @file    main.c
  * @author  Franz Korf
  * @brief   Kleines Testprogramm fuer neu erstelle Fonts.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"
#include "init.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "lcd.h"
#include "fontsFLASH.h"
#include "additionalFonts.h"
#include "error.h"

#define PORT_BLUE_BUTTON			GPIOC
#define PIN_BLUE_BUTTON				13

struct FontPair {
	sFONT *font;
	sFONT *fontFlash;
};

struct FontPair fontList[] = {  // List of Fonts to be tested.
	{&Font24, 						&Font24FLASH}, 
	{&Font20, 						&Font20FLASH}, 
	{&Font16, 						&Font16FLASH}, 
	{&Font12, 						&Font12FLASH}, 
	{&Font8,  						&Font8FLASH}, 
	{&CourierNew48,				&CourierNew48FLASH},
	{&CourierNewBold40,		&CourierNewBold40FLASH},
	{&CourierNewBold60,		&CourierNewBold60FLASH},
	{&Consolas40,					&Consolas40FLASH},
	{&Consolas60,					&Consolas60FLASH},
	{&Consolas80,					&Consolas80FLASH},
	{&Consolas90,					&Consolas90FLASH},
	{&ConsolasBold40,			&ConsolasBold40FLASH},
	{&ConsolasBold60,			&ConsolasBold60FLASH},
	{&ConsolasBold80,			&ConsolasBold80FLASH},
	{&ConsolasBold90,			&ConsolasBold90FLASH},
	{&FiraMono40,					&FiraMono40FLASH},
	{&FiraMono60,					&FiraMono60FLASH},
	{&FiraMono80,					&FiraMono80FLASH},
	{&FiraMono90,					&FiraMono90FLASH},
	{&FiraMonoMedium40,		&FiraMonoMedium40FLASH},
	{&FiraMonoMedium60,		&FiraMonoMedium60FLASH},
	{&FiraMonoMedium80,		&FiraMonoMedium80FLASH},
	{&FiraMonoMedium90,		&FiraMonoMedium90FLASH},
};


/**
  ******************************************************************************
  * @brief  Diese Funktion initialisiert den blauen Button des Nucleo Boards.
  *         Er ist mit Pin PC 13 verbunden.
  * @param  None
  * @retval None
  ******************************************************************************
  */
static void initBlueButton(void){
	// blauer Taster = Port C Pin 13
	__HAL_RCC_GPIOC_CLK_ENABLE();          // enable peri. clock for port C
	// setze den Pin von Blue Button auf Input
	PORT_BLUE_BUTTON->MODER = PORT_BLUE_BUTTON->MODER & ~(0x03U << (PIN_BLUE_BUTTON*0));   // setze Port A pin 0 auf input
	// setze die Geschwindigkeit des Pin von Blue Button auf high
	PORT_BLUE_BUTTON->OSPEEDR = PORT_BLUE_BUTTON->OSPEEDR | (0x03 << (PIN_BLUE_BUTTON*0)); // setze Port A pin 0 of high speed
}

/**
  ******************************************************************************
  * @brief  Diese Funktion liefert den Status des blauen Button des Discovery 
  *         Boards.
  * @param  None
  * @retval 0 : Button nicht gedrueckt, 
  *         1 : Button gedrueckt.
  *        -1 : Fehler - Bounce Berechnung.
  ******************************************************************************
  */
 static char statusBlueButton(void){
	const int anzahlLeseversuch = 5;
	const int bounceDelay       = 5 /* ms */;
	
	uint16_t button = PORT_BLUE_BUTTON->IDR & (0x01U << PIN_BLUE_BUTTON);
	for (int i = 0 ; i < anzahlLeseversuch; i++) {
		HAL_Delay(bounceDelay); 
    uint16_t x = PORT_BLUE_BUTTON->IDR & (0x01U << PIN_BLUE_BUTTON);
		if (button == x) {
			return (button)? 1 : 0;
		}
		button = x;
	}
	return 0xFF; // Lesefehler
}

/**
  ******************************************************************************
  * @brief  Diese Funktion wartet, bis der blauen Button gedrueckt und wieder 
  *         losgelassen wird.
  * @param  None
  * @retval None
  ******************************************************************************
  */
static void waitUntilButtonPressedAndReleased() {
	while  (1 != statusBlueButton()) { } // Warte, bis der blaue Button gedrueckt ist.
	while  (0 != statusBlueButton()) { } // Warte, bis der blaue Button losgelassen wird
}

/**
  ******************************************************************************
  * @brief  Diese Funktion durchlaueft den STM Zeichensatz und 
  *         gibt jedes Zeichen aus einem etablierten Referenzzeichensatz
  *         von STM und dem zu testenden Zeichensatz aus.
  * @param  f   Der zu testende Zeichenssatz
  * @retval None
  ******************************************************************************
  */
static void runTest(struct FontPair *f) {
	const Coordinate ref = {10,10};
	const Coordinate test = {40,40};
	sFONT *refFont      = &Font16;
	
  GUI_clear(BLACK);                        // clear display and set background color	
	for (char c = ' '; c <= '~'; c++) {
		// display reference symbol
		GUI_disChar(ref, c, refFont, BLUE, YELLOW);
	  // display test symbol form program memory
		GUI_disChar(test, c, f->font, BLUE, YELLOW);
	  // display test symbol form flash memory
		Coordinate test2 = {test.x + 2 *(f->font->Width), test.y };
		GUI_disChar(test2, c, f->fontFlash, BLUE, YELLOW);
    HAL_Delay(1500);
		// clear Display	
		GUI_clear(BLACK);   
	}
}

int main(void) {
	initITSboard();    // Initialisierung des ITS Boards
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch
	if (!checkVersionFlashFonts()) {
	    // Ueberpruefe Version der Fonts im Flash passt nicht zur Software Version
		  Error_Handler();
	}
	initBlueButton();
	
	if (!checkVersionFlashFonts()) {
	    // Ueberpruefe Version der Fonts im Flash passt nicht zur Software Version
		  Error_Handler();
	}
  // Begruessungstext	
	lcdPrintlnS("Druecke blauen Taster zum Start.");
	waitUntilButtonPressedAndReleased();
	
	// Test in Endlosschleife
	while(1) {
		for (int i = 0; i < sizeof(fontList) / sizeof(fontList[0]); i++) {
			runTest(fontList+i);
		}
		
		lcdPrintlnS("Test abgeschlossen. Neustart in 10s.");
		HAL_Delay(10000);
	}
}

// EOF
