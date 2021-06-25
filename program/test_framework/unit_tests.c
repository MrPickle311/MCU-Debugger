/*
 * unit_tests.c
 *
 * Created: 24/06/2021 15:04:25
 *  Author: Damian Wójcik
 */ 

#include "unit_tests.h"

//generic

void uartSendGlobalCommand(enum GlobalCommand cmd)
{
	TEST_UART_SEND_BYTE(cmd);
}

void uartSendTestCaseCommand(enum TestCaseCommand cmd)
{
	TEST_UART_SEND_BYTE(cmd);
}

void uartSendText(const char __memx* txt)
{
	TEST_UART_SEND_TXT(txt);
}

byte_t __get_nth_byte_of_data(void* data_source , uint8_t byte_nmbr)
{
	return (byte_t)( ( *(uint64_t*)data_source ) >>  ( byte_nmbr * 8 ) );
}

void __send_array(byte_t* array , uint8_t size)
{
	for_N( byte_nmbr , size )
		TEST_UART_SEND_BYTE(array[byte_nmbr]);
}

void uartSendData(void* data_source , uint8_t data_size)
{
	byte_t* byte_array = calloc(data_size , sizeof (byte_t));
	
	for_N( byte_nmbr , data_size )
		byte_array[byte_nmbr] = __get_nth_byte_of_data(data_source , byte_nmbr);
	
	__send_array(byte_array , data_size);
	
	free(byte_array);
}

//generic END

//specific in unit test

void uartSendExpressionName(const char __memx* name)
{
	uartSendTestCaseCommand(SENDING_NAME);
	uartSendText(name);
}

void uartSendTypeDescriptor(enum TypeDescriptor descriptor)
{
	uartSendTestCaseCommand(SENDING_TYPE_DESCRIPTOR);
	TEST_UART_SEND_BYTE(descriptor);
}

void uartSendCurrentValue(void* current_value , uint8_t value_size)
{
	uartSendTestCaseCommand(SENDING_CURRENT_VALUE);
	uartSendData(current_value , value_size);
}

void uartSendExpectedValue(void* expected_value , uint8_t value_size)
{
	uartSendTestCaseCommand(SENDING_EXPECTED_VALUE);
	uartSendData(expected_value , value_size);
}

void uartSendTestResult(bool is_passed)
{
	uartSendTestCaseCommand(SENDING_TEST_RESULT);
	TEST_UART_SEND_BYTE(is_passed);
}

void uartSendLineNumber(uint16_t line_nmbr)
{
	uartSendTestCaseCommand(SENDING_LINE_NMBR);
	uartSendData(&line_nmbr , sizeof(uint16_t)  );	
}

void uartTerminateSendingUnitTestResult()
{
	uartSendTestCaseCommand(END_SENDING_UNIT_TEST_RESULT);
}

void uartSendLowerValue(void *lower_value, uint8_t value_size)
{
	uartSendTestCaseCommand(SENDING_LOWER_VALUE);
	uartSendData(lower_value , value_size  );
}

void uartSendUpperValue(void *lower_value, uint8_t value_size)
{
	uartSendTestCaseCommand(SENDING_UPPER_VALUE);
	uartSendData(lower_value , value_size  );
}



//specific in unit test END

//compressed functions

void uartSendUnitTestResultHeader(const char __memx* name , enum TypeDescriptor descrpitor)
{
	uartSendGlobalCommand(SENDING_UNIT_TEST_RESULT);
	uartSendExpressionName( name );
	uartSendTypeDescriptor( descrpitor );
}

void uartSendUnitTestResultValues(void* current_value , void* expected_value ,uint8_t value_size)
{
	uartSendExpectedValue( expected_value , value_size );
	uartSendCurrentValue(  current_value  , value_size );
}

void uartSendValueRange(void *lower_value, void *upper_value, uint8_t value_size)
{
	uartSendLowerValue(lower_value , value_size);
	uartSendUpperValue(upper_value , value_size);
}

bool checkCondition(bool condition)
{
	return condition == true;
}

//compressed functions END
