/*
 * com.c
 *
 * Created: 15/05/2021 22:04:11
 *  Author: Damian Wójcik
 */ 

#include "com.h"

#define SIGNAL_DURATION  2//ms 
#define PIN_2			 2

void COM_sendDebugSignal()//on port E
{
	PORT_setPinLow(PORT_STATE(E) , PIN_2);
	_delay_ms(SIGNAL_DURATION); 
	PORT_setPinHigh(PORT_STATE(E) , PIN_2);
}

#define SECOND_BYTE 1//the second byte always contains size of variable
#define TWO_BYTES   2//command byte and sizeof byte

void COM_getVariable()
{
	//get size of variable
	FRAM_CircularBuffer_safePush(fram_buffer									, 
								 TWI_Buffer_AtByte(TWI0_Buffer , SECOND_BYTE) );
	
	//lading variable bits(8,16...) to FRAM
	for_N( byte_nmbr , TWI_Buffer_AtByte( TWI0_Buffer , SECOND_BYTE ))
		FRAM_CircularBuffer_safePush( fram_buffer									,
									  TWI_Buffer_AtByte(TWI0_Buffer, TWO_BYTES + byte_nmbr ));
	
	TWI_Buffer_GoNextByte(TWI0_Buffer);//ommit a command
	TWI_Buffer_GoNextByte(TWI0_Buffer);//ommit a variable size
	
	for_N(i,TWI_Buffer_AtByte(TWI0_Buffer,1))
		TWI_Buffer_GoNextByte(TWI0_Buffer);//ommit a variable body

	//jump over command byte + sizeof byte + variable bytes
	uint8_t name_pos = TWO_BYTES + TWI_Buffer_AtByte( TWI0_Buffer , SECOND_BYTE);
	
	while(TWI_Buffer_AtByte( TWI0_Buffer , name_pos ))//name sending, while name[i] != '\0'
	{
		FRAM_CircularBuffer_safePush( fram_buffer						,
									  TWI_Buffer_AtByte( TWI0_Buffer , name_pos));
		
		++name_pos;
	}
	
	// loading terminating '\0' to FRAM 
	FRAM_CircularBuffer_safePush( fram_buffer							 ,
								  TWI_Buffer_AtCurrentByte(TWI0_Buffer));
}

void COM_configureDevice()
{
	CORE_configureDebuggerFRAM(TWI_Buffer_AtByte(TWI0_Buffer,1));
}

//in pixels
#define ERROR_X_POS		100 
#define ERROR_Y_POS		150 

//TWI/I2C bus critical error
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
	bool proccessing_flag = true;//switch in while, so i need this flag
	while(proccessing_flag)		 //to break switch
	{
		if( TWI_Buffer_Is_DataIsReadyToProcess(TWI0_Buffer) )
		{
			//first byte is a byte which contains a command 
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
		var_buffer.raw_name_text_      = calloc( NAME_MAX_LENGTH , sizeof(char) );
		var_buffer.value_		       = 0;
		var_buffer.current_text_pos_   = 0;
		var_buffer.breakpoints_loaded_ = 0;
		//var_buffer.is_empty_		  = true;
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
	//var_buffer.is_empty_		  = false;
	
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

