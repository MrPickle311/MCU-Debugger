

#ifndef UNIT_TESTS_
#define UNIT_TESTS_

#include <stdbool.h>
#include <stdint.h>
#include "../../drivers/core/bit_utils.h"
#include "framework_flash_strings.h"


#define TEST_UART_SEND_TXT(txt_ptr)	
#define TEST_UART_SEND_BYTE(byte)	

enum GlobalCommand{ START					 = 0 ,
					SENDING_TEST_CASE        = 1 ,
					SENDING_UNIT_TEST_RESULT = 2 ,
					END_ENTIRE_TRANSACTION   = 3 };

enum TestCaseCommand{ SENDING_NAME			  = 0,
					  SENDING_TYPE_DESCRIPTOR = 1,
					  SENDING_CURRENT_VALUE   = 2,
					  SENDING_EXPECTED_VALUE  = 3,
					  SENDING_TEST_RESULT     = 4,
					  SENDING_LINE_NMBR		  = 5,
					  END_SENDING_TEST		  = 6 };

enum TypeDescriptor{ UINT8_T  = 0 , 
					 UINT16_T = 1 , 
					 UINT32_T = 2 , 
					 UINT64_T = 3 , 
					 
					 INT8_T   = 4 , 
					 INT16_T  = 5 , 
					 INT32_T  = 6 , 
					 INT64_T  = 7 ,
					 
					 BOOL     = 8 , 
					 CHAR     = 9 ,
					 TYPES_COUNT};


extern const __flash uint8_t arrai[TYPES_COUNT];

#define GET_EXPRESSION_NAME(expression,target)	sprintf(target, "%s",expression)

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

void uartTerminateSendingTestCase();

//specific in unit test END

/*

#define DEFINE_TEST_RESULT(type)\
struct TEST_RESULT_##type \
{\
	const char __memx*  expresion_name_; \
	enum TypeDescriptor	type_descriptor_; \
	type			    current_value_; \
	type			    expected_value_; \
	bool			    is_test_passed_; \
	uint16_t		    line_nmbr_; \
};\
typedef struct TEST_RESULT_##type TEST_RESULT_##type;


DEFINE_TEST_RESULT(uint8_t);
DEFINE_TEST_RESULT(uint16_t);
DEFINE_TEST_RESULT(uint32_t);
DEFINE_TEST_RESULT(uint64_t);
DEFINE_TEST_RESULT(int8_t);
DEFINE_TEST_RESULT(int16_t);
DEFINE_TEST_RESULT(int32_t);
DEFINE_TEST_RESULT(int64_t);
DEFINE_TEST_RESULT(bool);
DEFINE_TEST_RESULT(char);

void* allocateUnitTestResult(const char __memx*		expresion_name	,
							 enum TypeDescriptor	type_descriptor ,
							 void*					current_value   ,
							 void*					expected_value  ,
							 bool					is_test_passed  ,
							 uint16_t				line_nmbr		
							 );

struct UNIT_TEST
{
	void*			  test_results_;
	struct UNIT_TEST* next_test_;
};

typedef struct UNIT_TEST UNIT_TEST;

struct TEST_CASE_RESULT
{
	const char __memx* test_case_name_;
	uint8_t			   unit_tests_count_;
	UNIT_TEST*		   current_unit_test_;
	UNIT_TEST*		   unit_test_head_;
}current_test_case;

typedef struct TEST_CASE_RESULT TEST_CASE_RESULT;

void initTestCaseResult(TEST_CASE_RESULT* test_case_result , const char __memx* test_case_name);

void clearTestCaseResult(TEST_CASE_RESULT* test_case_result);

void sendResultPackage(TEST_CASE_RESULT*   test_case_result);

void pushNewUnitTest( TEST_CASE_RESULT*   test_case_result , 
					  UNIT_TEST*		  unit_test );

*/

#define TEST_INIT()\
		uartSendGlobalCommand(START)

#define TEST_CASE(name)\
		void name()

