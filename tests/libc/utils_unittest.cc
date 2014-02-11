/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2014 Yanzheng Li

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************************/

/* Unit tests for functions defined in include/libc/utils.h */

#include <string.h>
#include "../_unittest.h"
#include "../../include/libc/assert.h"
#include "../../include/libc/utils.h"


/*******************************************
 * Tests for MAX(a,b)
 *******************************************/
class MAXUnitTest : public ::testing::Test {};


TEST_F(MAXUnitTest, TestMaxOfPositiveInteger)
{
  EXPECT_EQ(6, MAX(5, 6));
  EXPECT_EQ(6, MAX(0, 6));
}

TEST_F(MAXUnitTest, TestMaxOfNegativeInteger)
{
  EXPECT_EQ(-5, MAX(-5, -6));
  EXPECT_EQ(0, MAX(-5, 0));
}

TEST_F(MAXUnitTest, TestMaxOfPositiveFloatingNumber)
{
  EXPECT_EQ(880.0f, MAX(0.2f, 880.0f));
  EXPECT_EQ(0.1f, MAX(0.0f, 0.1f));
}

TEST_F(MAXUnitTest, TestMaxOfNegativeFloatingNumber)
{
  EXPECT_EQ(-5.0f, MAX(-5.0f, -6.0f));
  EXPECT_EQ(0.0f, MAX(-5.0f, 0.0f));
}


/*******************************************
 * Tests for MIN(a,b)
 *******************************************/
class MINUnitTest : public ::testing::Test {};


TEST_F(MINUnitTest, TestMinOfPositiveInteger)
{
  EXPECT_EQ(5, MIN(5,6));
  EXPECT_EQ(0, MIN(5,0));
}

TEST_F(MINUnitTest, TestMinOfNegativeInteger)
{
  EXPECT_EQ(-6, MIN(-5, -6));
  EXPECT_EQ(-6, MIN(0, -6));
}

TEST_F(MINUnitTest, TestMinOfPositiveFloatingNumber)
{
  EXPECT_EQ(5.0f, MIN(5.0f, 6.0f));
  EXPECT_EQ(0.0f, MIN(0.0f, 5.0f));
}

TEST_F(MINUnitTest, TestMinOfNegativeFloatingNumber)
{
  EXPECT_EQ(-6.0f, MIN(-5.0f, -6.0f));
  EXPECT_EQ(-6.0f, MIN(0.0f, -6.0f));
}


/*******************************************
 * Unit test for:
 * RETURN_IF_NULL(val)
 *******************************************/
class ReturnIfNullUnitTest : public ::testing::Test {};


TEST_F(ReturnIfNullUnitTest, TestPassNull)
{
  char *val = NULL;
  RETURN_IF_NULL(val);
  ASSERT_EQ(1, 2);
}

TEST_F(ReturnIfNullUnitTest, TestPassNotNull)
{
  char *val = (char*)malloc(sizeof(char));
  ASSERT_TRUE(val != NULL);
  RETURN_IF_NULL(val);
  free(val);
}


/*******************************************
 * Unit test for:
 * RETURN_IF_TRUE(val)
 *******************************************/
class ReturnIfTrueUnitTest : public ::testing::Test {};


TEST_F(ReturnIfTrueUnitTest, TestPassTrue)
{
  int val = 1 == 1;
  RETURN_IF_TRUE(val);
  ASSERT_EQ(1, 2);
}

TEST_F(ReturnIfTrueUnitTest, TestPassFalse)
{
  int val = 1 == 2;
  RETURN_IF_TRUE(val);
  ASSERT_EQ(1, 1);
}


/*******************************************
 * Tests for:
 * RETURN_IF_FALSE(val)
 *******************************************/
class ReturnIfFalseUnitTest : public ::testing::Test {};


TEST_F(ReturnIfFalseUnitTest, TestPassTrue)
{
  int val = 1 == 1;
  RETURN_IF_FALSE(val);
  ASSERT_EQ(1, 1);
}

TEST_F(ReturnIfFalseUnitTest, TestPassFalseTest)
{
  int val = 1 == 2;
  RETURN_IF_FALSE(val);
  ASSERT_EQ(1, 2);
}


