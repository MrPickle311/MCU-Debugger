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
}

void configureDebouncingTimer()
{
	TIMER_2_Setup setup = TIMER_2_DefaultSettings;
	setup.prescaler_ = TIMER_Async_Prescaler256;
	setup.mode_ = TIMER_8Bit_Normal;
	setup.interrupt_mode_ = TIMER_8Bit_Overflow;
	TIMER_haltAsynchronousTimer();
	TIMER_2_Init(setup,false);
}

int main(void)
{
    configureTWISlave();
    configureTWIMaster();
	configureLCD();
	configurePORTS();
	configureDebouncingTimer();
	debounce_init();
	
	_delay_ms(500);
	
	sei();
	
	MENU_navigate();
    while (1) 
    {
		if(data_transfer_flag)
		{
			if(TWI_Buffer_Is_DataIsReadyToProcess(TWI0_Buffer))
			{
				
				/*
				switch(TWI1_Buffer.body_[0])
				{
					case 
				}*/
				
				//Paint_DrawNum(16,220,TWI0_Buffer.body_[0],  &Font16, BLACK, WHITE);
				//Paint_DrawNum(16,236,TWI0_Buffer.body_[1],  &Font16, BLACK, WHITE);
				//Paint_DrawNum(16,252,TWI0_Buffer.body_[2],  &Font16, BLACK, WHITE);
				//Paint_DrawNum(16,268,TWI0_Buffer.body_[3],  &Font16, BLACK, WHITE);
				TWI0_emptyBuffer();
				//data_transfer_flag = 0;
			}
		}
		//else 
    }
}

