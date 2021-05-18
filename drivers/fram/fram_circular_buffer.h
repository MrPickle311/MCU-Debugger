/*
 * fram_circular_buffer.h
 *
 * Created: 17/05/2021 23:51:51
 *  Author: Damian
 */ 


#ifndef FRAM_CIRCULAR_BUFFER_H_
#define FRAM_CIRCULAR_BUFFER_H_

#include "../core/global_utils.h"
#include "fram.h"

typedef struct FRAM_CircularBuffer_t FRAM_CircularBuffer_t;

typedef FRAM_CircularBuffer_t* FRAM_CircularBuffer;

FRAM_CircularBuffer FRAM_CircularBuffer_init(uint16_t start_address,uint16_t end_address);

void FRAM_CircularBuffer_reconfigure(FRAM_CircularBuffer buffer,uint16_t start_address, uint16_t end_address);

void FRAM_CircularBuffer_Release(FRAM_CircularBuffer buffer);

void FRAM_CircularBuffer_clear(FRAM_CircularBuffer buffer);

enum OperationStatus FRAM_CircularBuffer_safePush(FRAM_CircularBuffer buffer,byte_t data);

void FRAM_CircularBuffer_forcePush(FRAM_CircularBuffer buffer,byte_t data);

enum OperationStatus FRAM_CircularBuffer_pop(FRAM_CircularBuffer buffer,byte_t* dest);

bool FRAM_CircularBuffer_isEmpty(FRAM_CircularBuffer buffer);

bool FRAM_CircularBuffer_isFull(FRAM_CircularBuffer buffer);

length_t FRAM_CircularBuffer_getCapacity(FRAM_CircularBuffer buffer);

length_t FRAM_CircularBuffer_getFillLevel(FRAM_CircularBuffer buffer);

#endif /* FRAM_CIRCULAR_BUFFER_H_ */