#define EXAMINE_TEST_CASE(name)\
		uartSendGlobalCommand(SENDING_TEST_CASE);\
		uartSendText( PSTR( ##name ) );\
		name();

#define TEST_END()\
		uartSendGlobalCommand(END_ENTIRE_TRANSACTION)



#define EXPECT_TRUE(expression)\
	if(expression)


/**
 * @Macro:       UCUNIT_Check(condition, msg, args)
 *
 * @Description: Checks a condition and prints a message.
 *
 * @Param msg:   Message to write.
 * @Param args:  Argument list as string
 *
 * @Remarks:     Basic check. This macro is used by all higher level checks.
 *
 */
#define UCUNIT_Check(condition, msg, args)             \
    if ( (condition) ) { UCUNIT_PassCheck(msg, args); } else { UCUNIT_FailCheck(msg, args); }

/**
 * @Macro:       UCUNIT_CheckIsEqual(expected,actual)
 *
 * @Description: Checks that actual value equals the expected value.
 *
 * @Param expected: Expected value.
 * @Param actual: Actual value.
 *
 * @Remarks:     This macro uses UCUNIT_Check(condition, msg, args).
 *
 */
#define UCUNIT_CheckIsEqual(expected,actual)         \
    UCUNIT_Check( (expected) == (actual), "IsEqual", #expected "," #actual )

/**
 * @Macro:       UCUNIT_CheckIsNull(pointer)
 *
 * @Description: Checks that a pointer is NULL.
 *
 * @Param pointer: Pointer to check.
 *
 * @Remarks:     This macro uses UCUNIT_Check(condition, msg, args).
 *
 */
#define UCUNIT_CheckIsNull(pointer)                  \
    UCUNIT_Check( (pointer) == NULL, "IsNull", #pointer)

/**
 * @Macro:       UCUNIT_CheckIsNotNull(pointer)
 *
 * @Description: Checks that a pointer is not NULL.
 *
 * @Param pointer: Pointer to check.
 *
 * @Remarks:     This macro uses UCUNIT_Check(condition, msg, args).
 *
 */
#define UCUNIT_CheckIsNotNull(pointer)               \
    UCUNIT_Check( (pointer) != NULL, "IsNotNull", #pointer)

/**
 * @Macro:       UCUNIT_CheckIsInRange(value, lower, upper)
 *
 * @Description: Checks if a value is between lower and upper bounds (inclusive)
 *               Mathematical: lower <= value <= upper
 *
 * @Param value: Value to check.
 * @Param lower: Lower bound.
 * @Param upper: Upper bound.
 *
 * @Remarks:     This macro uses UCUNIT_Check(condition, msg, args).
 *
 */
#define UCUNIT_CheckIsInRange(value, lower, upper)   \
    UCUNIT_Check( ( (value>=lower) && (value<=upper) ), "IsInRange", #value "," #lower "," #upper)

/**
 * @Macro:       UCUNIT_CheckIs8Bit(value)
 *
 * @Description: Checks if a value fits into 8-bit.
 *
 * @Param value: Value to check.
 *
 * @Remarks:     This macro uses UCUNIT_Check(condition, msg, args).
 *
 */

#define UCUNIT_CheckIsBitSet(value, bitno) \
    UCUNIT_Check( (1==(((value)>>(bitno)) & 0x01) ), "IsBitSet", #value "," #bitno)

/**
 * @Macro:       UCUNIT_CheckIsBitClear(value, bitno)
 *
 * @Description: Checks if a bit is not set in value.
 *
 * @Param value: Value to check.
 * @Param bitno: Bit number. The least significant bit is 0.
 *
 * @Remarks:     This macro uses UCUNIT_Check(condition, msg, args).
 *
 */
#define UCUNIT_CheckIsBitClear(value, bitno) \
    UCUNIT_Check( (0==(((value)>>(bitno)) & 0x01) ), "IsBitClear", #value "," #bitno)

#endif /*UNIT_TESTS_*/
