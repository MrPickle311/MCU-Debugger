/*
 * com.c
 *
 * Created: 15/05/2021 22:04:11
 *  Author: Damian Wójcik
 */ 

#include "com.h"

void COM_sendDebugSignal()
{
	PORT_setPinLow(PORT_STATE(E),2);
	_delay_ms(2);
	PORT_setPinHigh(PORT_STATE(E),2);
}

void COM_getVariable()
{
	FRAM_CircularBuffer_safePush(fram_buffer,TWI_Buffer_AtByte(TWI0_Buffer,1));//sizeof
	
	for_N(i,TWI_Buffer_AtByte(TWI0_Buffer,1))
		FRAM_CircularBuffer_safePush(fram_buffer,TWI_Buffer_AtByte(TWI0_Buffer,2 + i));
	
	TWI_Buffer_GoNextByte(TWI0_Buffer);//ommit a command
	TWI_Buffer_GoNextByte(TWI0_Buffer);//ommit a variable size
	
	for_N(i,TWI_Buffer_AtByte(TWI0_Buffer,1))
		TWI_Buffer_GoNextByte(TWI0_Buffer);//ommit a variable body


	uint8_t i = 2 + TWI_Buffer_AtByte(TWI0_Buffer,1);
	while(TWI_Buffer_AtByte(TWI0_Buffer,i))//name sending, while name[i] != '\0'
	{
		FRAM_CircularBuffer_safePush(fram_buffer,TWI_Buffer_AtByte(TWI0_Buffer,i));
		++i;
	}

	FRAM_CircularBuffer_safePush(fram_buffer,TWI_Buffer_AtCurrentByte(TWI0_Buffer));// '\0' 
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
					//Paint_DrawString_EN(16*6,16*10,"Start",&Font16,GREEN,WHITE);
					//Paint_DrawRectangle(16*6 - 5,16*10 - 5,16*6+16*4,16*10 + 16,WHITE,2,DRAW_FILL_EMPTY);
					COM_configureDevice();
					TWI0_emptyBuffer();
					break;
				case SENDING_VARIABLE:
					COM_getVariable();
					TWI0_emptyBuffer();
					break;
				case SENDING_ARRAY:
					COM_getArray();
					break;
				case END_TRANSACTION:
					TWI0_emptyBuffer();
					proccessing_flag = false;
					break;
				default:
					//CHANGE TO PRINT A MENU_MESSAGE
					//Paint_Clear(BLACK);
					//Paint_DrawString_EN(0,0, "ERROR!",&Font16,BLACK,WHITE);
					while(1);
			}
		}
	}
}

