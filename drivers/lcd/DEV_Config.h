/*****************************************************************************
* | File        :   DEV_Config.c
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*                Used to shield the underlying layers of each master 
*                and enhance portability
*----------------
* | This version:   V1.0
* | Date        :   2018-11-22
* | Info        :

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
#ifndef _DEV_CONFIG_H_
#define _DEV_CONFIG_H_

#include <stdint.h>
#include <stdio.h>
#include "Debug.h"
#include <avr/pgmspace.h>
#include <avr/builtins.h>
#include <util/delay.h>
#include "../core/bit_utils.h"

#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

/**
 * GPIO config
**/
//DIN MOSI PB3 11
//CLK SCK PB5 13
#define DEV_CS_PIN  10	//PB2
#define DEV_DC_PIN  7	//PD7
#define DEV_RST_PIN 8	//PB0
#define DEV_BL_PIN  9	//PB1

void __write(volatile uint8_t* reg,uint8_t pin,uint8_t state);

void LCD_digitalWrite(uint8_t pin_nr,uint8_t state);

uint8_t __read(volatile uint8_t* reg,uint8_t pin);

uint8_t LCD_digitalRead(uint8_t pin_nr);

void initTimer1();

uint8_t transfer(uint8_t data);

void LCD_analogWrite(uint8_t value);

/**
 * GPIO read and write
**/
#define DEV_Digital_Write(_pin, _value) LCD_digitalWrite(_pin, _value == 0? 0:1)
#define DEV_Digital_Read(_pin) LCD_digitalRead(_pin)


/**
 * SPI
**/
#define DEV_SPI_WRITE(_dat)    transfer(_dat)

/**
 * delay x ms
**/
#define DEV_Delay_ms(__xms)    _delay_ms(__xms)

/**
 * PWM_BL
**/
 #define  DEV_Set_PWM(_Value)  LCD_analogWrite(_Value)

/*-----------------------------------------------------------------------------*/
 void Config_Init();
#endif
