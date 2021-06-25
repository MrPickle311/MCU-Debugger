

#ifndef UNIT_TESTS_
#define UNIT_TESTS_

#include <stdbool.h>
#include <stdint.h>
#include "../../drivers/core/bit_utils.h"
#include "../../drivers/core/global_utils.h"

#define TEST_UART_SEND_TXT(txt_ptr)	
#define TEST_UART_SEND_BYTE(byte)	

enum GlobalCommand{ START					 = 0 ,
					SENDING_TEST_CASE        = 1 ,
					SENDING_UNIT_TEST_RESULT = 2 ,
					END_SENDING_TEST_CASE    = 3 ,
					END_ENTIRE_TRANSACTION   = 4 };

enum TestCaseCommand{ SENDING_NAME					= 0 ,
                      SENDING_TYPE_DESCRIPTOR		= 1 ,
                      SENDING_CURRENT_VALUE			= 2 ,
                      SENDING_EXPECTED_VALUE		= 3 ,
                      SENDING_TEST_RESULT			= 4 ,
                      SENDING_LINE_NMBR				= 5 ,
                      SENDING_LOWER_VALUE           = 6 ,
                      SENDING_UPPER_VALUE           = 7 ,
                      END_SENDING_UNIT_TEST_RESULT	= 8 };

enum TypeDescriptor{ UINT8_T  = 0  , 
					 UINT16_T = 1  , 
					 UINT32_T = 2  , 
					 UINT64_T = 3  , 
					 			   
					 INT8_T   = 4  , 
					 INT16_T  = 5  , 
					 INT32_T  = 6  , 
					 INT64_T  = 7  ,
					 			   
					 BOOL     = 8  , 
					 CHAR     = 9  ,
					 PTR	  = 10 ,
					 BIT	  = 11 , 
					 TYPES_COUNT};

//generic

void uartSendGlobalCommand(enum GlobalCommand cmd);

void uartSendTestCaseCommand(enum TestCaseCommand cmd);

void uartSendText(const char __memx* txt);

void uartSendData(void* data_source , uint8_t data_size );

//generic END

//specific in unit test

void uartSendExpressionName(const char __memx* name);

void uartSendTypeDescriptor(enum TypeDescriptor descriptor);

void uartSendCurrentValue(void* current_value , uint8_t value_size );

void uartSendExpectedValue(void* expected_value , uint8_t value_size);

void uartSendTestResult(bool is_passed);

void uartSendLineNumber(uint16_t line_nmbr);

void uartTerminateSendingUnitTestResult();

void uartSendLowerValue(void* lower_value, uint8_t value_size);

void uartSendUpperValue(void* lower_value, uint8_t value_size);

//specific in unit test END

//compressed functions

void uartSendUnitTestResultHeader( const char __memx* name , enum TypeDescriptor descrpitor );

void uartSendUnitTestResultValues(void* current_value , void* expected_value ,uint8_t value_size );

void uartSendValueRange(void* lower_value , void* upper_value , uint8_t value_size);

bool checkCondition(bool condition);


#define TEST_INIT()\
		uartSendGlobalCommand(START)

#define TEST_CASE(name)\
		void name()

