/*****************************************************************************
* | File      	:	LCD_Driver.c
* | Author      :   Waveshare team
* | Function    :	ILI9486 Drive function
* | Info        :
*   Image scanning
*      Please use progressive scanning to generate images or fonts
*----------------
* |	This version:   V1.0
* | Date        :   2018-01-11
* | Info        :   Basic version
*
******************************************************************************/

/**************************Intermediate driver layer**************************/
#include "../IncLocal/LCD_Driver.h"
#include "../IncLocal/LCD_Tools.h"
#include "../IncLocal/LCD_GPIO.h"
#include "../IncLocal/LCD_TIM.h"
#include "../IncLocal/LCD_SPI.h"
#include "LCD_Debug.h"

#define LCD_CS_0		HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)
#define LCD_CS_1		HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)

#define LCD_RST_0		HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET)
#define LCD_RST_1		HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET)

#define LCD_DC_0		HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET)
#define LCD_DC_1		HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET)

// Masks for scan direction of frame buffer 
#if CURRENT_SCAN_DIR == L2R_U2D
#define MEM_ACC_REG_DATA_FOR_CMD_0X36   0x08
#define DIS_FUN_REG_DATA_FOR_CMD_0XB6   0x22
#define SCREEN_FORMAT_LANDSCAPE (LCD_HEIGHT > LCD_WIDTH)

#elif CURRENT_SCAN_DIR == L2R_D2U
#define MEM_ACC_REG_DATA_FOR_CMD_0X36  0x08
#define DIS_FUN_REG_DATA_FOR_CMD_0XB6  0x62
#define SCREEN_FORMAT_LANDSCAPE (LCD_HEIGHT > LCD_WIDTH)

#elif CURRENT_SCAN_DIR == R2L_U2D
#define MEM_ACC_REG_DATA_FOR_CMD_0X36  0x08
#define DIS_FUN_REG_DATA_FOR_CMD_0XB6  0x02
#define SCREEN_FORMAT_LANDSCAPE (LCD_HEIGHT > LCD_WIDTH)

#elif CURRENT_SCAN_DIR == R2L_D2U
#define MEM_ACC_REG_DATA_FOR_CMD_0X36  0x08
#define DIS_FUN_REG_DATA_FOR_CMD_0XB6  0x42
#define SCREEN_FORMAT_LANDSCAPE (LCD_HEIGHT > LCD_WIDTH)

#elif CURRENT_SCAN_DIR == U2D_L2R
#define MEM_ACC_REG_DATA_FOR_CMD_0X36  0x28
#define DIS_FUN_REG_DATA_FOR_CMD_0XB6  0x22
#define SCREEN_FORMAT_LANDSCAPE (LCD_WIDTH > LCD_HEIGHT)

#elif CURRENT_SCAN_DIR == U2D_R2L
#define MEM_ACC_REG_DATA_FOR_CMD_0X36  0x28
#define DIS_FUN_REG_DATA_FOR_CMD_0XB6  0x02
#define SCREEN_FORMAT_LANDSCAPE (LCD_WIDTH > LCD_HEIGHT)

#elif CURRENT_SCAN_DIR == D2U_L2R
#define MEM_ACC_REG_DATA_FOR_CMD_0X36  0x28
#define DIS_FUN_REG_DATA_FOR_CMD_0XB6  0x62
#define SCREEN_FORMAT_LANDSCAPE (LCD_WIDTH > LCD_HEIGHT)

#elif CURRENT_SCAN_DIR == D2U_R2L
#define MEM_ACC_REG_DATA_FOR_CMD_0X36  0x28
#define DIS_FUN_REG_DATA_FOR_CMD_0XB6  0x42
#define SCREEN_FORMAT_LANDSCAPE (LCD_WIDTH > LCD_HEIGHT)

#endif

/*******************************************************************************
function:
	Hardware reset
*******************************************************************************/
static void LCD_Reset(void){
    LCD_RST_1;
    Driver_Delay_ms(500);
    LCD_RST_0;
    Driver_Delay_ms(500);
    LCD_RST_1;
    Driver_Delay_ms(500);
}

static void LCD_SetBackLight(uint16_t value){
	PWM_SetValue(value);
}

/**
  * @brief  This function writes a command to ILI9486L LCD driver chip.
	* @param  cmd	Command
  * @retval None
  */
static void LCD_WriteCMD(uint8_t cmd){
    LCD_DC_0;
    LCD_CS_0;
    SPI4W_Write_Byte(cmd);
    LCD_CS_1;
}

/**
  * @brief  This function writes the same 16 bit parameter, that belongs to a
  *         command of ILI9486L LCD driver chip, several times.
  *         This function should be used for successive parameters
  *         that have the same value.
	* @param  param The parameter of the CMD
  * @param  no    Number of times, the parameter should be written
  * @retval None
  */