/*******************************************
 * Unit test for:
 * RETURN_IF_EQUALS(val1, val2)
 *******************************************/
class ReturnIfEqualsUnitTest : public ::testing::Test {};


TEST_F(ReturnIfEqualsUnitTest, TestPassTrue)
{
  int val1 = 1;
  int val2 = 1;
  RETURN_IF_EQUALS(val1, val2);
  ASSERT_EQ(1, 2);
}

TEST_F(ReturnIfEqualsUnitTest, TestPassFalse)
{
  int val1 = 1;
  int val2 = 2;
  RETURN_IF_EQUALS(val1, val2);
  ASSERT_EQ(1, 1);
}


/*******************************************
 * Unit tests for:
 * void set_nth_bit(int *val, char bit) 
 *******************************************/
class SetNthBitUnitTest : public ::testing::Test {
public:
  void test(int, int);
};


void
SetNthBitUnitTest::test(int bit, int expected_value)
{
  int val = 0;
  set_nth_bit(&val, bit);
  EXPECT_EQ(expected_value, val);
}


TEST_F(SetNthBitUnitTest, TestSet1stBit)
{
  test(1, 0x0001);
}

TEST_F(SetNthBitUnitTest, TestSet2ndBit)
{
  test(2, 0x0002);
}

TEST_F(SetNthBitUnitTest, TestSet4thBit)
{
  test(4, 0x0008);
}

TEST_F(SetNthBitUnitTest, TestSet8thBit)
{
  test(8, 128);
}

TEST_F(SetNthBitUnitTest, TestSet16thBit)
{
  test(16, 32768);
}

TEST_F(SetNthBitUnitTest, TestSet32thBit)
{
  test(32, -(1 << 31));
}


/*******************************************
 * Unit tests for:
 * void clear_nth_bit(int *val, char bit) 
 *******************************************/
class ClearNthBitUnitTest : public ::testing::Test {
public:
  void test(int, int, int=0);
};


void
ClearNthBitUnitTest::test(int value, int bit, int expected_value)
{
  clear_nth_bit(&value, bit);
  EXPECT_EQ(expected_value, value);
}


TEST_F(ClearNthBitUnitTest, TestClear1stBit)
{
  test(0x0001, 1);
}

TEST_F(ClearNthBitUnitTest, TestClear2ndBit)
{
  test(0x0002, 2);
}

TEST_F(ClearNthBitUnitTest, TestClear4thBit)
{
  test(0x0008, 4);
}

TEST_F(ClearNthBitUnitTest, TestClear8thBit)
{
  test(128, 8);
}

TEST_F(ClearNthBitUnitTest, TestClear16thBit)
{
  test(32768, 16);
}

TEST_F(ClearNthBitUnitTest, TestClear32thBit)
{
  test(-(1 << 31), 32);
}


/*******************************************
 * Unit test for:
 * void is_bit_set(int val, char bit) 
 *******************************************/
class IsBitSetUnitTest : public ::testing::Test {};


TEST_F(IsBitSetUnitTest, TestBitSetOnZeroValueInteger)
{
  EXPECT_FALSE(is_bit_set(0, 1));
}

TEST_F(IsBitSetUnitTest, TestSetOn1stBit)
{
  EXPECT_TRUE(is_bit_set(1, 1));
}

TEST_F(IsBitSetUnitTest, TestSetOn2ndBit)
{
  int val = 2;
  EXPECT_FALSE(is_bit_set(val, 1));
  EXPECT_TRUE(is_bit_set(val, 2));
}

TEST_F(IsBitSetUnitTest, TestSetOn4thBit)
{
  int val = 8;
  EXPECT_FALSE(is_bit_set(val, 1));
  EXPECT_FALSE(is_bit_set(val, 2));
  EXPECT_TRUE(is_bit_set(val, 4));
}

TEST_F(IsBitSetUnitTest, TestSetOn8thBit)
{
  int val = 128;
  EXPECT_FALSE(is_bit_set(val, 1));
  EXPECT_FALSE(is_bit_set(val, 2));
  EXPECT_FALSE(is_bit_set(val, 4));
  EXPECT_TRUE(is_bit_set(val, 8));
}

