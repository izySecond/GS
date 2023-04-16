/**
 ******************************************************************************
 * @file    lcd.c
 * @author  Silke Behn
 *          HAW-Hamburg
 *          Labor für technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.1
 *
 * @date    16. Juli 2020
 * @brief   Funktionen zum ansteuern des Waveshare 4 inch 480*320 Resistive Touch TFT
 *          09.12.2020 lcdmaxy/lcdmaxx vom aktuellen Font abhängig,  lcdGotoXY ->Berechnung mit Fontdaten gekoppelt
 ******************************************************************************
 */
#include "lcd.h"
#include "LCD_GUI.h"
#include "string.h"
#include "fontsFLASH.h"

#include <stdio.h>
#include "LCD_general.h"

#define LCDXSTART 10            //x Start der Textausgabe
#define LCDYSTART 10            //y Start der Textausgabe
#define STARTFONTWIDTH  11
#define STARTFONTHEIGHT 16


//Waveshare 4 inch 480*320

static Coordinate crd = {LCDXSTART,LCDYSTART};
static COLOR  lcdColor = BLACK;
static sFONT* lcdFont  = &Font16FLASH; 
static int fontWidth = STARTFONTWIDTH;
static int fontHeight = STARTFONTHEIGHT;
static int lcdmaxy = LCD_HEIGHT - STARTFONTHEIGHT;
static int lcdmaxx = LCD_WIDTH - STARTFONTWIDTH;

static void stringCopy(char* quelle, char* ziel, int von, int bis){
	 int j=0;
	 for (int i= von; i<bis; i++){ 	
		  ziel[j] = quelle[i];
		  j++;
	 }
	 ziel[j] = '\0';
}

/*---------------------------------------------
 * txt  Ausgabestring
 * --------------------------------------------
 * Gibt den Ausgabestring auf dem LCD aus
 * Dabei wird zuerst alles rechts vom Curor geloescht
 * --------------------------------------------*/
void lcdPrintS(char txt[]){
	  int len = strlen(txt);  //Gesamtlaenge vom auszugebenen Text
	  int anz = 0;
	  int maxZeichen = lcdmaxx / fontWidth;
	  char txtTeil[maxZeichen+1]; 
	  Coordinate br = {0,0};  //Button right 
	
	  //Text in Zeilen aufsplitten und Zeilenweise ausgeben
	  while ( len > 0 ){ 
			
			anz = (lcdmaxx - crd.x) / fontWidth;   //Wie viel Zeichen noch in die Zeile passen  
			if ( anz > len ) {
				  anz = len;
			}
            stringCopy(txt, txtTeil, 0, anz);     //diese Zeichen in txtTeil kopieren
			br.x = lcdmaxx;
		    br.y = crd.y+fontHeight;
			GUI_drawRectangle(crd, br, WHITE, true , DOT_PIXEL_1X1 ); //Alte Zeichen löschen
			GUI_disStr(crd, txtTeil , lcdFont, LCD_BACKGROUND, lcdColor ); //Zeichen ausgeben
			
			//Naechste Zeile vorbereiten
			crd.x = crd.x + anz * fontWidth;
			if ( crd.x+fontWidth > lcdmaxx){
			   crd.x = LCDXSTART;
			   crd.y = crd.y + fontHeight;
				 if ( crd.y > lcdmaxy ){
				     crd.y = LCDYSTART;
			   }
			}
			txt = txt + anz;
	        len = strlen(txt);
			
	  } 	
}

/*---------------------------------------------
 * txt  Ausgabestring
 * --------------------------------------------
 * Gibt den Ausgabestring auf dem LCD aus
 * Dabei werden nur die Zeichen ersetzt, die geschrieben werden
 * --------------------------------------------*/
