/*
 * unit_tests.c
 *
 * Created: 24/06/2021 15:04:25
 *  Author: Damian Wójcik
 */ 

#include "unit_tests.h"

const __flash uint8_t arrai[TYPES_COUNT] = { 
	1 ,
	2 ,
	4 ,
	8 ,
	1 ,
	2 ,
	4 ,
	8 ,
	1 ,
	1};

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
	return (byte_t)( ( *data_source ) >>  ( byte_nmbr * 8 ) );
}

void __send_array(byte_t* array , uint8_t size)
{
	for_N( byte_nmbr , size )
		TEST_UART_SEND_BYTE(array[byte_nmbr]);
}

void uartSendData(void* data_source , uint8_t data_size)
{
	byte_t byte_array[data_size];
	
	for_N( byte_nmbr , data_size )
		byte_t[byte_nmbr] = __get_nth_byte_of_data(data_source , byte_nmbr);
	
	__send_array(byte_array , data_size);
	
}

//generic END

//specific in unit test

void uartSendExpressionName(const char __memx* name)
{
	
}

void uartSendTypeDescriptor(enum TypeDescriptor descriptor)
{

}

void uartSendCurrentValue(void* current_value , uint8_t value_size)
{

}

void uartSendExpectedValue(void* expected_value , uint8_t value_size)
{

}

void uartSendTestResult(bool is_passed)
{

}

void uartSendLineNumber(uint16_t line_nmbr)
{

}

void uartTerminateSendingTestCase()
{

}

//specific in unit test END



/*

void* allocateUnitTestResult(const char __memx* expresion_name , enum TypeDescriptor type_descriptor , void* current_value , void* expected_value , bool is_test_passed , uint16_t line_nmbr)
{

}


void __reset_pointers(TEST_CASE_RESULT* test_case_result)
{
	test_case_result->current_unit_test_	 = NULL;
	test_case_result->unit_test_head_       = NULL;
	test_case_result->unit_tests_count_	 = 0;
}

void initTestCaseResult(TEST_CASE_RESULT* test_case_result , const char __memx* test_case_name)
{
	test_case_result->test_case_name_	= test_case_name;
	__reset_pointers(test_case_result);
}

void __free_memory(TEST_CASE_RESULT* test_case_result)
{
	
}

void clearTestCaseResult(TEST_CASE_RESULT* test_case_result)
{
	test_case_result->test_case_name_ = PSTR("\0");
	__reset_pointers(test_case_result);
}

void sendResultPackage(TEST_CASE_RESULT* test_case_result)
{
	//TODO
}

void pushNewUnitTest(TEST_CASE_RESULT* test_case_result , UNIT_TEST* unit_test)
{
	if(test_case_result->unit_tests_count_ == 0)
	{
		test_case_result->current_unit_test_ = unit_test;
		test_case_result->unit_test_head_    = unit_test;
		test_case_result->unit_tests_count_ = 1;
	}
	else
	{
		++test_case_result->unit_tests_count_;
		test_case_result->current_unit_test_->next_test_ = unit_test;
		test_case_result->current_unit_test_ = test_case_result->current_unit_test_->next_test_;
	}
}

*/