TEST_F(IsBitSetUnitTest, TestSetOn16thBit)
{
  int val = 32768;
  EXPECT_FALSE(is_bit_set(val, 1));
  EXPECT_FALSE(is_bit_set(val, 2));
  EXPECT_FALSE(is_bit_set(val, 4));
  EXPECT_FALSE(is_bit_set(val, 8));
  EXPECT_TRUE(is_bit_set(val, 16));
}

TEST_F(IsBitSetUnitTest, TestSetOn32thBit)
{ 
  int val = -(1 << 31);
  EXPECT_FALSE(is_bit_set(val, 1));
  EXPECT_FALSE(is_bit_set(val, 2));
  EXPECT_FALSE(is_bit_set(val, 4));
  EXPECT_FALSE(is_bit_set(val, 8));
  EXPECT_FALSE(is_bit_set(val, 16));
  EXPECT_TRUE(is_bit_set(val, 32));
}


/*******************************************
 * Unit Test for:
 * rand_top(int top)
 *******************************************/
class RandTopUnitTest : public ::testing::Test {};


TEST_F(RandTopUnitTest, TestRandomness)
{
  int i;
  for(i = 1; i <= 5000; i++) {
    ASSERT_LE(rand_top(i), i);
  }
}

/*******************************************
 * Unit Test for:
 * rand_range(int min, int max)
 *******************************************/
class RandRangeUnitTest : public ::testing::Test {};


TEST_F(RandRangeUnitTest, TestRandomness)
{
  int i;
  int min = 20;
  for(i = min; i <= 5000; i++) {
    int _rand = rand_range(min, i);
    ASSERT_LE(min, _rand);
    ASSERT_GE(i, _rand);
  }
}

/*******************************************
 * Unit Tests for:
 * randf_top(double top)
 *******************************************/
class RandFTopUnitTest : public ::testing::Test {};


TEST_F(RandFTopUnitTest, TestRandomness)
{
  int i;
  for(i = 1.0f; i <= 5000.0f; i++) {
    ASSERT_LE(rand_top(i), i);
  }
}

/*******************************************
 * Unit Tests for:
 * randf_range(double min, double max)
 *******************************************/
class RandFRangeUnitTest : public ::testing::Test {};


TEST_F(RandFRangeUnitTest, TestRandomness)
{
  double i;
  double min = 20.0f;
  for(i = min; i <= 5000.0f; i++) {
    double _rand = randf_range(min, i);
    ASSERT_LE(min, _rand);
    ASSERT_GE(i, _rand);
  }
}

/*******************************************
 * Tests for:
 * generate_text(size_t len, size_t max) 
 *******************************************/
class GenerateTextUnitTest : public ::testing::Test {};


TEST_F(GenerateTextUnitTest, TestPassNonZeroLenAndMax)
{
  char *text = generate_text(6, 12);
  ASSERT(text);
  ASSERT_LE(6, strlen(text));
  ASSERT_GE(12, strlen(text));
}

TEST_F(GenerateTextUnitTest, TestPassZeroMinAndMax)
{
  char *text = generate_text(0, 0);
  ASSERT(text == NULL);
}

TEST_F(GenerateTextUnitTest, TestPassNonZeroLenAndZeroMax)
{
  int i;
  for(i = 1; i <= 5000; i++) {
    char *text = generate_text(i, 0);
    ASSERT(text);
    ASSERT_EQ(i, strlen(text));
    free(text);
  }
}

TEST_F(GenerateTextUnitTest, TestPassZeroLenAndNonZeroMax)
{
  int i;
  for(i = 1; i <= 5000; i++) {
    char *text = generate_text(0, i);
    ASSERT(text);
    ASSERT_GE(i, strlen(text));
    free(text);
  }
}

/*******************************************
 * Unit Tests for:
 * generate_loremipsum()
 *******************************************/
class GenerateLoremIpsumUnitTest : public ::testing::Test {};


TEST_F(GenerateLoremIpsumUnitTest, TestGenerateLoremIpsum)
{
  char *ipsum = generate_loremipsum();
  ASSERT(ipsum);
  ASSERT_GE(strlen(ipsum), 0);
  free(ipsum);
}