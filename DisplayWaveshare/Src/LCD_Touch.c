/*****************************************************************************
* | File      	:	LCD_Touch.c
* | Author      :   Waveshare team
* | Function    :	LCD Touch Pad Driver and Draw
* | Info        :
*   Image scanning
*      Please use progressive scanning to generate images or fonts
*----------------
* |	This version:   V1.0
* | Date        :   2017-08-16
* | Info        :   Basic version
*
******************************************************************************/
#include "LCD_Touch.h"
#include "../IncLocal/LCD_Driver.h"
#include "../IncLocal/LCD_Tools.h"
#include "../IncLocal/LCD_SPI.h"
#include "LCD_GUI.h"
#include "LCD_Debug.h"
#include "fontsFLASH.h"

#include "../IncLocal/LCD_GPIO.h"

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define TP_PRESS_DOWN           0x80     // Touch pressed down during last scan

#define TP_CS_0			HAL_GPIO_WritePin(TP_CS_GPIO_Port, TP_CS_Pin, GPIO_PIN_RESET)
#define TP_CS_1			HAL_GPIO_WritePin(TP_CS_GPIO_Port, TP_CS_Pin, GPIO_PIN_SET)

#define GET_TP_IRQ	HAL_GPIO_ReadPin(TP_IRQ_GPIO_Port, TP_IRQ_Pin)

//Touch screen structure
typedef struct {
	POINT XpointRaw;  // Without correction based on the calibration 
	POINT YpointRaw;  // Without correction based on the calibration 
	Coordinate calibratedTouch; // Calibrated touch; if	deviation recognized TP_Read_TwiceADC is to high: Undefined Coordinate
	int16_t iXoff;
	int16_t iYoff;
	float fXfac;
	float fYfac;
  uint8_t chStatus;
}TP_DEV;

static TP_DEV sTP_DEV;

// Bit MAP CROSS 
const uint8_t Cross[] = {
//                       Column
//                     1          2          3
//            01234567 89012345 67890123 45678901
//ROW  0      00000000 00001000 00000000 00000000   // (0,12)
              0x00,    0x08,    0x00,    0x00,
//ROW  1      00000000 00001000 00000000 00000000   // (1,12)
              0x00,    0x08,    0x00,    0x00,
//ROW  2      00000000 00001000 00000000 00000000   // (2,12)
              0x00,    0x08,    0x00,    0x00,
//ROW  3      00000000 00001000 00000000 00000000   // (3,12)
              0x00,    0x08,    0x00,    0x00,
//ROW  4      00000000 00001000 00000000 00000000   // (4,12)
              0x00,    0x08,    0x00,    0x00,
//ROW  5      00000000 00001000 00000000 00000000   // (5,12)
              0x00,    0x08,    0x00,    0x00,
//ROW  6      00000000 00111110 00000000 00000000   // (6,10) (6,11) (6,12) (6,13) (6,14)
              0x00,    0x3E,    0x00,    0x00,
//ROW  7      00000000 01001001 00000000 00000000   // (7,9) (7,12) (7,15)
              0x00,    0x49,    0x00,    0x00,
//ROW  8      00000000 10001000 10000000 00000000   // (8,8) (8,12) (8,16)
              0x00,    0x88,    0x80,    0x00,
//ROW  9      00000001 00001000 01000000 00000000   // (9,7) (9,12) (9,17)
              0x01,    0x08,    0x40,    0x00,
//ROW  10     00000010 00001000 00100000 00000000   // (10,6) (10,12) (10,18)
              0x02,    0x08,    0x20,    0x00,
//ROW  11     00000010 0001 1100 00100000 00000000   // (11,6) (11,11) (11,12) (11,13) (11,18)
              0x02,    0x1C,    0x20,    0x00,
//ROW  12     11111111 11111111 11111111 10000000   // (12,0) to (12,24)
              0xFF,    0x0FF,    0xFF,    0x80,
//ROW  13     00000010 00011100 00100000 00000000   // (13,6) (13,11) (13,12) (13,13) (13,18)
              0x02,    0x1C,    0x20,    0x00,
//ROW  14     00000010 00001000 00100000 00000000   // (14,6) (14,12) (14,18)
              0x02,    0x08,    0x20,    0x00,
//ROW  15     00000001 00001000 01000000 00000000   // (15,7) (15,12) (15,17)
              0x01,    0x08,    0x40,    0x00,
//ROW  16     00000000 10001000 10000000 00000000   // (16,8) (16,12) (16,16)
              0x00,    0x88,    0x80,    0x00,
//ROW  17     00000000 01001001 00000000 00000000   // (17,9) (17,12) (17,15)
              0x00,    0x49,    0x00,    0x00,
//ROW  18     00000000 00111110 00000000 00000000   // (18,10) (18,11) (18,12) (18,13) (18,14)
              0x00,    0x3E,    0x00,    0x00,
//ROW  19     00000000 00001000 00000000 00000000   // (19,12)
              0x00,    0x08,    0x00,    0x00,
//ROW  20     00000000 00001000 00000000 00000000   // (20,12)
              0x00,    0x08,    0x00,    0x00,
//ROW  21     00000000 00001000 00000000 00000000   // (21,12)
              0x00,    0x08,    0x00,    0x00,
//ROW  22     00000000 00001000 00000000 00000000   // (22,12)
              0x00,    0x08,    0x00,    0x00,
//ROW  23     00000000 00001000 00000000 00000000   // (23,12)
              0x00,    0x08,    0x00,    0x00,
//ROW  24     00000000 00001000 00000000 00000000   // (24,12)
              0x00,    0x08,    0x00,    0x00
};

