/**
 ******************************************************************************
 * @file    keypad.c
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor für technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    27. Juli 2020
 * @brief   Dieses Modul implementiert ein einfaches Keypad für das LCD
 *          Display. Im rechten Teil befindet sich ein kleines Terminal 
 *          fuer die Ausgabe.
 *          Bei Bedarf sollte der Touch vorab bei der Initialisierung kalibiert 
 *          werden.
 *
 ******************************************************************************
 */

#include "keypad.h"
#include <stdint.h>
#include <stdio.h>
#include "LCD_general.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "terminal.h"
#include "delay.h"
#include "fontsFLASH.h"  // Fonts located in FLASH memory behind program 

/** The keypad consists of 6 rows and 7 columms. 
  * All buttons have the same size.
  */ 
	
#define BACKGROUND_DISPLAY		BLACK
#define BACKGROUND_BUTTON		  BLUE
#define FOREGROUND_BUTTON		  YELLOW
#define BACKGROUND_TERMINAL		GREEN
#define FOREGROUND_TERMINAL		BLACK

#define KEY_FONT              Consolas40FLASH
#define TERMINAL_FONT         Font16FLASH
	
// Label of keypad button
struct LabelOfButton { // LabelKeypad
   int8_t XOffset;     // Offset topleft corner of  key labeling
	 int8_t YOffset;
	 char c;             // Key labeling, 0x00 : no button
};

// Button of keypad
struct Button {    
   Coordinate topLeft;
   struct LabelOfButton label; // 0x00 : no button
};

// Definition of invalid button
#define INVALID_BUTTON {0,0, {0,  0, 0x00}}

#define SET_INVALID(X) {(X).topLeft.x = 0;     (X).topLeft.y = 0; \
                        (X).label.XOffset = 0; (X).label.YOffset = 0; (X).label.c = 0x00; }

// Test if button is valid
#define VALID_BUTTON(x) ((x).label.c != 0x00)

#define NO_ROWS				             6
#define NO_COLUMNS			           7

#define X_POS_KEYPAD_AREA          0
#define Y_POS_KEYPAD_AREA          0
#define WIDTH_KEYPAD_AREA          (LCD_WIDTH  - X_POS_KEYPAD_AREA)
#define HEIGHT_KEYPAD_AREA         (LCD_HEIGHT - Y_POS_KEYPAD_AREA)

#define BUTTON_WIDTH                60
#define BUTTON_HEIGHT               50
#define GAP_BETWEEN_BUTTONS         3
#define GAP_BETWEEN_KEYPAD_PART     (12 * GAP_BETWEEN_BUTTONS)

#define X_COORDINATE(X)     (X_POS_KEYPAD_AREA + GAP_BETWEEN_BUTTONS + (X)*(BUTTON_WIDTH + GAP_BETWEEN_BUTTONS) + (((X)>2)?GAP_BETWEEN_KEYPAD_PART:0))
#define Y_COORDINATE(Y)     (Y_POS_KEYPAD_AREA + 2 + (Y)*(BUTTON_HEIGHT + GAP_BETWEEN_BUTTONS))
#define COORDINATES(X,Y)    {X_COORDINATE(X),Y_COORDINATE(Y)}

const struct Button keypad[NO_ROWS][NO_COLUMNS] = {
   { // line 0
	   {COORDINATES(0,0), {19, 1, '1'}},  {COORDINATES(1,0), {19, 1, '2'}},  {COORDINATES(2,0), {19, 1, '3'}},  
	   INVALID_BUTTON,                    INVALID_BUTTON,                    INVALID_BUTTON,                    INVALID_BUTTON}, 
	 { // line 1
		 {COORDINATES(0,1), {19, 1, '4'}},  {COORDINATES(1,1), {19, 1, '5'}},  {COORDINATES(2,1), {19, 1, '6'}},  
	   INVALID_BUTTON,                    INVALID_BUTTON,                    INVALID_BUTTON,                    INVALID_BUTTON},
	 { // line 2
     {COORDINATES(0,2), {19, 1, '7'}},  {COORDINATES(1,2), {19, 1, '8'}},  {COORDINATES(2,2), {19, 1, '9'}},  
	   INVALID_BUTTON,                    INVALID_BUTTON,                    INVALID_BUTTON,                    INVALID_BUTTON},
	 { // line 3
		 {COORDINATES(0,3), {19, 1, '#'}},  {COORDINATES(1,3), {19, 1, '0'}},  {COORDINATES(2,3), {19, 1, ' '}}, 
	   INVALID_BUTTON,                    INVALID_BUTTON,                    INVALID_BUTTON,                    INVALID_BUTTON},
	 { // line 4
     {COORDINATES(0,4), {17, 0, '+'}},  {COORDINATES(1,4), {19, 1, '-'}},  {COORDINATES(2,4), {19, 1, 'd'}},  
	   INVALID_BUTTON,                    INVALID_BUTTON,                    INVALID_BUTTON,                    INVALID_BUTTON},
	 { // line 5
     {COORDINATES(0,5), {17, 2, '*'}},  {COORDINATES(1,5), {19, 1, '/'}},  {COORDINATES(2,5), {19, 1, 'r'}},  
	   {COORDINATES(3,5), {19, 1, 'e'}},  {COORDINATES(4,5), {19, 1, 'p'}},  {COORDINATES(5,5), {19, 1, 'P'}}, {COORDINATES(6,5), {19, 01, 'C'}}}	
}; 

