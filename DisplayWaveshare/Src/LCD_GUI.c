/*****************************************************************************
* | File      	:	LCD_GUI.c
* | Author      :   Waveshare team
* | Function    :	Achieve drawing: draw points, lines, boxes, circles and
*                   their size, solid dotted line, solid rectangle hollow
*					rectangle, solid circle hollow circle.
* | Info        :
*   Achieve display characters: Display a single character, string, number
*   Achieve time display: adaptive size display time minutes and seconds
*----------------
* |	This version:   V1.0
* | Date        :   2017-08-16
* | Info        :   Basic version
*
******************************************************************************/
#include "LCD_GUI.h"
#include <stdio.h>
#include "../IncLocal/LCD_Driver.h"
#include "../IncLocal/LCD_Tools.h"
#include "fontsFLASH.h"
#include "LCD_Debug.h"

void GUI_init(uint16_t blVal){
	 LCD_init(blVal);
	 LCD_clear(LCD_BACKGROUND);
}

void GUI_clear(COLOR Color){
   LCD_clear(Color);
}

void GUI_drawPoint(Coordinate crd, COLOR color, DOT_PIXEL dotPixel, DOT_STYLE dotStyle){
	 Coordinate tl = crd;
	 LENGTH width  = dotPixel;
	 LENGTH height = dotPixel;
	
   if(!VALID_COORDINATE(crd)){
		  DEBUG("GUI_drawPoint Input exceeds the normal display range\n");
      return;
	 }
  if (DOT_FILL_AROUND == dotStyle) {
		 tl.x = (crd.x >= (dotPixel-1)) ? (crd.x - (dotPixel-1)) : 0;
		 tl.y = (crd.y >= (dotPixel-1)) ? (crd.y - (dotPixel-1)) : 0;
     width  = crd.x - tl.x + dotPixel;
		 height = crd.y - tl.y + dotPixel;
	 }
	 LCD_fillRectangleMC (tl, width,	height, color);
}

void GUI_drawRectangle(Coordinate tl, Coordinate br, COLOR color, bool filled, DOT_PIXEL lineWeight){
	 if(!VALID_COORDINATE(tl) || !VALID_COORDINATE(br) || (tl.x > br.x) || (tl.y > br.y)){
		  DEBUG("GUI_drawRectangle Input exceeds the normal display range\n");
      return;
	 }
	 if (filled || ((tl.x + 2*lineWeight - 1) >= br.x) || ((tl.y + 2*lineWeight - 1) >= br.y))  {
		  // rectangle filled due to parameter filled or due to line weight
		  LCD_fillRectangleMC(tl, br.x -  tl.x + 1, br.y - tl.y + 1, color);
	 } else {
		  Coordinate crd;
		  LENGTH weight = br.x -  tl.x + 1;                             // top line
		  LENGTH height = lineWeight;		 
      LCD_fillRectangleMC(tl, weight, height, color);               // top line
		  crd.x = tl.x; crd.y = br.y - lineWeight + 1;                  // bottom line
      LCD_fillRectangleMC(crd, weight, height, color);
		  weight = lineWeight;                                          // left line
		  height = br.y -  tl.y - 2 * lineWeight + 1;		 
			crd.x = tl.x;  crd.y = tl.y + lineWeight;                     
      LCD_fillRectangleMC(crd, weight, height, color);
		 	crd.x = br.x - lineWeight + 1;  crd.y = tl.y + lineWeight;    // reight line                  
      LCD_fillRectangleMC(crd, weight, height, color);		 
	 }
}