void lcdPrintReplS(char txt[]){
	  int len = strlen(txt);  //Gesamtlaenge vom auszugebenen Text
	  int anz = 0;
	  int maxZeichen = lcdmaxx / fontWidth;
	  char txtTeil[maxZeichen+1]; 
	  Coordinate br = {0,0};  //Button right 
	
	  //Text in Zeilen aufsplitten und Zeilenweise ausgeben
	  while ( len > 0 ){ 
			
			anz = (lcdmaxx - crd.x) / fontWidth;   //Wie viel Zeichen noch in die Zeile passen  
			if ( anz > len ) {
				  anz = len;
			}
      stringCopy(txt, txtTeil, 0, anz);     //diese Zeichen in txtTeil kopieren
			br.x = crd.x + anz*fontWidth;
		  br.y = crd.y+fontHeight;
			GUI_drawRectangle(crd, br, WHITE, true , DOT_PIXEL_1X1 ); //Alte Zeichen löschen
			GUI_disStr(crd, txtTeil , lcdFont, LCD_BACKGROUND, lcdColor ); //Zeichen ausgeben
			
			//Naechste Zeile vorbereiten
			crd.x = crd.x + anz * fontWidth;
			if ( crd.x+fontWidth > lcdmaxx){
			   crd.x = LCDXSTART;
			   crd.y = crd.y + fontHeight;
				 if ( crd.y > lcdmaxy ){
				     crd.y = LCDYSTART;
			   }
			}
			txt = txt + anz;
	    len = strlen(txt);
			
		} 	
}




/*---------------------------------------------
 * zahl  Zahl, die auf dem Display ausgegeben 
 *       werden soll.
 * --------------------------------------------
 * Gibt eine Zahl auf dem Display aus.
 * --------------------------------------------*/
void lcdPrintInt(int zahl){
	 char txt[12];
	 sprintf( txt, "%d", zahl);
	 lcdPrintS(txt);
}


/*---------------------------------------------
 * txt  Ausgabestring
 * --------------------------------------------
 * Gibt den Ausgabestring auf dem LCD aus
 * Anschließend geht der Cursor in die neue Zeile
 * --------------------------------------------*/
void lcdPrintlnS(char txt[]){
	 lcdPrintS(txt);
	 crd.y = crd.y + fontHeight;
	 if ( crd.y >= lcdmaxy ){
		  crd.y = LCDYSTART;
	 }
	 crd.x = LCDXSTART;
}

/*---------------------------------------------
 * c  Ausgabe Zeichen
 * --------------------------------------------
 * Gibt ein Zeichen auf dem LCD aus
 * --------------------------------------------*/
void lcdPrintC(char c){
	  
	  Coordinate br = {0,0};  //Button right 
	
		br.x = crd.x + fontWidth;
	  br.y = crd.y + fontHeight;
		GUI_drawRectangle(crd, br, WHITE, true , DOT_PIXEL_1X1 ); //Alte Zeichen löschen
		GUI_disChar(crd, c, lcdFont, LCD_BACKGROUND, lcdColor);
		
		//Naechste Zeile vorbereiten
		crd.x = crd.x + fontWidth;
		if ( crd.x+fontWidth > lcdmaxx){
			  crd.x = LCDXSTART;
			  crd.y = crd.y + fontHeight;
				if ( crd.y > lcdmaxy ){
				    crd.y = LCDYSTART;
			  }
		}
}		 	


/*---------------------------------------------
 * font  Fonthöhe erlaubt sind (8,12,16,20,24)
 * --------------------------------------------
 * stellt einen neuen font ein
 * --------------------------------------------*/
void lcdSetFont(int font){
	  switch (font) {
			case 8:   lcdFont  = &Font8FLASH; 
	              break;
			
			case 12:	lcdFont  = &Font12FLASH; 
	              break;
			
			case 16:  lcdFont  = &Font16FLASH; 
	              break;
			
			case 20:  lcdFont  = &Font20FLASH; 
	              break;
				
			case 24:  lcdFont  = &Font24FLASH; 
	              break;
			default:  printf("Die Fonthoehe ist nicht erlaubt. Gueltige Werte sind 8, 12, 16, 20, 24\n");					
		}
		fontWidth = lcdFont->Width;
    fontHeight = lcdFont->Height;
		lcdmaxy = LCD_HEIGHT - lcdFont->Height;
    lcdmaxx = LCD_WIDTH  - lcdFont->Width;
}

void lcdGotoXY( POINT x, POINT y){
	  crd.x = x*fontWidth;
	  crd.y = y*fontHeight;
	
}

// EOF
