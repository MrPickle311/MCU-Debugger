/*
 * com.c
 *
 * Created: 15/05/2021 22:04:11
 *  Author: Damian Wójcik
 */ 

#include "com.h"

void COM_sendDebugSignal()
{
	PORT_setPinHigh(PORT_STATE(E),2);
	_delay_ms(2);
	PORT_setPinLow(PORT_STATE(E),2);
}

void COM_getVariable()
{
	FRAM_CircularBuffer_safePush(fram_buffer,TWI_Buffer_AtByte(TWI0_Buffer,1));//value
	FRAM_CircularBuffer_safePush(fram_buffer,TWI_Buffer_AtByte(TWI0_Buffer,2));//H-adr
	FRAM_CircularBuffer_safePush(fram_buffer,TWI_Buffer_AtByte(TWI0_Buffer,3));//L-adr
	while(TWI_Buffer_AtCurrentByte(TWI0_Buffer))//name, while name[i] != '\0'
	{
		FRAM_CircularBuffer_safePush(fram_buffer,TWI_Buffer_AtCurrentByte(TWI0_Buffer));
		TWI_Buffer_GoNextByte(TWI0_Buffer);
	}
	FRAM_CircularBuffer_safePush(fram_buffer,TWI_Buffer_AtCurrentByte(TWI0_Buffer));// '\0' , ewentualnie usun¹æ
}

void COM_getArray()
{
	//TODO:
}

void COM_configureDevice()
{
	configureDebuggerFRAM(TWI_Buffer_AtByte(TWI0_Buffer,1));
}

void COM_commandProcessor()
{
	bool proccessing_flag = true;
	while(proccessing_flag)
	{
		if(TWI_Buffer_Is_DataIsReadyToProcess(TWI0_Buffer))
		{
			switch(TWI_Buffer_AtByte(TWI0_Buffer,0))
			{
				case DEVICE_START:
					COM_configureDevice();
					proccessing_flag = false;
					break;
				case SENDING_VARIABLE:
					COM_getVariable();
					break;
				case SENDING_ARRAY:
					COM_getArray();
					break;
				case END_TRANSACTION:
					proccessing_flag = false;
					break;
			}
			TWI0_emptyBuffer();
		}
	}
}

