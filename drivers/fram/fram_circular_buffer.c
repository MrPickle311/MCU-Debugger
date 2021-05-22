/*
 * fram_circular_buffer.c
 *
 * Created: 18/05/2021 09:24:27
 *  Author: Damian Wójcik
 */ 

#include "fram_circular_buffer.h"
#include <stdlib.h>
#include <util/atomic.h>

struct FRAM_CircularBuffer_t
{
	large_length_t beg_adr_;
	large_length_t end_adr_;
	
	large_length_t current_push_adr_;
	large_length_t current_pop_adr_;
	
	bool is_full_;
};

FRAM_CircularBuffer FRAM_CircularBuffer_init(uint16_t start_address,uint16_t end_address)
{
	FRAM_CircularBuffer buffer_temp;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		buffer_temp = malloc(sizeof(FRAM_CircularBuffer_t));
	}
	
	buffer_temp->beg_adr_ = start_address;
	buffer_temp->end_adr_ = end_address;
	buffer_temp->current_push_adr_ = 0;
	buffer_temp->current_pop_adr_ = 0;
	return buffer_temp;
}

void FRAM_CircularBuffer_reconfigure(FRAM_CircularBuffer buffer,uint16_t start_address, uint16_t end_address)
{
	buffer->beg_adr_ = start_address;
	buffer->end_adr_ = end_address;
	FRAM_CircularBuffer_clear(buffer);
}

void FRAM_CircularBuffer_Release(FRAM_CircularBuffer buffer)
{
	free(buffer);
}

void FRAM_CircularBuffer_clear(FRAM_CircularBuffer buffer)
{
	buffer->current_push_adr_ = 0;
	buffer->current_pop_adr_ = 0;
	buffer->is_full_ = false;
}

//extracted pointer advancment logic
static inline void _advancePtr(FRAM_CircularBuffer buffer)
{
	if(buffer->is_full_)
		if(++buffer->current_pop_adr_ ==  FRAM_CircularBuffer_getCapacity(buffer))
			buffer->current_pop_adr_ = 0;
	
	if(++buffer->current_push_adr_ == FRAM_CircularBuffer_getCapacity(buffer))
		buffer->current_push_adr_ = 0;
	
	buffer->is_full_ = buffer->current_push_adr_ == (FRAM_CircularBuffer_getCapacity(buffer) - 1);
}

//this helper function is called when when removing a value from the buffer
static inline void _retreatPtr(FRAM_CircularBuffer buffer)
{
	buffer->is_full_ = false;
	
	if(++buffer->current_pop_adr_ == buffer->current_push_adr_)
		buffer->current_pop_adr_ = 0;
}

enum OperationStatus FRAM_CircularBuffer_safePush(FRAM_CircularBuffer buffer,byte_t data)
{
	enum OperationStatus status = Failure;
	
	if(!buffer->is_full_)
	{
		FRAM_writeSingleByte(data,buffer->beg_adr_ + buffer->current_push_adr_);
		
		_advancePtr(buffer);
		status = Success;
	}
	
	return status;
}

void FRAM_CircularBuffer_forcePush(FRAM_CircularBuffer buffer,byte_t data)
{
	FRAM_writeSingleByte(data,buffer->beg_adr_ + buffer->current_push_adr_);
	_advancePtr(buffer);
}

enum OperationStatus FRAM_CircularBuffer_pop(FRAM_CircularBuffer buffer, byte_t* dest)
{
	enum OperationStatus status = Failure;
	
	if (!FRAM_CircularBuffer_isEmpty(buffer))
	{
		*dest = FRAM_readRandomByte(buffer->beg_adr_ + buffer->current_pop_adr_);
		_retreatPtr(buffer);
		status = Success;
	}
	
	return status;
}

bool FRAM_CircularBuffer_isEmpty(FRAM_CircularBuffer buffer)
{
	return !buffer->is_full_ && buffer->current_push_adr_ == buffer->current_pop_adr_ ;
}

bool FRAM_CircularBuffer_isFull(FRAM_CircularBuffer buffer)
{
	return buffer->is_full_;
}

large_length_t FRAM_CircularBuffer_getCapacity(FRAM_CircularBuffer buffer)
{
	return buffer->end_adr_ - buffer->beg_adr_ + 1;
}

large_length_t FRAM_CircularBuffer_getFillLevel(FRAM_CircularBuffer buffer)
{
	return buffer->current_push_adr_;
}

bool FRAM_CircularBuffer_IOPosMatch(FRAM_CircularBuffer buffer)
{
	return buffer->current_pop_adr_ == buffer->current_push_adr_;
}

void FRAM_CircularBuffer_injectSavePosition(FRAM_CircularBuffer buffer,large_length_t pos)
{
	buffer->current_push_adr_ = pos;
}
