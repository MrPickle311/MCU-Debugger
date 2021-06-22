/*
 * ext_twi.h
 *
 * Created: 07/05/2021 23:37:48
 *  Author: Damian Wójcik
 */ 


#ifndef EXT_TWI_H_
#define EXT_TWI_H_

#include <avr/builtins.h>
#include "../core/global_utils.h"
#include "../core/twi_master.h"

#define FRAM_ADR 0b1010000 //by default this address is not shifted 


volatile byte_t __fram_local_buffer__ ;

void FRAM_writeSingleByte(byte_t byte, large_length_t address);

void FRAM_writeByteSequence(byte_t* bytes,length_t size,large_length_t start_address);

byte_t FRAM_readPreviousByte(address_16bit_t address);

byte_t FRAM_readRandomByte(large_length_t address);

void FRAM_readByteSequence(byte_t* target,length_t size,large_length_t start_address);

void FRAM_readText(char* target,large_length_t start_address);

#endif /* EXT_TWI_H_ */