/*******************************************************************************
function:
		Read the ADC of the channel
    SPI Bus speed must be set to the Speed of Touch

parameter:
	Channel_Cmd :	0x90: Read channel Y +, select the ADC resolution is 12 bits, set to differential mode
					0xd0: Read channel x +, select the ADC resolution is 12 bits, set to differential mode
*******************************************************************************/

static uint16_t TP_Read_ADC(uint8_t CMD) {
  uint16_t Data = 0;
	
	// SPIsetTPSpeed();	
	TP_CS_0;

	SPI4W_Write_Byte(CMD);
	Driver_Delay_us(200);
	
	Data = SPI4W_Read_Byte();
	Data <<= 8;//7bit
	
	Data |= SPI4W_Read_Byte();
	UPDATE_TP_CMD(CMD, Data);
	Data >>= 3;//5bit
	TP_CS_1;

	// SPIsetLCDSpeed();	
  return Data;
}

/*******************************************************************************
function:
		Read the 5th channel value and exclude the maximum and minimum returns the average
    SPI Bus speed must be set to the Speed of Touch

parameter:
	Channel_Cmd :	0x90 :Read channel Y +
					0xd0 :Read channel x +
*******************************************************************************/
#define READ_TIMES  5	//Number of readings
#define LOST_NUM    1	//Discard value
static uint16_t TP_Read_ADC_Average(uint8_t Channel_Cmd)
{
    uint8_t i, j;
    uint16_t Read_Buff[READ_TIMES];
    uint16_t Read_Sum = 0, Read_Temp = 0;

    //Read and save multiple samples
    for(i = 0; i < READ_TIMES; i++){
		Read_Buff[i] = TP_Read_ADC(Channel_Cmd);
		Driver_Delay_us(200);
	}
	
    //Sort from small to large
    for (i = 0; i < READ_TIMES  -  1; i ++) {
        for (j = i + 1; j < READ_TIMES; j ++) {
            if (Read_Buff[i] > Read_Buff[j]) {
                Read_Temp = Read_Buff[i];
                Read_Buff[i] = Read_Buff[j];
                Read_Buff[j] = Read_Temp;
            }
        }
    }

    //Exclude the largest and the smallest
    for (i = LOST_NUM; i < READ_TIMES - LOST_NUM; i ++)
        Read_Sum += Read_Buff[i];

    //Averaging
    Read_Temp = Read_Sum / (READ_TIMES - 2 * LOST_NUM);

    return Read_Temp;
}

