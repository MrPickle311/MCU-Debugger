/*
 * settings.c
 *
 * Created: 17/05/2021 23:33:27
 *  Author: Damian Wójcik
 */ 

#include "settings.h"

volatile uint8_t breakpoints_total_nmbr = 1;

volatile uint8_t current_breakpoint_to_save = 0;

volatile uint8_t current_breakpoint_to_load = 0;

volatile FRAM_CircularBuffer fram_buffer;

void resetLoadBreakpoint()
{
	//UPDATE_SAVE_POS();
	current_breakpoint_to_load = 0;
	FRAM_CircularBuffer_reconfigure(fram_buffer,GET_BREAKPOINT_START_ADDRESS(0), GET_BREAKPOINT_END_ADDRESS(0));
	FRAM_CircularBuffer_injectSavePosition(fram_buffer,current_fram_push_positions[current_breakpoint_to_load]);
}

void resetSaveBreakpoint()
{
	FRAM_CircularBuffer_reconfigure(fram_buffer,GET_BREAKPOINT_START_ADDRESS(current_breakpoint_to_save), GET_BREAKPOINT_END_ADDRESS(current_breakpoint_to_save));
}

void goToNextSaveBreakpoint()//te dwie linijki nastêpuj¹ po sobie , co jest przyczyn¹ buga
{
	current_fram_push_positions[current_breakpoint_to_save] = FRAM_CircularBuffer_getFillLevel(fram_buffer);
	current_breakpoint_to_save = ( current_breakpoint_to_save + 1 ) % breakpoints_total_nmbr;
}

void goToNextLoadBreakpoint()
{
	current_breakpoint_to_load = ( current_breakpoint_to_load + 1 ) % breakpoints_total_nmbr;
	FRAM_CircularBuffer_reconfigure(fram_buffer,GET_BREAKPOINT_START_ADDRESS(current_breakpoint_to_load), GET_BREAKPOINT_END_ADDRESS(current_breakpoint_to_load));
	FRAM_CircularBuffer_injectSavePosition(fram_buffer,current_fram_push_positions[current_breakpoint_to_load]);
}

void configureDebuggerFRAM(uint16_t breakpoints_nmbr)
{
	breakpoints_total_nmbr = breakpoints_nmbr;
	current_fram_push_positions = calloc(breakpoints_total_nmbr,sizeof(uint16_t));
	for_N(i,breakpoints_total_nmbr)
		current_fram_push_positions[i] = 0;
	fram_buffer = FRAM_CircularBuffer_init(GET_BREAKPOINT_START_ADDRESS(0), GET_BREAKPOINT_END_ADDRESS(0));
}

