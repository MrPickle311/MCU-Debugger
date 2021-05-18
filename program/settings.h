/*
 * settings.h
 *
 * Created: 17/05/2021 23:33:08
 *  Author: Damian W�jcik
 */ 


#ifndef SETTINGS_H_
#define SETTINGS_H_

#define FRAM_SIZE 32768

#include "../drivers/fram/fram_circular_buffer.h"

volatile uint8_t breakpoints_total_nmbr;

volatile uint8_t current_breakpoint_to_save;

volatile uint8_t current_breakpoint_to_load;

volatile FRAM_CircularBuffer fram_buffer;

#define GET_BREAKPOINT_START_ADDRESS(breakpoint_nmbr)	( FRAM_SIZE / breakpoints_total_nmbr ) * breakpoint_nmbr

#define GET_BREAKPOINT_END_ADDRESS(breakpoint_nmbr)		( FRAM_SIZE / breakpoints_total_nmbr ) * ( breakpoint_nmbr + 1 ) - 1

#define GET_BREAKPOINT_CURR_POS()						breakpoint_current_pos		

void goToNextSaveBreakpoint();

void goToNextLoadBreakpoint();

void configureDebuggerFRAM(uint16_t breakpoints_nmbr);

#endif /* SETTINGS_H_ */