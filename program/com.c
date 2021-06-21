/*
 * com.c
 *
 * Created: 15/05/2021 22:04:11
 *  Author: Damian Wójcik
 */ 

#include "com.h"

void COM_sendDebugSignal()//it sets a low  state for a 2 ms -> request for a new breakpoint
{
	PORT_setPinLow(PORT_STATE(E),2);
	_delay_ms(2);
	PORT_setPinHigh(PORT_STATE(E),2);
}

void COM_getVariable()
{
	FRAM_CircularBuffer_safePush(fram_buffer,TWI_Buffer_AtByte(TWI0_Buffer,1));//sizeof
	
	for_N(i,TWI_Buffer_AtByte(TWI0_Buffer,1))
		FRAM_CircularBuffer_safePush( fram_buffer							,
									  TWI_Buffer_AtByte(TWI0_Buffer,2 + i));
	
	TWI_Buffer_GoNextByte(TWI0_Buffer);//ommit a command
	TWI_Buffer_GoNextByte(TWI0_Buffer);//ommit a variable size
	
	for_N(i,TWI_Buffer_AtByte(TWI0_Buffer,1))
		TWI_Buffer_GoNextByte(TWI0_Buffer);//ommit a variable body


	uint8_t i = 2 + TWI_Buffer_AtByte(TWI0_Buffer,1);
	while(TWI_Buffer_AtByte(TWI0_Buffer,i))//name sending, while name[i] != '\0'
	{
		FRAM_CircularBuffer_safePush( fram_buffer						,
									  TWI_Buffer_AtByte(TWI0_Buffer,i));
		
		++i;
	}

	FRAM_CircularBuffer_safePush( fram_buffer							 ,
								  TWI_Buffer_AtCurrentByte(TWI0_Buffer));// '\0' 
}

void COM_configureDevice()
{
	CORE_configureDebuggerFRAM(TWI_Buffer_AtByte(TWI0_Buffer,1));
}

//in pixels
#define ERROR_X_POS		100 
#define ERROR_Y_POS		150 

static inline void __transmittion_error()
{
	Paint_Clear(BLACK);
	Paint_DrawString_EN( ERROR_X_POS , 
						 ERROR_Y_POS , 
						 "ERROR!"	 ,
						 &Font16	 ,
						 BLACK		 ,
						 WHITE);
}

#define COMMAND_BYTE 0

void COM_commandProcessor()
{
	bool proccessing_flag = true;
	while(proccessing_flag)
	{
		if( TWI_Buffer_Is_DataIsReadyToProcess(TWI0_Buffer) )
		{
			switch( TWI_Buffer_AtByte(TWI0_Buffer , COMMAND_BYTE) )
			{
				case DEVICE_START:
					COM_configureDevice();
					TWI0_emptyBuffer();
					break;
				case SENDING_VARIABLE:
					COM_getVariable();
					TWI0_emptyBuffer();
					break;
				case END_TRANSACTION:
					TWI0_emptyBuffer();
					proccessing_flag = false;
					break;
				default:
					__transmittion_error();
					while(1);
			}
		}
	}
}

void COM_initVariableBuffer()
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		var_buffer.raw_name_text_     = calloc( NAME_MAX_LENGTH , sizeof(char) );
		var_buffer.value_		      = 0;
		var_buffer.current_text_pos_  = 0;
		var_buffer.is_empty_		  = true;
	}
}

void COM_getVariableData()
{
	static uint8_t  sb_idx		= 0;
	static uint8_t  var_size	= 0;
	static uint8_t  helper_var	= 0;
	
	//reset local
	sb_idx		= 0;
	var_size	= 0;
	helper_var	= 0;
	
	//reset var buffer
	var_buffer.value_			  = 0;
	var_buffer.raw_name_text_[0]  = '\0';
	var_buffer.current_text_pos_  = 0;
	var_buffer.is_empty_		  = false;
	
	FRAM_CircularBuffer_pop(fram_buffer,&var_size);//get var size
	
	//value collecting
	for_N(i,var_size)
	{
		FRAM_CircularBuffer_pop(fram_buffer,&helper_var);
		var_buffer.value_ |= helper_var << ( 8 * i ) ;//8-bit , 16-bit ...
	}
	
	FRAM_CircularBuffer_pop(fram_buffer,&var_buffer.raw_name_text_[sb_idx]);//get first char of the var name
	
	//the rest of the name
	while(var_buffer.raw_name_text_[sb_idx++])
		FRAM_CircularBuffer_pop(fram_buffer,&var_buffer.raw_name_text_[sb_idx]);
}

