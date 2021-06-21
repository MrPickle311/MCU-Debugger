/*
 * com.h
 *
 * Created: 15/05/2021 22:03:55
 *  Author: Damian Wójcik
 */ 


#ifndef COM_H_
#define COM_H_

#include "../drivers/core/twi_slave.h"
#include "../drivers/fram/fram.h"
#include "../drivers/core/port.h"
#include "core.h"
#include <util/delay.h>
#include <stdbool.h>
#include "debounce.h"
#include "../drivers/lcd/GUI_Paint.h"

//debugger settings

#define DEBUGGER_ADR			0xF0

void COM_sendDebugSignal();

enum Command
{
	DEVICE_START	   = 0x80,
	SENDING_VARIABLE   = 0x81,
	NAME_SENT          = 0x8C,
	END_TRANSACTION    = 0xFF
};

void COM_getVariable();

void COM_configureDevice();

void COM_commandProcessor();

#define NAME_MAX_LENGTH		40

volatile struct COM_ReceivedData
{
	uint32_t value_;
	char*	 raw_name_text_;
	uint8_t  current_text_pos_;
	bool	 is_empty_;
}var_buffer;

#define VAR_BUF_AT_CURRENT_CHAR(var_buffer)	var_buffer.raw_name_text_[var_buffer.current_text_pos_]


void COM_initVariableBuffer();

void COM_getVariableData();

#endif /* COM_H_ */