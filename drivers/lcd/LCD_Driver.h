/*****************************************************************************
* | File      	:	LCD_Driver.h
* | Author      :   Waveshare team
* | Function    :   LCD driver
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2018-12-18
* | Info        :   
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#ifndef __LCD_DRIVER_H
#define __LCD_DRIVER_H

#include "DEV_Config.h"

#define LCD_WIDTH   240 //LCD width
#define LCD_HEIGHT  320 //LCD height
 
//void LCD_WriteData_Word(UWORD da);

//void LCD_SetCursor(UWORD X, UWORD Y);
void LCD_SetWindow(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD  Yend);
void LCD_DrawPaint(UWORD x, UWORD y, UWORD Color);

void LCD_Init(void);
void LCD_SetBackLight(UWORD Value);

void LCD_Clear(UWORD Color);
void LCD_ClearWindow(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend,UWORD color);

#define LCD_Write_Command(data)\
		DEV_CS_PIN_SET_LOW();\
		DEV_DC_PIN_SET_LOW();\
		DEV_SPI_WRITE(data)

#define LCD_WriteData_Byte(data)\
		DEV_CS_PIN_SET_LOW();\
		DEV_DC_PIN_SET_HIGH();\
		DEV_SPI_WRITE(data);\
		DEV_CS_PIN_SET_HIGH()

#define LCD_WriteData_Word(data)\
		DEV_CS_PIN_SET_LOW();\
		DEV_DC_PIN_SET_HIGH();\
		DEV_SPI_WRITE((data>>8) & 0xff);\
		DEV_SPI_WRITE(data);\
		DEV_CS_PIN_SET_HIGH()

#define LCD_SetCursor(X,Y)\
		LCD_Write_Command(0x2a);\
		LCD_WriteData_Byte(X >> 8);\
		LCD_WriteData_Byte(X);\
		LCD_WriteData_Byte(X >> 8);\
		LCD_WriteData_Byte(X);\
		\
		LCD_Write_Command(0x2b);\
		LCD_WriteData_Byte(Y >> 8);\
		LCD_WriteData_Byte(Y);\
		LCD_WriteData_Byte(Y >> 8);\
		LCD_WriteData_Byte(Y);\
		\
		LCD_Write_Command(0x2C)

#endif