/**
  * @brief  Diese Funktion liefert zu einer Koordinate den zugehörigen Button
  * @param  crd Koordinate
  * @retval Button der Koordinate
  */
static struct Button touchCoordinateToKey(Coordinate crd){
   int row = 0;
	 int col = 0;
	 const int distanceFromTheEdge = 8;
	 const struct Button invalidKey = INVALID_BUTTON; 
	
	 for (row = 0; row < NO_ROWS; row ++){
		 	for (col = 0; col < NO_COLUMNS; col ++){
			   POINT x = keypad[row][col].topLeft.x + distanceFromTheEdge;
				 POINT y = keypad[row][col].topLeft.y + distanceFromTheEdge;
				 if ((x <= crd.x) && (crd.x <= (x + BUTTON_WIDTH-distanceFromTheEdge)) && 
					   (y <= crd.y) && (crd.y <= (y + BUTTON_HEIGHT-distanceFromTheEdge))) {
					  return keypad[row][col];
				 }
			}	 
	 }
	 // No key found
	 return invalidKey;
}

/**
  * @brief  Diese Funktion stellt einen Button auf dem KeyPad dar
  * @param  key          Button, der dargestellt werden soll
  * @param  invertColors Vertauschte Vordergrund- und Hintergrundfarbe des Button
  * @retval none
  */
