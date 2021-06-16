/*
 * debugger.c
 *
 * Created: 10/05/2021 21:20:06
 * Author : Damian Wójcik
 */ 

#include <avr/io.h>
#include "drivers/core/twi_slave.h"
#include "drivers/core/twi_master.h"
#include "drivers/lcd/LCD_Driver.h"
#include "drivers/lcd/GUI_Paint.h"
#include "drivers/fram/fram.h"
#include "drivers/core/port.h"
#include <avr/interrupt.h>
#include "drivers/core/timer.h"
#include "program/debounce.h"
#include "program/menu.h"
#include "program/com.h"
#include "tests/fram_buffer_test.h"
#include <avr/pgmspace.h>

volatile address_16bit_t fram_push_adr = {0x0};
volatile address_16bit_t fram_pop_adr  = {0x0};
volatile uint8_t		 data_transfer_flag =  0;//0 -transfer off, 1 - transfer on

void configureTWISlave()
{
	TWI_Slave_Setup setup;
	setup.buffer_size_ = 100;
	setup.device_address_ = DEBUGGER_ADR;//not shifted
	setup.device_address_mask_ = 0x0;
	TWI0_Slave_init(setup);
}

void configureTWIMaster()
{
	TWI_Master_Setup setup = TWI_Master_defaultSetup;
	setup.standard_speed_ = TWI_100Kbps;
	setup.startup_enable_ = true;
	setup.use_standard_twi_speed_ = true;
	setup.startup_generate_acknowledge_signal_ = true;
	TWI1_Master_init(setup);
}

void configureLCD()
{
	 Config_Init();
	 LCD_Init();
	 LCD_Clear(0xffff);
	 Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, BLACK);
	 Paint_Clear(BLACK);
	 //Paint_DrawString_EN(20,180, "Welcome to Debugger",  &Font16, BLACK, WHITE);
}

void configurePORTS()
{
	PORT_setPinAsOutput(PORT_CONFIG(E),2);
	PORT_setPinHigh(PORT_STATE(E),2);
}

void configureDebouncingTimer()
{
	TIMER_2_Setup setup = TIMER_2_DefaultSettings;
	setup.prescaler_ = TIMER_Async_Prescaler256;
	setup.mode_ = TIMER_8Bit_Normal;
	setup.interrupt_mode_ = TIMER_8Bit_Overflow;
	DISABLE_BUTTONS();
	TIMER_2_Init(setup,false);
}

void wipeFRAM()
{
	for(uint16_t i = 0 ; i < FRAM_SIZE; ++i)
		FRAM_writeSingleByte(0,i);
}

#define PGM_STR(X) ( (const __flash char[]){ X } )//initializing conversion

const __flash char* const __flash table1[] = { PGM_STR("Line1\0") , PGM_STR("Line2\0") , PGM_STR("Line3\0") };

int main(void)
{
	configurePORTS();
	
	//configureTWISlave();
	//configureTWIMaster();
	configureLCD();
	//configureDebouncingTimer();
	//debounce_init();
	
	//wipeFRAM(); this function slow debugger ,so i tunred it off
	//_delay_ms(300);
	
	MENU_Menu menu1;
	menu1.begin_.x_ = 0;
	menu1.begin_.y_ = 0;
	menu1.end_.x_ = 10;
	menu1.end_.y_ = 10;
	
	MENU_Menu menu2;
	menu2.begin_.x_ = 11;
	menu2.begin_.y_ = 0;
	menu2.end_.x_ = 20;
	menu2.end_.y_ = 10;
	
	MENU_Menu menu3;
	menu3.begin_.x_ = 0;
	menu3.begin_.y_ = 11;
	menu3.end_.x_ = 20;
	menu3.end_.y_ = 19;
	
	MENU_printMenu(&menu1);
	MENU_printMenu(&menu2);
	MENU_printMenu(&menu3);
	
	MENU_TextRange range;
	range.x_start_ = 0;
	range.x_end_   = 20;
	
	menuS.str_lines_ = table1[1];
	
	for(uint8_t i = 0 ; i < 7 ; ++i)
		MENU_printTextLine_NotSelected(menuS.str_lines_ ,&range, 1, 12 + i);
	
	Paint_DrawLine(20 * FONT_16_WIDTH + 6 , 
				   12 * FONT_16_HEIGHT , 
				   20 * FONT_16_WIDTH + 6 , 
				   19 * FONT_16_HEIGHT ,
				   RED , 
				   2 , 
				   LINE_STYLE_SOLID);
	
	Paint_DrawLine(20 * FONT_16_WIDTH + 6 ,
				   12 * FONT_16_HEIGHT ,
				   20 * FONT_16_WIDTH + 6 ,
				   14 * FONT_16_HEIGHT ,
				   RED ,
				   4 ,
				   LINE_STYLE_SOLID);
	
	//sei();
	//fram_test1();
	//fram_test2();
	//fram_test3();
	//fram_test4();
    while (1)
	{
		//MENU_navigate();
	}
}