/*******************************************************************************
function:
		Read X channel and Y channel AD value
    SPI Bus speed must be set to the Speed of Touch
parameter:
	Channel_Cmd :	0x90 :Read channel Y +
					0xd0 :Read channel x +
*******************************************************************************/
static void TP_Read_ADC_XY(uint16_t *pXCh_Adc, uint16_t  *pYCh_Adc )
{
    *pXCh_Adc = TP_Read_ADC_Average(0xD0);
    *pYCh_Adc = TP_Read_ADC_Average(0x90);
}

/*******************************************************************************
function:
		2 times to read the touch screen IC, and the two can not exceed the deviation,
		ERR_RANGE, meet the conditions, then that the correct reading, otherwise the reading error.
parameter:
	Channel_Cmd :	pYCh_Adc = 0x90 :Read channel Y +
					pXCh_Adc = 0xd0 :Read channel x +
*******************************************************************************/
#define ERR_RANGE 50	//tolerance scope
static bool TP_Read_TwiceADC(uint16_t *pXCh_Adc, uint16_t  *pYCh_Adc ) {
    uint16_t XCh_Adc1, YCh_Adc1, XCh_Adc2, YCh_Adc2;
	  
	  SPIsetTPSpeed();	
    //Read the ADC values Read the ADC values twice
    TP_Read_ADC_XY(&XCh_Adc1, &YCh_Adc1);
  	Driver_Delay_us(10);
    TP_Read_ADC_XY(&XCh_Adc2, &YCh_Adc2);
	  SPIsetLCDSpeed();	
	  *pXCh_Adc = (XCh_Adc1 + XCh_Adc2) / 2;
    *pYCh_Adc = (YCh_Adc1 + YCh_Adc2) / 2;
	
    //The ADC error used twice is greater than ERR_RANGE to take the average
	  return     ((XCh_Adc2 <= XCh_Adc1 && XCh_Adc1 < XCh_Adc2 + ERR_RANGE) ||
               (XCh_Adc1 <= XCh_Adc2 && XCh_Adc2 < XCh_Adc1 + ERR_RANGE))
           && ((YCh_Adc2 <= YCh_Adc1 && YCh_Adc1 < YCh_Adc2 + ERR_RANGE)  ||
               (YCh_Adc1 <= YCh_Adc2 && YCh_Adc2 < YCh_Adc1 + ERR_RANGE));
}