#define EXAMINE_TEST_CASE(name)\
		uartSendGlobalCommand(SENDING_TEST_CASE);\
		uartSendText( PSTR( #name ) );\
		name();

#define TEST_CASE_END()\
		uartSendGlobalCommand(END_SENDING_TEST_CASE)

#define TEST_PROTOCOL_END()\
		uartSendGlobalCommand(END_ENTIRE_TRANSACTION)


#define BOOL_BODY(expression, expected_value)\
		uartSendUnitTestResultHeader( PSTR( #expression ) , BOOL );\
		uartSendUnitTestResultValues( &(bool){expression} , &(bool){true} , sizeof(bool) );\
		uartSendTestResult( checkCondition( expression == expected_value ) );\
		uartTerminateSendingUnitTestResult()
		
#define EXPECT_TRUE(expression) BOOL_BODY(expression , true)
		
#define EXPECT_FALSE(expression) BOOL_BODY(expression , false)


#define COMPARISON_BODY( expression , expected_value , type_descriptor , type , operator)\
		uartSendUnitTestResultHeader( PSTR( #expression ) , type_descriptor );\
		uartSendUnitTestResultValues( &(type){expression} , &(type){expected_value} , sizeof(type) );\
		uartSendTestResult( checkCondition( expression operator expected_value ) );\
		uartTerminateSendingUnitTestResult()

#define EXPECT_EQUAL( expression , expected_value , type_descriptor , type )\
		COMPARISON_BODY( expression , expected_value , type_descriptor , type , ==)

#define EXPECT_NOT_EQUAL( expression , expected_value , type_descriptor , type )\
		COMPARISON_BODY( expression , expected_value , type_descriptor , type , !=)



//optimization
#define PTR_SIZE	 1
#define PTR_NOT_NULL 1
#define PTR_NULL	 0

#define NULL_BODY( expression , expected , operator )\
		uartSendUnitTestResultHeader( PSTR( #expression ) , PTR );\
		uartSendExpectedValue( expected , PTR_SIZE );\
		uartSendTestResult( checkCondition( expression operator NULL ) );\
		uartTerminateSendingUnitTestResult()

//pointers

#define EXPECT_NULL( expression ) NULL_BODY(expression , PTR_NULL  , ==)

#define EXPECT_NOT_NULL( expression ) NULL_BODY(expression , PTR_NOT_NULL , !=)

//pointers END

//equality

#define EXPECT_EQUAL_uint8_t( expression , expected_value )\
		EXPECT_EQUAL( expression , expected_value , UINT8_T , uint8_t )

#define EXPECT_EQUAL_uint16_t( expression , expected_value )\
		EXPECT_EQUAL( expression , expected_value , UINT16_T , uint16_t )

#define EXPECT_EQUAL_uint32_t( expression , expected_value )\
		EXPECT_EQUAL( expression , expected_value , UINT32_T , uint32_t )

#define EXPECT_EQUAL_uint64_t( expression , expected_value )\
		EXPECT_EQUAL( expression , expected_value , UINT64_T , uint64_t )



#define EXPECT_EQUAL_int8_t( expression , expected_value )\
		EXPECT_EQUAL( expression , expected_value , INT8_T ,  int8_t )

#define EXPECT_EQUAL_int16_t( expression , expected_value )\
		EXPECT_EQUAL( expression , expected_value , INT16_T , int16_t )

#define EXPECT_EQUAL_int32_t( expression , expected_value )\
		EXPECT_EQUAL( expression , expected_value , INT32_T , int32_t )

#define EXPECT_EQUAL_int64_t( expression , expected_value )\
		EXPECT_EQUAL( expression , expected_value , INT64_T , int64_t )
		


#define EXPECT_NOT_EQUAL_char( expression , expected_value )\
		EXPECT_NOT_EQUAL( expression , expected_value , CHAR , char )


#define EXPECT_NOT_EQUAL_uint8_t( expression , expected_value )\
		EXPECT_NOT_EQUAL( expression , expected_value , UINT8_T , uint8_t )

#define EXPECT_NOT_EQUAL_uint16_t( expression , expected_value )\
		EXPECT_NOT_EQUAL( expression , expected_value , UINT16_T , uint16_t )

#define EXPECT_NOT_EQUAL_uint32_t( expression , expected_value )\
		EXPECT_NOT_EQUAL( expression , expected_value , UINT32_T , uint32_t )

#define EXPECT_NOT_EQUAL_uint64_t( expression , expected_value )\
		EXPECT_NOT_EQUAL( expression , expected_value , UINT64_T , uint64_t )



#define EXPECT_NOT_EQUAL_int8_t( expression , expected_value )\
		EXPECT_NOT_EQUAL( expression , expected_value , INT8_T ,  int8_t )

#define EXPECT_NOT_EQUAL_int16_t( expression , expected_value )\
		EXPECT_NOT_EQUAL( expression , expected_value , INT16_T , int16_t )

#define EXPECT_NOT_EQUAL_int32_t( expression , expected_value )\
		EXPECT_NOT_EQUAL( expression , expected_value , INT32_T , int32_t )

#define EXPECT_NOT_EQUAL_int64_t( expression , expected_value )\
		EXPECT_NOT_EQUAL( expression , expected_value , INT64_T , int64_t )
		


#define EXPECT_NOT_EQUAL_char( expression , expected_value )\
		EXPECT_NOT_EQUAL( expression , expected_value , CHAR , char )	

//equality END

//range checking 


#define RANGE_BODY(expression, lower , upper , type_descriptor , type )\
        uartSendUnitTestResultHeader(  #expression  , type_descriptor );\
        uartSendCurrentValue(&(type){expression}, sizeof(type));\
        uartSendValueRange( &(type){lower} , &(type){upper} , sizeof(type));\
        uartSendTestResult( checkCondition( ( expression >= lower ) && ( expression <= upper ) ) );\
        uartTerminateSendingUnitTestResult()

#define EXPTECT_IN_RANGE_uint8_t(expression, lower , upper)\
		RANGE_BODY(expression, lower , upper, UINT8_T , uint8_t)

#define EXPTECT_IN_RANGE_uint16_t(expression, lower , upper)\
		RANGE_BODY(expression, lower , upper, UINT16_T , uint16_t)

#define EXPTECT_IN_RANGE_uint32_t(expression, lower , upper)\
		RANGE_BODY(expression, lower , upper, UINT32_T , uint32_t)
		
#define EXPTECT_IN_RANGE_uint64_t(expression, lower , upper)\
		RANGE_BODY(expression, lower , upper, UINT64_T , uint64_t)


#define EXPTECT_IN_RANGE_int8_t(expression, lower , upper)\
		RANGE_BODY(expression, lower , upper, INT8_T , int8_t)

#define EXPTECT_IN_RANGE_int16_t(expression, lower , upper)\
		RANGE_BODY(expression, lower , upper, INT16_T , int16_t)

#define EXPTECT_IN_RANGE_int32_t(expression, lower , upper)\
		RANGE_BODY(expression, lower , upper, INT32_T , int32_t)
		
#define EXPTECT_IN_RANGE_int64_t(expression, lower , upper)\
		RANGE_BODY(expression, lower , upper, INT64_T , int64_t)
		
#define EXPTECT_IN_RANGE_char(expression, lower , upper)\
		RANGE_BODY(expression, lower , upper, CHAR , char)
		

//end range checking 

//cheking bit set/clear


#define BIT_SIZE	1//althought it is in-byte size
#define BIT_SET		1
#define BIT_CLEARED 0

#define BIT_BODY(expression , expected , bit_nmbr , EXPECTED_STATE_CHECKER )\
		uartSendUnitTestResultHeader( PSTR( #expression ) , BIT );\
		uartSendExpectedValue( &(uint8_t){expected} , BIT_SIZE );\
		uartSendTestResult( checkCondition( EXPECTED_STATE_CHECKER( expression , bit_nmbr ) ) );\
		uartTerminateSendingUnitTestResult()

#define EXPECT_BIT_SET(expression  , bit_nmbr)\
		BIT_BODY(expression , BIT_SET , bit_nmbr, IS_BIT_SET_AT )

#define EXPECT_BIT_CLEAR(expression  , bit_nmbr)\
		BIT_BODY(expression , BIT_CLEARED , bit_nmbr, IS_BIT_CLEARED_AT )
		

//cheking bit set/clear END

#endif /*UNIT_TESTS_*/
