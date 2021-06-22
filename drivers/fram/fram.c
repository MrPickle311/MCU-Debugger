/*
 * fram.c
 *
 * Created: 15/05/2021 22:16:48
 *  Author: Damian Wójcik
 */ 

#include "fram.h"

//GPIOR0

volatile byte_t __fram_local_buffer__ = 0; //zamien na GPIOR

void FRAM_writeSingleByte(byte_t byte, large_length_t address)
{
	TWI1_startSequence_NoACK();
	TWI1_sendDeviceAddressForSending_NoACK(FRAM_ADR);
	TWI1_sendByte_NoACK((byte_t)(address >> 8));//zmien na ack ,jesli pojawia sie klopoty
	TWI1_sendByte_NoACK((byte_t)address);
	TWI1_sendByte_NoACK(byte);
	TWI1_stopSequence();
}

void FRAM_writeByteSequence(byte_t* bytes,length_t size,large_length_t start_address)
{
	uint8_t array_idx = 0;
	while(size--)
	{
		FRAM_writeSingleByte(bytes[array_idx++],start_address);
		++start_address;
	}
}

byte_t FRAM_readPreviousByte(address_16bit_t address)
{
	TWI1_startSequence_NoACK();
	TWI1_sendDeviceAddreessForReceiving_NoACK(FRAM_ADR);
	__fram_local_buffer__ = TWI1_receiveByte_NoACK();
	TWI1_stopSequence();
	return __fram_local_buffer__;
}

byte_t FRAM_readRandomByte(large_length_t address)
{
	TWI1_startSequence_NoACK();
	TWI1_sendDeviceAddressForSending_NoACK(FRAM_ADR);
	TWI1_sendByte_NoACK((byte_t)(address >> 8));//zmien na ack ,jesli pojawia sie klopoty
	TWI1_sendByte_NoACK((byte_t)(address));
	TWI1_stopSequence();
	
	TWI1_startSequence_NoACK();
	TWI1_sendDeviceAddreessForReceiving_NoACK(FRAM_ADR);
	__fram_local_buffer__ = TWI1_receiveByte_NoACK();
	TWI1_stopSequence();
	
	return __fram_local_buffer__;
}

void FRAM_readByteSequence(byte_t* target,length_t size,large_length_t start_address)
{
	uint8_t array_idx = 0;
	while(size--)
	{
		target[array_idx++] = FRAM_readRandomByte(start_address);
		++start_address;
	}
}

void FRAM_readText(char* target , large_length_t start_address)
{
	uint8_t array_idx = 0;
	target[array_idx] = (char)FRAM_readRandomByte(start_address);
	while(target[array_idx])
	{
		++array_idx;
		target[array_idx] = (char)FRAM_readRandomByte(start_address + array_idx);
	}
}