static void LCD_WriteParameters(uint16_t param, uint32_t no){
    LCD_DC_1;
    LCD_CS_0;
    for(uint32_t i = 0; i < no; i++) {
        SPI4W_Write_Byte(param >> 8);
        SPI4W_Write_Byte(param & 0XFF);
    }
    LCD_CS_1;
}

/**
  * @brief  This function writes a parameter, that belongs to a
  *         command of ILI9486L LCD driver chip.
  *         Successive parameters are sent by successive calls
  *         of this function.
	* @param  param The parameter of the CMD
  * @retval None
  */
static void LCD_WriteParam(uint16_t param){
    LCD_WriteParameters(param,1);
}

static void LCD_WriteCMDList(const char *list, int noOfListElems) {
	  int elem = 0;
	  while (elem < noOfListElems) {
			 LCD_WriteCMD(list[elem++]);
			 int noOfArgs = list[elem++];
			 while(noOfArgs-- > 0) {
				  LCD_WriteParam(list[elem++]);
			 } 			
		}
}

/* A Command Sequenz is a byte Array, that consists out of several command.
   Each command has following structure:
        1. Byte command
        2. Byte Number of commands
        3. For each Argument one byte
*/

const char initCmds[] =  {
	    (0xF9), // 1. command
			2,      // 2 agurments
	    (0x00), // arg1
      (0x08), // arg2

      (0xC0), // 2. Command
	     2,     // two arguments
      (0x19), // arg 1 VREG1OUT POSITIVE
      (0x1a), // arg 2 VREG2OUT NEGATIVE

      (0xC1),
	    2,
      (0x45),//VGH,VGL    VGH>=14V.
      (0x00),

      (0xC2),	//Normal mode, increase can change the display quality, while increasing power consumption
      1,
	    (0x33),

      (0XC5),
	    2,
      (0x00),
      (0x28),//VCM_REG[7:0]. <=0X80.

      (0xB1), //Sets the frame frequency of full color normal mode
			2, 
      (0xA0),//0XB0 =70HZ, <=0XB0.0xA0=62HZ
      (0x11),

      (0xB4),
			1,
      (0x02), //2 DOT FRAME MODE,F<=70HZ.

      (0xB6),//
			3,
      (0x00),
      (0x42),//0 GS SS SM ISC[3:0];
      (0x3B),

      (0xB7),
			1,
      (0x07),

      (0xE0),
			15,
      (0x1F), // arg 1
      (0x25),
      (0x22), // arg 3
      (0x0B),
      (0x06), // arg 5
      (0x0A),
      (0x4E), // arg 7
      (0xC6),
      (0x39), // arg 9
      (0x00),
      (0x00), // arg 11
      (0x00),
      (0x00), // arg 13
      (0x00), 
      (0x00), // arg 15

      (0XE1),
			15,
      (0x1F), // arg 1
      (0x3F),
      (0x3F), // arg 3
      (0x0F),
      (0x1F), // arg 5
      (0x0F),
      (0x46), // arg 7
      (0x49),
      (0x31), // arg 9
      (0x05),
      (0x09), // arg 11
      (0x03),
      (0x1C), // arg 13
      (0x1A),
      (0x00), // arg 15

      (0XF1),
			8,
      (0x36), // arg 1
      (0x04),
      (0x00), // arg 3
      (0x3C),
      (0x0F), // arg 5
      (0x0F),
      (0xA4), // arg 7
      (0x02), // arg 8

      (0XF2),
			9,
      (0x18), // arg 1
      (0xA3),
      (0x12), // arg 3
      (0x02),
      (0x32), // arg 5
      (0x12),
      (0xFF), // arg 7
      (0x32),
      (0x00), // arg 9

      (0XF4),
			5,
      (0x40), // arg 1
      (0x00),
      (0x08), // arg 3
      (0x91),
      (0x04), // arg 5

      (0XF8),
			2,
      (0x21),
      (0x04),

      (0X3A),	//Set Interface Pixel Format
			1,
      (0x55)
};

/********************************************************************************
function:	Set the display scan and color transfer modes
parameter:
		Scan_dir   :   Scan direction
		Colorchose :   RGB or GBR color format
********************************************************************************/
static void LCD_SetGramScanWay(void){
 
    // Set the read / write scan direction of the frame memory
    LCD_WriteCMD(0xB6);
    LCD_WriteParam(0X00);
    LCD_WriteParam(DIS_FUN_REG_DATA_FOR_CMD_0XB6);

    LCD_WriteCMD(0x36);
    LCD_WriteParam(MEM_ACC_REG_DATA_FOR_CMD_0X36);
}