void GUI_disChar(Coordinate tl, char c, sFONT* font, COLOR bg, COLOR fg){
	 if(!VALID_COORDINATE(tl)){
		  DEBUG("GUI_disChar Input exceeds the normal display range\n");
      return;
	 }
	 if ((c < ' ') || (c > '~')) {
			//Character out of range
      DEBUG("GUI_disChar character out of font.\n");
			c = ' ';
   }
	 
	 // UNSCHOEN, zu Fonts auslagern 
	 uint32_t offset = (c - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
	 LCD_fillRectangleBW (tl, font->Width, font->Height, bg, fg, font->table + offset);
 }

void GUI_disStr(Coordinate tl, const char *s, sFONT *font, COLOR bg, COLOR fg){
   if(!VALID_COORDINATE(tl)) {
       DEBUG("GUI_disStr Input exceeds the normal display range\n");
       return;
   }
   while(*s != '\0') {
	    bool nlRequired = ((tl.y + font->Height - 1) >= LCD_HEIGHT) || 
			                  ((tl.x + font->Width  - 1) >= LCD_WIDTH);
			if ((*s == '\n') || nlRequired) {
				 tl.x = 0;
				 tl.y = ((tl.y + 2*font->Height - 1) < LCD_HEIGHT) ? tl.y + font->Height : 0;
			}
			if (*s != '\n') {
			  GUI_disChar(tl, *s, font, bg, fg);
			  tl.x = tl.x + font->Width;
			}
			s++;	
   }
}

void GUI_disNum(Coordinate tl, int32_t no, sFONT* font, COLOR bg, COLOR fg) {
   char buf[32];
   if (!VALID_COORDINATE(tl)) {
      DEBUG("GUI_disNum Input exceeds the normal display range\n");
      return;
   }
   itoa(no,buf); 
	 GUI_disStr(tl, buf, font, bg, fg);
}

void GUI_drawCircle(Coordinate center, LENGTH radius, COLOR color, bool filled , DOT_PIXEL lineWeight){
   Coordinate crd;
	 if(!VALID_COORDINATE(center)) {
      DEBUG("GUI_drawCircle Input exceeds the normal display range\n");
      return;
	 }
	 
	 if (filled) { 
		  lineWeight = DOT_PIXEL_1X1;
	 }

   //Draw a circle from(0, R) as a starting point
   int16_t XCurrent = 0;
   int16_t YCurrent = radius;

   //Cumulative error,judge the next point of the logo
   int16_t Esp = 3 - (radius << 1 );

   int16_t sCountY;
   while(XCurrent <= YCurrent ) { //Realistic circles
	    for(sCountY = (filled) ? XCurrent : YCurrent; sCountY <= YCurrent; sCountY ++ ) {
				 crd.x = center.x + XCurrent; crd.y = center.y + sCountY;  if(VALID_COORDINATE(crd)){ GUI_drawPoint(crd, color, lineWeight, DOT_FILL_AROUND);}//1
         crd.x = center.x - XCurrent;                              if(VALID_COORDINATE(crd)){ GUI_drawPoint(crd, color, lineWeight, DOT_FILL_AROUND);}//2
         crd.x = center.x - sCountY;  crd.y = center.y + XCurrent; if(VALID_COORDINATE(crd)){ GUI_drawPoint(crd, color, lineWeight, DOT_FILL_AROUND);}//3
				                              crd.y = center.y - XCurrent; if(VALID_COORDINATE(crd)){ GUI_drawPoint(crd, color, lineWeight, DOT_FILL_AROUND);}//4
         crd.x = center.x - XCurrent; crd.y = center.y - sCountY;  if(VALID_COORDINATE(crd)){ GUI_drawPoint(crd, color, lineWeight, DOT_FILL_AROUND);}//5
         crd.x = center.x + XCurrent;                              if(VALID_COORDINATE(crd)){ GUI_drawPoint(crd, color, lineWeight, DOT_FILL_AROUND);}//6
         crd.x = center.x + sCountY;  crd.y = center.y - XCurrent; if(VALID_COORDINATE(crd)){ GUI_drawPoint(crd, color, lineWeight, DOT_FILL_AROUND);}//7
                                      crd.y = center.y + XCurrent; if(VALID_COORDINATE(crd)){ GUI_drawPoint(crd, color, lineWeight, DOT_FILL_AROUND);}//0
      }
      if(Esp < 0 ){
         Esp += 4 * XCurrent + 6;
      } else {
         Esp += 10 + 4 * (XCurrent - YCurrent );
         YCurrent --;
      }
      XCurrent ++;
   }
}


static bool GUI_drawHorVarLine(Coordinate tl, Coordinate br, COLOR fg, COLOR bg, bool solid, DOT_PIXEL lineWidth){
   if (!VALID_COORDINATE(tl) || !VALID_COORDINATE(br) || (((tl.x != br.x) && (tl.y != br.y)))) {
      return false; // nothing printed
   }
	 // dot, horizontal - or vertical line
	 // If tl is not at the top right of br, swap tl and br.
	 if ((tl.x > br.x) || (tl.y > br.y)){
		  Coordinate tmp = tl;
		  tl = br;
		  br = tmp;
	 }
	 // tl is placed on the left above br. 
	 //  Calculate top left corner, height and width adjusted by line width and edge of the screen
	 Coordinate tlCorner; 
   tlCorner.x = (tl.x >= (lineWidth-1)) ? (tl.x - (lineWidth-1)) : 0;
	 tlCorner.y = (tl.y >= (lineWidth-1)) ? (tl.y - (lineWidth-1)) : 0;
	 LENGTH width  = tl.x - tlCorner.x + lineWidth + (br.x - tl.x);
	 LENGTH height = tl.y - tlCorner.y + lineWidth + (br.y - tl.y);
	 
	 // Draw rectangle based on tlCorner, height and width
	 if (solid) {
		  LCD_fillRectangleMC (tlCorner, width,	height, fg);
	 } else {
		 // toogle: 2 Dots filled, one Dot blanked
		 LENGTH len = 2* lineWidth; // Length of the segment currently to be drawn
		 COLOR  col = fg;           // Color of the segment currently to be drawn
		 while ((width > 0) && (height > 0)){
			  if (tl.y == br.y) { // horizontal line
					 len = (width > len) ? len : width;
					 LCD_fillRectangleMC (tlCorner, len ,	height, col);
					 tlCorner.x += len;
					 width = (width > len) ? width - len : 0;
				} else { // vertical line
					len = (height > len) ? len : height;					
					 LCD_fillRectangleMC (tlCorner, width,	len, col);
					 tlCorner.y += len;
					 height = (height > len) ? height - len : 0;					
			  }
			  col = (col == fg) ? bg : fg;
				len = (len == lineWidth) ? 2 * lineWidth : lineWidth;
		 }
	 }
	 return true; // line printed
}
	
void GUI_drawLine(Coordinate left, Coordinate right, COLOR fg, COLOR bg, bool solid, DOT_PIXEL lineWidth){
	 if (GUI_drawHorVarLine(left, right, fg, bg, solid, lineWidth)){
		  // dot, vertical or horizontal line printed
		  return;
	 }
	 if (!VALID_COORDINATE(left) || !VALID_COORDINATE(right)){ 
      DEBUG("GUI_drawLine Input exceeds the normal display range\n");
      return;
   }
	 if (!solid){
      DEBUG("GUI_drawLine Dotted lines not support for arbitray slope\n");
      return;		  
	 }
	 // draw points from left to right
	 if (left.x > right.x) {
		  Coordinate tmp = left;
		  left = right;
		  right = tmp;
	 }
	 
   POINT Xpoint = left.x;
   POINT Ypoint = left.y;
	 
   int32_t dx = right.x >= left.x ? right.x - left.x : left.x - right.x;
	 int32_t dy = right.y >= left.y ? left.y - right.y :right.y - left.y;

   // Increment direction, 1 is positive, -1 is counter;
   int32_t XAddway = left.x < right.x ? 1 : -1;
   int32_t YAddway = left.y < right.y ? 1 : -1;

   //Cumulative error
   int32_t Esp = dx + dy;

	 GUI_drawPoint(left, fg, lineWidth, DOT_FILL_AROUND);
   for(;;) {
      //Painted dotted line, 2 point is really virtual
			Coordinate crd = {Xpoint, Ypoint};
			// dotted : 8 point bg ; 8 points fg
			GUI_drawPoint(crd, fg, lineWidth, DOT_FILL_AROUND);
      if (2 * Esp >= dy) {
         if (Xpoint == right.x) break;
         Esp += dy;
         Xpoint += XAddway;
      }
      if(2 * Esp <= dx) {
         if(Ypoint == right.y) break;
         Esp += dx;
         Ypoint += YAddway;
      }
   }
	 GUI_drawPoint(right, fg, lineWidth, DOT_FILL_AROUND);
}

/******************************************************************************
function:	Display the Gray map,1 byte = 8bit = 2 points
parameter:
	Xpoint ：X coordinate
	Ypoint : Y coordinate
	pMap   : Pointing to the picture
	Width  ：Bitmap Width
	Height : Bitmap Height
note:
	This function is suitable for bitmap, because a 4-bit data accounted for 1 points
	Please use the Image2lcd generated array
******************************************************************************/
void GUI_DisGrayMap(POINT Xpoint, POINT Ypoint, const unsigned char *pBmp)
{
    //Get the Map header Gray, width, height
    char Gray;
    Gray = *(pBmp + 1);
    POINT Height, Width;
    Width = (*(pBmp + 3) << 8) | (*(pBmp + 2));
    Height = (*(pBmp + 5) << 8) | (*(pBmp + 4));

	  Coordinate crd;
    POINT i, j;
    if(Gray == 0x04) { //Sixteen gray levels
        pBmp = pBmp + 6;
        for(j = 0; j < Height; j++)
            for(i = 0; i < Width / 2; i++) {
							  crd.y = Ypoint + j;
                crd.x = Xpoint + i * 2;     GUI_drawPoint(crd, ~(*pBmp >> 4), DOT_PIXEL_1X1, DOT_FILL_AROUND);
                crd.x = Xpoint + i * 2 + 1; GUI_drawPoint(crd, ~*pBmp , DOT_PIXEL_1X1, DOT_FILL_AROUND);
                pBmp++;
            }
    } else {
        DEBUG("Does not support type\n");
        return;
    }
}
// EOF