/*******************************************************************************
function:
		Calculation

return value: 
					Touched pressed
*******************************************************************************/
static bool TP_Scan(void) {
	  const LENGTH maxCol = LCD_WIDTH - 1;
	  const LENGTH maxRow = LCD_HEIGHT - 1;
    //In X, Y coordinate measurement, IRQ is disabled and output is low
	  if (GET_TP_IRQ) {
			 // Touch not pressed 
			 sTP_DEV.chStatus &= ~TP_PRESS_DOWN;
			 return false;
	  }
		// touch pressed
	  sTP_DEV.chStatus |= TP_PRESS_DOWN;
		if (TP_Read_TwiceADC(&sTP_DEV.XpointRaw, &sTP_DEV.YpointRaw)) {
       // Converts the result to screen coordinates
			if (CURRENT_SCAN_DIR == R2L_D2U) {
         sTP_DEV.calibratedTouch.x = sTP_DEV.fXfac * sTP_DEV.XpointRaw + sTP_DEV.iXoff;
         sTP_DEV.calibratedTouch.y = sTP_DEV.fYfac * sTP_DEV.YpointRaw + sTP_DEV.iYoff;
      } else if(CURRENT_SCAN_DIR == L2R_U2D) {
         sTP_DEV.calibratedTouch.x = maxCol - sTP_DEV.fXfac * sTP_DEV.XpointRaw - sTP_DEV.iXoff;
         sTP_DEV.calibratedTouch.y = maxRow - sTP_DEV.fYfac * sTP_DEV.YpointRaw - sTP_DEV.iYoff;
      } else if(CURRENT_SCAN_DIR == U2D_R2L) {
         sTP_DEV.calibratedTouch.x = sTP_DEV.fXfac * sTP_DEV.YpointRaw + sTP_DEV.iXoff;
         sTP_DEV.calibratedTouch.y = sTP_DEV.fYfac * sTP_DEV.XpointRaw + sTP_DEV.iYoff;
      } else {
         sTP_DEV.calibratedTouch.x = maxCol - sTP_DEV.fXfac * sTP_DEV.YpointRaw - sTP_DEV.iXoff;
         sTP_DEV.calibratedTouch.y = maxRow - sTP_DEV.fYfac * sTP_DEV.XpointRaw - sTP_DEV.iYoff;
      }
		} else {
			// invalid touch according to the deviations recognized by TP_Read_TwiceADC
			sTP_DEV.calibratedTouch.x = INVALID_POINT;
			sTP_DEV.calibratedTouch.y = INVALID_POINT;
      sTP_DEV.XpointRaw = INVALID_POINT;
      sTP_DEV.YpointRaw = INVALID_POINT;
		}
		return true;
}

/*******************************************************************************
function:
		Draw Cross
parameter:
			Xpoint :	The x coordinate of the point
			Ypoint :	The y coordinate of the point
			Color  :	Set color
*******************************************************************************/
static void TP_DrawCross(Coordinate crd, COLOR Color){
	 crd.x -= 12;
	 crd.y -= 12;
	 LCD_fillRectangleBW (crd, 25,	25, LCD_BACKGROUND, Color, Cross);
}

/*******************************************************************************
function:
		The corresponding ADC value is displayed on the LC
parameter:
			(Xpoint0 ,Xpoint0):	The coordinates of the first point
			(Xpoint1 ,Xpoint1):	The coordinates of the second point
			(Xpoint2 ,Xpoint2):	The coordinates of the third point
			(Xpoint3 ,Xpoint3):	The coordinates of the fourth point
			hwFac	:	Percentage of error
*******************************************************************************/
static void TP_ShowInfo(Coordinate topLeft,
                        Coordinate topRight,
                        Coordinate bottomLeft,
                        Coordinate bottomRight) {
    sFONT* TP_Font = &Font16FLASH;
    LENGTH TP_Dx = 	TP_Font->Width;

	  Coordinate tl = {40, 160};
	  Coordinate br = {250, 270};
	
		GUI_drawRectangle(tl, br, WHITE, true, DOT_PIXEL_1X1);
		
		tl.x = 40;   tl.y = 160; GUI_disStr(tl, "x1",        TP_Font, FONT_BACKGROUND, RED);
		tl.x = 140;              GUI_disStr(tl, "y1",        TP_Font, FONT_BACKGROUND, RED);
		tl.x = 40;   tl.y = 180; GUI_disStr(tl, "x2",        TP_Font, FONT_BACKGROUND, RED);
		tl.x = 140;              GUI_disStr(tl, "y2",        TP_Font, FONT_BACKGROUND, RED);
		tl.x = 40;   tl.y = 200; GUI_disStr(tl, "x3",        TP_Font, FONT_BACKGROUND, RED);
		tl.x = 140;              GUI_disStr(tl, "y3",        TP_Font, FONT_BACKGROUND, RED);
		tl.x = 40;   tl.y = 220; GUI_disStr(tl, "x4",        TP_Font, FONT_BACKGROUND, RED);
		tl.x = 140;              GUI_disStr(tl, "y4",        TP_Font, FONT_BACKGROUND, RED);
		
    tl.x = 40 + 3 * TP_Dx;       tl.y = 160; GUI_disNum(tl, topLeft.x,     TP_Font, FONT_BACKGROUND, RED);
    tl.x = 40 + 3 * TP_Dx + 100;             GUI_disNum(tl, topLeft.y,     TP_Font, FONT_BACKGROUND, RED);
    tl.x = 40 + 3 * TP_Dx;       tl.y = 180; GUI_disNum(tl, topRight.x,    TP_Font, FONT_BACKGROUND, RED);
    tl.x = 40 + 3 * TP_Dx + 100;             GUI_disNum(tl, topRight.y,    TP_Font, FONT_BACKGROUND, RED);
    tl.x = 40 + 3 * TP_Dx;       tl.y = 200; GUI_disNum(tl, bottomLeft.x,  TP_Font, FONT_BACKGROUND, RED);
    tl.x = 40 + 3 * TP_Dx + 100;             GUI_disNum(tl, bottomLeft.y,  TP_Font, FONT_BACKGROUND, RED);
    tl.x = 40 + 3 * TP_Dx;       tl.y = 220; GUI_disNum(tl, bottomRight.x, TP_Font, FONT_BACKGROUND, RED);
    tl.x = 40 + 3 * TP_Dx + 100;             GUI_disNum(tl, bottomRight.y, TP_Font, FONT_BACKGROUND, RED);
}
												
