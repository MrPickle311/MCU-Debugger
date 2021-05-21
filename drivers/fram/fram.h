/*
 * ext_twi.h
 *
 * Created: 07/05/2021 23:37:48
 *  Author: Damian
 */ 


#ifndef EXT_TWI_H_
#define EXT_TWI_H_

#include <avr/builtins.h>
#include "../core/global_utils.h"
#include "../core/twi_master.h"

#define FRAM_ADR 0b1010000 //default not  shifted address


volatile byte_t __fram_local_buffer__ ;

void FRAM_writeSingleByte(byte_t byte, large_length_t address);

void FRAM_writeByteSequence(byte_t* bytes,length_t size,large_length_t start_address);

/*
void FRAM_pageWrite(byte_t* byte_array,length_t count_of_bytes ,address_16bit_t start_address)
{
	TWI0_startSequence_NoACK();
	TWI0_sendDeviceAddressForSending_ACK(FRAM_ADR);
	
	for(length_t i = 0 ; i < count_of_bytes ; ++i)
	{
		TWI0_sendByte_ACK(byte_array[i]);
	}
	
	TWI0_stopSequence();
}
*/

byte_t FRAM_readPreviousByte(address_16bit_t address);

byte_t FRAM_readRandomByte(large_length_t address);

void FRAM_readByteSequence(byte_t* target,length_t size,large_length_t start_address);

void FRAM_readText(char* target,large_length_t start_address);

/*
void FRAM_readBytesSequence(byte_t* byte_array,length_t count_of_bytes ,address_16bit_t start_address)
{
	TWI0_startSequence_NoACK();
	TWI0_sendDeviceAddressForSending_NoACK(FRAM_ADR);
	TWI0_sendByte_NoACK(((word)start_address).byte[0]);
	TWI0_sendByte_NoACK(((word)start_address).byte[1]);
	TWI0_stopSequence();
	
	TWI0_startSequence_NoACK();
	
	for(length_t i = 0 ; i < count_of_bytes  - 1; ++i)
	{
		TWCR=_BV(TWEA) | _BV(TWINT) | _BV(TWEN);
		while (!(TWCR & _BV(TWINT)));
		byte_array[i] = TWDR;
		
		//TWCR = 1 << TWEA;
	}
	
	TWCR=_BV(TWINT) | _BV(TWEN);
	while (!(TWCR & _BV(TWINT)));
	byte_array[count_of_bytes - 1] = TWDR;
	
	//byte_array[count_of_bytes - 1] = TWI0_receiveByte_NACK();
	
	TWI0_stopSequence();
}
*/

#endif /* EXT_TWI_H_ */