/********************************************************************************
function:
	initialization
********************************************************************************/
void LCD_init(uint16_t LCD_BLval) {
	
	 // Initialize HW
   GPIO_Init(); // Initialize all configured peripherals
   SPIinit();  // Initialize SPI
   TIM4_Init(); // Initialize Timer for LCD Display
	
   LCD_Reset(); // HW reset
	
	 LCD_BLval = (LCD_BLval > 1000) ? 1000 : LCD_BLval; 
	 LCD_SetBackLight(LCD_BLval);
	
   LCD_WriteCMDList(initCmds,sizeof(initCmds)); //Set the initialization register
		
   LCD_SetGramScanWay( ); //Set the display scan and color transfer modes
   Driver_Delay_ms(200);

   //sleep out
   LCD_WriteCMD(0x11);
   Driver_Delay_ms(120);

   //Turn on the LCD display
   LCD_WriteCMD(0x29);
}

static void LCD_setWindow(Coordinate tl, Coordinate br){
	 COORDINATE_TO_DM(tl, br);	
	 // set Window
	 LCD_WriteCMD(0x2A); //set the X coordinates
   LCD_WriteParam((tl.x >> 8) & 0xFF); //Set the horizontal starting point to the high octet
   LCD_WriteParam(tl.x & 0xFF);        //Set the horizontal starting point to the low octet
   LCD_WriteParam((br.x >> 8) & 0xFF); //Set the horizontal end to the high octet
   LCD_WriteParam(br.x & 0xFF);	       //Set the horizontal end to the low octet

   LCD_WriteCMD(0x2B); //set the Y coordinates
   LCD_WriteParam((tl.y >> 8) & 0xFF);
   LCD_WriteParam(tl.y & 0xFF);
   LCD_WriteParam((br.y >> 8) & 0xFF);
   LCD_WriteParam(br.y  & 0xFF);
}

/********************************************************************************
function:	Fill an area with a color
parameter:
	topLeft :   Start point x coordinate
	Ystart  :   Start point y coordinate
	Xend   :   End point coordinates
	Yend   :   End point coordinates
	Color  :   Set the color
********************************************************************************/
void LCD_fillRectangleMC(Coordinate tl, LENGTH width,	LENGTH height, COLOR color){
   if (VALID_COORDINATE(tl)){
	    // calculate bottomRight coordinate.
	    // Pixel out of display range will be ignored
	    Coordinate br = {tl.x + (width-1), tl.y + (height - 1)};
	    br.x = (br.x > (LCD_WIDTH  - 1)) ? (LCD_WIDTH  - 1) : br.x;
	    br.y = (br.y > (LCD_HEIGHT - 1)) ? (LCD_HEIGHT - 1) : br.y;
	 
	    LCD_setWindow(tl, br);
		 
			// Write Color into Window
			LCD_WriteCMD(0x2C);
			uint32_t noOfPixel = (uint32_t)(br.x - tl.x + 1) * (uint32_t)(br.y - tl.y + 1);
			LCD_WriteParameters(color , noOfPixel);
	 }
}

void LCD_fillRectangleBW (Coordinate tl, LENGTH width,	LENGTH height, COLOR bg, COLOR fg, const uint8_t *table){
	 if (VALID_COORDINATE(tl)){
 	    // calculate bottomRight coordinate.
	    // Pixel out of display range will be ignored
	    Coordinate br = {tl.x + (width-1), tl.y + (height - 1)};
	    br.x = (br.x > (LCD_WIDTH  - 1)) ? (LCD_WIDTH  - 1) : br.x;
	    br.y = (br.y > (LCD_HEIGHT - 1)) ? (LCD_HEIGHT - 1) : br.y;
 
	    LCD_setWindow(tl, br);
			
			// write colors according to table (given in the STM32 font front)
			LCD_WriteCMD(0x2C);	
      for (POINT row = 0; (row < height) && ((row + tl.y) <= br.y); row ++ ){ // write all lines
         for (POINT col = 0; col < width; col ++ ){ // write each col of one line
					  if ((col + tl.x) <= br.x) { // in display range
							 LCD_WriteParam((*table & (0x80 >> (col % 8))) ? fg : bg);	
            }
            if(col % 8 == 7) table++;
         }
         if(width % 8 != 0) table++;
      }
	 }
}

void LCD_clear(COLOR color){
	Coordinate topLeft = {0,0};
	LCD_fillRectangleMC(topLeft, LCD_WIDTH, LCD_HEIGHT, color);
}

bool LCD_isHorizontalMode(void){
	return SCREEN_FORMAT_LANDSCAPE;
}

// EOF
