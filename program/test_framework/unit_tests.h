

#ifndef UNIT_TESTS_
#define UNIT_TESTS_

#include <stdbool.h>

struct TEST_RESULT
{
	
};

typedef struct TEST_RESULT TEST_RESULT;

struct TEST_CASE_RESULT
{
	
};

typedef struct TEST_CASE_RESULT TEST_CASE_RESULT;

#define EXPECT_TRUE(expression)\
		

#define TEST_CASE(name)\
		bool name()

#define EXAMINE_TEST_CASE(name)\
		name()


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
#define UCUNIT_CheckIs8Bit(value)                      \
    UCUNIT_Check( value==(value & 0xFF), "Is8Bit", #value )

/**
 * @Macro:       UCUNIT_CheckIs16Bit(value)
 *
 * @Description: Checks if a value fits into 16-bit.
 *
 * @Param value: Value to check.
 *
 * @Remarks:     This macro uses UCUNIT_Check(condition, msg, args).
 *
 */
#define UCUNIT_CheckIs16Bit(value)                     \
    UCUNIT_Check( value==(value & 0xFFFF), "Is16Bit", #value )

/**
 * @Macro:       UCUNIT_CheckIs32Bit(value)
 *
 * @Description: Checks if a value fits into 32-bit.
 *
 * @Param value: Value to check.
 *
 * @Remarks:     This macro uses UCUNIT_Check(condition, msg, args).
 *
 */
#define UCUNIT_CheckIs32Bit(value)                     \
    UCUNIT_Check( value==(value & 0xFFFFFFFF), "Is32Bit", #value )

/**
 * Checks if bit is set
 */
/**
 * @Macro:       UCUNIT_CheckIsBitSet(value, bitno)
 *
 * @Description: Checks if a bit is set in value.
 *
 * @Param value: Value to check.
 * @Param bitno: Bit number. The least significant bit is 0.
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