uint16_t distance(Coordinate crd1, Coordinate crd2) {
	 // Pythagoras
	 uint32_t Dx = abs((int16_t)(crd1.x - crd2.x));//x1 - x2
   uint32_t Dy = abs((int16_t)(crd1.y - crd2.y));//y1 - y2
   Dx *= Dx;
   Dy *= Dy;
   uint16_t erg = sqrt(Dx + Dy);
	 return erg;	
}

/*******************************************************************************
function:
		Touch screen adjust
*******************************************************************************/
void TP_Adjust(void) {
	  const uint8_t Mar_Val = 12;
	  const LENGTH maxCol = LCD_WIDTH  - 1;
	  const LENGTH maxRow = LCD_HEIGHT - 1;
	  const Coordinate crossPos[4] = 
         { {Mar_Val,Mar_Val}           /* topLeft */,    {maxCol - Mar_Val, Mar_Val}         /* topRight */,
		       {Mar_Val, maxRow - Mar_Val} /* bottomLeft */, {maxCol - Mar_Val, maxRow - Mar_Val}/* bottomLeft */}; 
																		 ;
    Coordinate cornerArr[4]; // {topLeft, topRight, bottomLeft, bottomRight}
   //  uint16_t Sqrt1, Sqrt2;
    // float Dsqrt;
	  Coordinate tl;
    LCD_clear(LCD_BACKGROUND);
	  tl.x = 10; tl.y = 60;
	  GUI_disStr(tl, "Please use a pen and click the cross on", 
        &Font16FLASH, FONT_BACKGROUND, RED);
		tl.y = 80;
	  GUI_disStr(tl, "the screen. The cross will move until the", 
        &Font16FLASH, FONT_BACKGROUND, RED);	
	  tl.y = 100;
		GUI_disStr(tl, "screen adjustment is completed.", 
        &Font16FLASH, FONT_BACKGROUND, RED);	
					 
    sTP_DEV.chStatus = 0;
    while (1) {
			  // Read touch in all four corner points
			  for (int i = 0; i < 4; i++) {
					 cornerArr[i].x = INVALID_POINT;
					 cornerArr[i].y = INVALID_POINT;
					 TP_DrawCross(crossPos[i], RED);
					 while(!TP_pressed()){}; // Wait, until touch pressed
					 while(TP_pressed() || (cornerArr[i].x == INVALID_POINT) || (cornerArr[i].y == INVALID_POINT)) {
					    // select last valid press
						  if ((sTP_DEV.XpointRaw != INVALID_POINT) && (sTP_DEV.YpointRaw != INVALID_POINT)) {
                 cornerArr[i].x = sTP_DEV.XpointRaw;
                 cornerArr[i].y = sTP_DEV.YpointRaw;								 
							}
					 }; 
					 // Touch released and valid coordinate found
           Driver_Delay_ms(200);
					 TP_DrawCross(crossPos[i], WHITE);
				}
				// Calibration
				// 1.Compare the X direction
        uint16_t Sqrt1 = distance(cornerArr[0], cornerArr[1]);
        uint16_t Sqrt2 = distance(cornerArr[2], cornerArr[3]);
        float Dsqrt = (float)Sqrt1 / Sqrt2;
			  bool adjustAgain = (Dsqrt < (float) 0.95) || (Dsqrt > (float) 1.05) || 
									         (Sqrt1 == (float) 0) || (Sqrt2 == (float) 0);
								
			  // 2. Compare the X direction
			  Sqrt1 = distance(cornerArr[0], cornerArr[2]);
        Sqrt2 = distance(cornerArr[1], cornerArr[3]);
        Dsqrt = (float)Sqrt1 / Sqrt2;
        adjustAgain = adjustAgain || (Dsqrt < (float) 0.95) || (Dsqrt > (float) 1.05) ||
									    (Sqrt1 == (float) 0) || (Sqrt2 == (float) 0);
								
        //3.Compare diagonal
			  Sqrt1 = distance(cornerArr[1], cornerArr[2]);
        Sqrt2 = distance(cornerArr[0], cornerArr[3]);
        adjustAgain = adjustAgain || (Dsqrt < (float) 0.95) || (Dsqrt > (float) 1.05) ||
									    (Sqrt1 == (float) 0) || (Sqrt2 == (float) 0);								
								
			  if (adjustAgain) {
           TP_ShowInfo(cornerArr[0], cornerArr[1], cornerArr[2], cornerArr[3]);
           Driver_Delay_ms(1000);
					 continue;
        }

        //4.Get the scale factor and offset
        sTP_DEV.fXfac = 0;
        //According to the display direction to get
        //the corresponding scale factor and offset
        if(CURRENT_SCAN_DIR == R2L_D2U) {
           // DEBUG("R2L_D2U\n");
           sTP_DEV.fXfac = (float)(maxCol - 2 * Mar_Val) /
                           (int16_t)(cornerArr[1].x - cornerArr[0].x);
           sTP_DEV.fYfac = (float)(maxRow - 2 * Mar_Val) /
                           (int16_t)(cornerArr[2].y - cornerArr[0].y);
           sTP_DEV.iXoff = (maxCol -
                            sTP_DEV.fXfac * (cornerArr[1].x + cornerArr[0].x)) / 2;
           sTP_DEV.iYoff = (maxRow -
                            sTP_DEV.fYfac * (cornerArr[2].y + cornerArr[0].y)) / 2;
        } else if(CURRENT_SCAN_DIR == L2R_U2D) {
           // DEBUG("L2R_U2D\n");
           sTP_DEV.fXfac = (float)(maxCol - 2 * Mar_Val) /
                           (int16_t)(cornerArr[0].x - cornerArr[1].x);
           sTP_DEV.fYfac = (float)(maxRow - 2 * Mar_Val) /
                           (int16_t)(cornerArr[0].y - cornerArr[2].y);
           sTP_DEV.iXoff = (maxCol -
                            sTP_DEV.fXfac * (cornerArr[0].x + cornerArr[1].x)) / 2;
           sTP_DEV.iYoff = (maxRow - 
					                  sTP_DEV.fYfac * (cornerArr[0].y + cornerArr[2].y)) / 2;
        } else if (CURRENT_SCAN_DIR == U2D_R2L) {
           // DEBUG("U2D_R2L\n");
           sTP_DEV.fXfac = (float)(maxCol - 2 * Mar_Val) /
                           (int16_t)(cornerArr[1].y - cornerArr[0].y);
           sTP_DEV.fYfac = (float)(maxRow - 2 * Mar_Val) /
                           (int16_t)(cornerArr[2].x - cornerArr[0].x);
           sTP_DEV.iXoff = (maxCol -
                            sTP_DEV.fXfac * (cornerArr[1].y + cornerArr[0].y)) / 2;
           sTP_DEV.iYoff = (maxRow -
                            sTP_DEV.fYfac * (cornerArr[2].x + cornerArr[0].x)) / 2;
         } else {
           // DEBUG("D2U_L2R\n");
           sTP_DEV.fXfac = (float)(maxCol - 2 * Mar_Val) /
                           (int16_t)(cornerArr[0].y - cornerArr[1].y);
           sTP_DEV.fYfac = (float)(maxRow - 2 * Mar_Val) /
                           (int16_t)(cornerArr[0].x - cornerArr[2].x);
           sTP_DEV.iXoff = (maxCol -
                            sTP_DEV.fXfac * (cornerArr[0].y + cornerArr[1].y)) / 2;
           sTP_DEV.iYoff = (maxRow -
                            sTP_DEV.fYfac * (cornerArr[0].x + cornerArr[2].x)) / 2;
         }
         //6.Calibration is successful
         LCD_clear(LCD_BACKGROUND);
				 tl.x = 35; tl.y = 110;
         GUI_disStr(tl, "Touch Screen Adjust OK!", &Font16FLASH , FONT_BACKGROUND , RED);
         Driver_Delay_ms(1000);
         LCD_clear(LCD_BACKGROUND);
         return;
    }
}

