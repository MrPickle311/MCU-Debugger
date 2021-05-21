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
#include "DEV_Config.h"

void __write(volatile uint8_t* reg,uint8_t pin,uint8_t state)
{
	CLEAR_BIT_AT(*reg,pin);
	if(state != 0)
		SET_BIT_AT(*reg,pin);
}

void LCD_digitalWrite(uint8_t pin_nr,uint8_t state)
{
	switch(pin_nr)
	{
		case 7:
			__write(&PORTD,7,state);
			break;
		case 8:
			__write(&PORTB,0,state);
			break;
		case 9:
			__write(&PORTB,1,state);
			break;
		case 10:
			__write(&PORTB,2,state);
			break;
		default:
			break;
	}
}

uint8_t __read(volatile uint8_t* reg,uint8_t pin)
{
	return IS_BIT_SET_AT(*reg,pin);
}

uint8_t LCD_digitalRead(uint8_t pin_nr)
{
	switch(pin_nr)
	{
		case 7:
			return PIND & _BV(DDD7);
		case 8:
			return PINB & _BV(DDB0);
		case 9:
			return PINB & _BV(DDB1);
		case 10:
			return PINB & _BV(DDB2);
		default:
			return 0;
	}
}

void initTimer1()
{
	TCCR1A |= (_BV(COM1A1));
	TCCR1A |= (_BV(WGM10));
	TCCR1B = _BV(WGM12) | _BV(CS10);
}

uint8_t transfer(uint8_t data)
{
	SPDR0 = data;
	asm volatile("nop");//wait a clock tact
	while (!(SPSR0 & _BV(SPIF))) ; // wait
	return SPDR0;
}

void LCD_analogWrite(uint8_t value)
{
	OCR1A = value;
}

void GPIO_Init()
{
	SET_BIT_AT(DDRB,DDB0);
	SET_BIT_AT(DDRB,DDB1);
	SET_BIT_AT(DDRB,DDB2);
	SET_BIT_AT(DDRD,DDD7);
	initTimer1();
	LCD_analogWrite(140);
}

void Config_Init()
{
     GPIO_Init();
     
     SPCR0 |= _BV(CPOL) | _BV(CPHA);
     
     SPCR0 &= ~(_BV(DORD)); 
     
     //SPCR0 = ( SPCR0 & ~0x03  ) | ( 0x04  & 0x03  );
	 
	 //SPCR0 = ( SPCR0 & ~0x03  );
	 
     //SPSR0 = (SPSR0 & ~0x01  ) | ((0x04  >> 2) & 0x01  );
	 
     SPCR0 |= _BV(MSTR);
     SPCR0 |= _BV(SPE);
     
     SET_BIT_AT(PORTB,DDB2);//ss
     SET_BIT_AT(DDRB,DDB3);
     SET_BIT_AT(DDRB,DDB5); 
}
