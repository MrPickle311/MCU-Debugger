/*
 * com.h
 *
 * Created: 15/05/2021 22:03:55
 *  Author: Damian W�jcik
 */ 


#ifndef COM_H_
#define COM_H_

#include "../drivers/core/twi_slave.h"
#include "../drivers/fram/fram.h"
#include "../drivers/core/port.h"
#include "settings.h"
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
	SENDING_ARRAY	   = 0x82,
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
	uint8_t  current_text_pos = 0;
}var_buffer;

void COM_initVariableBuffer();

void COM_getVariableData();

#endif /* COM_H_ */