/*******************************************************************************
function:
		Use the default calibration factor
*******************************************************************************/
static void TP_SetDefaultCalibration(void) {
    if(	CURRENT_SCAN_DIR == D2U_L2R ) { //SCAN_DIR_DFT = D2U_L2R
        sTP_DEV.fXfac = -0.132443F ;
        sTP_DEV.fYfac = 0.089997F ;
        sTP_DEV.iXoff = 516L ;
        sTP_DEV.iYoff = -22L ;
    } else if( CURRENT_SCAN_DIR == L2R_U2D ) {
        sTP_DEV.fXfac = 0.089697F ;
        sTP_DEV.fYfac = 0.134792F ;
        sTP_DEV.iXoff = -21L ;
        sTP_DEV.iYoff = -39L ;
    } else if( CURRENT_SCAN_DIR == R2L_D2U ) {
        sTP_DEV.fXfac = 0.089915F ;
        sTP_DEV.fYfac =  0.133178F ;
        sTP_DEV.iXoff = -22L ;
        sTP_DEV.iYoff = -38L ;
    } else if( CURRENT_SCAN_DIR == U2D_R2L ) {
        sTP_DEV.fXfac = -0.132906F ;
        sTP_DEV.fYfac = 0.087964F ;
        sTP_DEV.iXoff = 517L ;
        sTP_DEV.iYoff = -20L ;
    } else {
        LCD_clear(LCD_BACKGROUND);
			  Coordinate tl = {0,60};
        GUI_disStr(tl, "Does not support touch-screen \
						calibration in this direction",
                         &Font16FLASH, FONT_BACKGROUND, RED);
    }
}

void TP_Init(bool TPcal) {
    TP_CS_1;
	  sTP_DEV.XpointRaw = 0;
	  sTP_DEV.YpointRaw = 0;
	  sTP_DEV.calibratedTouch.x = INVALID_POINT;
	  sTP_DEV.calibratedTouch.y = INVALID_POINT;
		
    sTP_DEV.chStatus = 0x00;
	  TP_SetDefaultCalibration();
	
	  if (TPcal) {
			 TP_Adjust();
		}
}

bool TP_pressed(void) {
	  return TP_Scan();
}

Coordinate TP_get(void) {
	 Coordinate erg = {sTP_DEV.calibratedTouch.x,sTP_DEV.calibratedTouch.y};
	 return erg;
}

// EOF
