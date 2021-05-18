/*
 * settings.c
 *
 * Created: 17/05/2021 23:33:27
 *  Author: Damian
 */ 

#include "settings.h"

volatile uint8_t breakpoints_total_nmbr = 1;

volatile uint8_t current_breakpoint_to_save = 0;

volatile FRAM_CircularBuffer fram_buffer;

volatile uint8_t current_breakpoint_to_load = 0;

void goToNextSaveBreakpoint()
{
	current_breakpoint_to_save = ( current_breakpoint_to_save + 1 ) % breakpoints_total_nmbr;
	FRAM_CircularBuffer_reconfigure(fram_buffer,GET_BREAKPOINT_START_ADDRESS(current_breakpoint_to_save), GET_BREAKPOINT_END_ADDRESS(current_breakpoint_to_save));
}

void goToNextLoadBreakpoint()
{
	current_breakpoint_to_load = ( current_breakpoint_to_load + 1 ) % breakpoints_total_nmbr;
	FRAM_CircularBuffer_reconfigure(fram_buffer,GET_BREAKPOINT_START_ADDRESS(current_breakpoint_to_load), GET_BREAKPOINT_END_ADDRESS(current_breakpoint_to_load));
}

void configureDebuggerFRAM(uint16_t breakpoints_nmbr)
{
	breakpoints_total_nmbr = breakpoints_nmbr;
	fram_buffer = FRAM_CircularBuffer_init(GET_BREAKPOINT_START_ADDRESS(0), GET_BREAKPOINT_END_ADDRESS(0));
}

