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

void initTimer1()//pwm mode
{
	TIMER_16BitSetup setup	  = TIMER_16bit_DefaultSettings;
	setup.mode_				  = TIMER_16Bit_8BitFastPWM_Top;
	setup.pins_under_control_ = TIMER_OnlyPinA;
	setup.pin_A_mode_		  = TIMER_CompareMatchClearPin;
	setup.prescaler_		  = TIMER_Synchronous_NoPrescaling;
	TIMER_1_Init(setup,false);
	DEV_Set_PWM(140);
}

#define PIN_0 0 
#define PIN_1 1 
#define PIN_2 2 
#define PIN_3 3 
#define PIN_5 5 
#define PIN_7 7

void GPIO_Init()
{
	PORT_setPinsAsOutput( PORT_CONFIG(B) ,
						  PIN_0			 ,
						  PIN_1			 ,
						  PIN_2			 ,
						  PIN_3			 ,
						  PIN_5);
	
	PORT_setPinAsOutput(PORT_CONFIG(D) , PIN_7);
	PORT_setPinHigh(PORT_STATE(B) , PIN_2 );
}

void spi_init()
{
	SPI_Setup setup = SPI_DefaultSetup;
	setup.clock_polarization_ = SPI_HighState_AtIdleState;
	setup.sampled_edge_ = SPI_FallingEdge;
	setup.sampled_edge_ = SPI_LSB_AtFirst;
	setup.sck_prescaler_ = SPI_SCK_2;
	setup.startup_enable_ = true;
	
	SPI0_Init(setup);
}

void Config_Init()
{
	GPIO_Init();
	initTimer1();
    spi_init();
}