static void printButton(struct Button key, bool invertColors){
	if (!VALID_BUTTON(key)) return;
	COLOR buttonTextColor = (invertColors)? BACKGROUND_BUTTON : FOREGROUND_BUTTON;
	COLOR buttonBackColor = (invertColors)? FOREGROUND_BUTTON : BACKGROUND_BUTTON;
	const uint16_t buttonMarginWidth  = 4;
		
	// draw button and margin
	Coordinate tl = key.topLeft;
	Coordinate br = {key.topLeft.x + BUTTON_WIDTH, key.topLeft.y + BUTTON_HEIGHT};
	GUI_drawRectangle(tl, br, buttonTextColor, true, DOT_PIXEL_1X1);
	tl.x = key.topLeft.x + buttonMarginWidth;
  tl.y = key.topLeft.y + buttonMarginWidth;
	br.x = key.topLeft.x + BUTTON_WIDTH  - buttonMarginWidth;
	br.y = key.topLeft.y + BUTTON_HEIGHT - buttonMarginWidth;
	GUI_drawRectangle(tl, br, buttonBackColor, true, DOT_PIXEL_1X1);
	
	// draw top left corner
	tl = key.topLeft;
  GUI_drawPoint(tl,   BACKGROUND_DISPLAY, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
	tl.y = tl.y + 2;
  GUI_drawPoint(tl,   BACKGROUND_DISPLAY, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
	tl.y = tl.y - 2;
	tl.x = tl.x + 2;
  GUI_drawPoint(tl,   BACKGROUND_DISPLAY, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);

	// draw top right corner	
  tl.x = key.topLeft.x + BUTTON_WIDTH-2;
	tl.y = key.topLeft.y;
  GUI_drawPoint(tl,   BACKGROUND_DISPLAY, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
	tl.x = tl.x+1;
  GUI_drawPoint(tl,   BACKGROUND_DISPLAY, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
	tl.x = tl.x + 1;
	tl.y = tl.y + 2;
  GUI_drawPoint(tl,   BACKGROUND_DISPLAY, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
	
	// draw bottom left corner
	tl.x = key.topLeft.x;
	tl.y = key.topLeft.y + BUTTON_HEIGHT-2;
	GUI_drawPoint(tl,   BACKGROUND_DISPLAY, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
	tl.y = tl.y + 1;
	GUI_drawPoint(tl,   BACKGROUND_DISPLAY, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
	tl.y = tl.y + 1;
	tl.x = tl.x + 2;
	GUI_drawPoint(tl,   BACKGROUND_DISPLAY, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
	
	// draw bottom right corner
	tl.x = key.topLeft.x + BUTTON_WIDTH-2;
  tl.y = key.topLeft.y + BUTTON_HEIGHT;
	GUI_drawPoint(tl,   BACKGROUND_DISPLAY, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
	tl.y = tl.y - 1;
	tl.x = tl.x + 1;
	GUI_drawPoint(tl,   BACKGROUND_DISPLAY, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
	tl.y = tl.y - 1;
	tl.x = tl.x + 1;
	GUI_drawPoint(tl,   BACKGROUND_DISPLAY, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
	
	// draw character
	tl.x = (uint16_t)((int) key.topLeft.x + (int) buttonMarginWidth + (int) key.label.XOffset);
	tl.y = (uint16_t)((int) key.topLeft.y + (int) buttonMarginWidth + (int) key.label.YOffset);
	GUI_disChar(tl, key.label.c, &KEY_FONT, buttonBackColor, buttonTextColor);
}

Terminal makeKeyPad(void) {
	 GUI_clear(BACKGROUND_DISPLAY);
	 for (uint8_t row = 0; row < NO_ROWS; row++) {
		  for (uint8_t col = 0; col < NO_COLUMNS; col++){
			   if (VALID_BUTTON(keypad[row][col])){
				 	 printButton(keypad[row][col], false);
			   }
		  }		
 	 }
	 Coordinate tl = COORDINATES(3,0);
	 Coordinate br = {X_COORDINATE(6) + BUTTON_WIDTH,Y_COORDINATE(5) - GAP_BETWEEN_BUTTONS};
  	 Terminal term = genTerm(tl, br, BACKGROUND_TERMINAL, FOREGROUND_TERMINAL, &TERMINAL_FONT); 
   return term;
}

/**
  * @brief  Diese Funktion liefert einen gültigen Tastendruck zurück.
  * @retval gültiger Tastendruck
  */
static struct Button getValidButton(void){
	 struct Button pressedButton = INVALID_BUTTON;	
	 while(!VALID_BUTTON(pressedButton)) {
		  if (TP_pressed()) {
			    Coordinate crd = TP_get();
			    pressedButton = touchCoordinateToKey(crd);
		  }
	 }
	 return pressedButton;
}

/**
  * @brief  Zum Unterdrücken von Ausreißern liest diese Funktion
  *         mehrere gültige gedrückte Button ein und liefert den 
  *         am meisten gedrückten Button zurück. 
  * @retval Der am meisten gedrückte Button. Sind alle Button gleich
  *         oft gedrückt, ist der Rückgabewert INVALID_BUTTON;
  */
static struct Button selectButton(void){
	 const int noOfTries = 7;
	 struct Button buttons[noOfTries];
	 int count[noOfTries];
	 // Lese Button ein
	 for(int i = 0; i < noOfTries; i++){
		  buttons[i] = getValidButton();
		  count[i] = 1;
	 }
	 // Bestimme mehrfache gedrückte Buttons
	 for(int i = 0; i < noOfTries; i++){
		  for(int j = i+1; j < noOfTries; j++){
				 if(buttons[i].label.c == buttons[j].label.c) {
					  count[i] += 1;
					  count[j] = 0;
				 }
			}
	 }
	 // Bestimme Button, der am häufigsten gedrückt wurde
	 int maxPressed = 0;
	 for(int i = 0; i < noOfTries; i++){
			if (count[i] > count[maxPressed]) {
				 maxPressed = i;
			}
	 }
	 // Überprüfe, ob zwei Button gleichoft gedrückt wurde, setzte das Ergebnis auf invalid
	 for(int i = 0; i < noOfTries; i++){
			if ((count[i] == count[maxPressed]) && (i != maxPressed)) {
				 SET_INVALID(buttons[maxPressed]);
			}
	 }
	 return buttons[maxPressed];
 }

char getKeyPadInput(void){
	 struct Button button = INVALID_BUTTON;
	 // Lese Button ein
	 while(!VALID_BUTTON(button)){
		  button = selectButton();
	 }
	 printButton(button, true); 
	 
	 // Warte, bis waitCount Mal erkannt wurde, dass der Touch nicht gedrückt ist
	 const int waitCount = 4;
	 int i = 0;
	 while (i <= waitCount) {
		  if (!TP_pressed()) { i ++; }
	 }
	 printButton(button, false);
	 return button.label.c;
}

/**
  * @brief  Diese Funktion führt einen einfachen Test des 
  *         KeyPad aus.
  * @retval none
  */
void testKeyPad(void) {
	 Terminal term = makeKeyPad();
   char buf[60];
	 while (1) {
		  char k = getKeyPadInput();
	    Coordinate crd = TP_get();
		  if (k == 0) {
		     printTerm("Unexpected result", &term);
	    } else {
			   // find buttom 
			   struct Button bot = INVALID_BUTTON;
			   for (int i = 0; i < NO_ROWS; i++) {
				    for (int j = 0; j < NO_COLUMNS; j++) {
					     if (keypad[i][j].label.c == k) {
						  	  bot = keypad[i][j];
						   }
					  }
				 }
				 if (!VALID_BUTTON(bot)){
					  sprintf(buf, "Fehler: Taste nicht gefunden.\n");
			      printTerm(buf, &term);
				 } else {
					  snprintf(buf, sizeof(buf), "Taste = %c\n", k);
			      printTerm(buf, &term);
			      snprintf(buf, sizeof(buf), "%d <= %d (x) <= %d\n", bot.topLeft.x, crd.x, bot.topLeft.x + BUTTON_WIDTH);
			      printTerm(buf, &term);					
	          snprintf(buf, sizeof(buf), "%d <= %d (y) <= %d\n", bot.topLeft.y, crd.y, bot.topLeft.y + BUTTON_HEIGHT);
			      printTerm(buf, &term);
		     }
			}
	 }
}

// EOF
