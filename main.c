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
#include "program/menu_logic.h"
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
	 Paint_DrawString_EN(20,180, "Welcome to Debugger",  &Font16, BLACK, WHITE);
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

int main(void)
{
	configurePORTS();
	configureTWISlave();
	configureTWIMaster();
	configureLCD();
	configureDebouncingTimer();
	
	_delay_ms(300);
	sei();
	
	MENU_startApplication();
	
	//fram_test1();
	//fram_test2();
	//fram_test3();
	//fram_test4();
    while (1);
}

