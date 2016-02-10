/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2016 Yanzheng Li

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

/* Unit tests for functions defined in sneaker/libc/utils.h */

#include "libc/utils.h"

#include "testing/testing.h"

#include <cassert>
#include <string>


#if defined(__clang__) and __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wshift-sign-overflow"
#endif


// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit tests for MAX(a,b)
 ******************************************************************************/
class max_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(max_unittest, TestMaxOfPositiveInteger)
{
  EXPECT_EQ(6, MAX(5, 6));
  EXPECT_EQ(6, MAX(0, 6));
}

// -----------------------------------------------------------------------------

TEST_F(max_unittest, TestMaxOfNegativeInteger)
{
  EXPECT_EQ(-5, MAX(-5, -6));
  EXPECT_EQ(0, MAX(-5, 0));
}

// -----------------------------------------------------------------------------

TEST_F(max_unittest, TestMaxOfPositiveFloatingNumber)
{
  EXPECT_EQ(880.0f, MAX(0.2f, 880.0f));
  EXPECT_EQ(0.1f, MAX(0.0f, 0.1f));
}

// -----------------------------------------------------------------------------

TEST_F(max_unittest, TestMaxOfNegativeFloatingNumber)
{
  EXPECT_EQ(-5.0f, MAX(-5.0f, -6.0f));
  EXPECT_EQ(0.0f, MAX(-5.0f, 0.0f));
}

// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit tests for MIN(a,b)
 ******************************************************************************/
class min_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(min_unittest, TestMinOfPositiveInteger)
{
  EXPECT_EQ(5, MIN(5,6));
  EXPECT_EQ(0, MIN(5,0));
}

// -----------------------------------------------------------------------------

TEST_F(min_unittest, TestMinOfNegativeInteger)
{
  EXPECT_EQ(-6, MIN(-5, -6));
  EXPECT_EQ(-6, MIN(0, -6));
}

// -----------------------------------------------------------------------------

TEST_F(min_unittest, TestMinOfPositiveFloatingNumber)
{
  EXPECT_EQ(5.0f, MIN(5.0f, 6.0f));
  EXPECT_EQ(0.0f, MIN(0.0f, 5.0f));
}

// -----------------------------------------------------------------------------

TEST_F(min_unittest, TestMinOfNegativeFloatingNumber)
{
  EXPECT_EQ(-6.0f, MIN(-5.0f, -6.0f));
  EXPECT_EQ(-6.0f, MIN(0.0f, -6.0f));
}

// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit test for:
 * RETURN_IF_NULL(val)
 ******************************************************************************/
class return_if_null_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(return_if_null_unittest, TestPassNull)
{
  char *val = NULL;
  RETURN_IF_NULL(val);
  ASSERT_EQ(1, 2);
}

// -----------------------------------------------------------------------------

TEST_F(return_if_null_unittest, TestPassNotNull)
{
  char *val = (char*)malloc(sizeof(char));
  ASSERT_TRUE(val != NULL);
  RETURN_IF_NULL(val);
  free(val);
}

// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit test for:
 * RETURN_IF_TRUE(val)
 ******************************************************************************/
class return_if_true_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(return_if_true_unittest, TestPassTrue)
{
  int val = 1 == 1;
  RETURN_IF_TRUE(val);
  ASSERT_EQ(1, 2);
}

// -----------------------------------------------------------------------------

TEST_F(return_if_true_unittest, TestPassFalse)
{
  int val = 1 == 2;
  RETURN_IF_TRUE(val);
  ASSERT_EQ(1, 1);
}

// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit test for:
 * RETURN_IF_FALSE(val)
 ******************************************************************************/
class return_if_false_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(return_if_false_unittest, TestPassTrue)
{
  int val = 1 == 1;
  RETURN_IF_FALSE(val);
  ASSERT_EQ(1, 1);
}

// -----------------------------------------------------------------------------

TEST_F(return_if_false_unittest, TestPassFalseTest)
{
  int val = 1 == 2;
  RETURN_IF_FALSE(val);
  ASSERT_EQ(1, 2);
}

// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit test for:
 * RETURN_IF_EQUALS(val1, val2)
 ******************************************************************************/
class return_if_equals_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(return_if_equals_unittest, TestPassTrue)
{
  int val1 = 1;
  int val2 = 1;
  RETURN_IF_EQUALS(val1, val2);
  ASSERT_EQ(1, 2);
}

// -----------------------------------------------------------------------------

TEST_F(return_if_equals_unittest, TestPassFalse)
{
  int val1 = 1;
  int val2 = 2;
  RETURN_IF_EQUALS(val1, val2);
  ASSERT_EQ(1, 1);
}

// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit tests for:
 * void set_nth_bit(int *val, char bit)
 ******************************************************************************/
class set_nth_bit_unittest : public ::testing::Test {
public:
  void test(int, int);
};

// -----------------------------------------------------------------------------

void
set_nth_bit_unittest::test(int bit, int expected_value)
{
  int val = 0;
  set_nth_bit(&val, static_cast<char>(bit));
  EXPECT_EQ(expected_value, val);
}

// -----------------------------------------------------------------------------

TEST_F(set_nth_bit_unittest, TestSet1stBit)
{
  test(1, 0x0001);
}

// -----------------------------------------------------------------------------

TEST_F(set_nth_bit_unittest, TestSet2ndBit)
{
  test(2, 0x0002);
}

// -----------------------------------------------------------------------------

TEST_F(set_nth_bit_unittest, TestSet4thBit)
{
  test(4, 0x0008);
}

// -----------------------------------------------------------------------------

TEST_F(set_nth_bit_unittest, TestSet8thBit)
{
  test(8, 128);
}

// -----------------------------------------------------------------------------

TEST_F(set_nth_bit_unittest, TestSet16thBit)
{
  test(16, 32768);
}

// -----------------------------------------------------------------------------

TEST_F(set_nth_bit_unittest, TestSet32thBit)
{
  test(32, (1 << 31));
}

// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit tests for:
 * void clear_nth_bit(int *val, char bit)
 ******************************************************************************/
class clear_nth_bit_unittest : public ::testing::Test {
public:
  void test(int, int, int=0);
};

// -----------------------------------------------------------------------------

void
clear_nth_bit_unittest::test(int value, int bit, int expected_value)
{
  clear_nth_bit(&value, static_cast<char>(bit));
  EXPECT_EQ(expected_value, value);
}

// -----------------------------------------------------------------------------

TEST_F(clear_nth_bit_unittest, TestClear1stBit)
{
  test(0x0001, 1);
}

// -----------------------------------------------------------------------------

TEST_F(clear_nth_bit_unittest, TestClear2ndBit)
{
  test(0x0002, 2);
}

// -----------------------------------------------------------------------------

TEST_F(clear_nth_bit_unittest, TestClear4thBit)
{
  test(0x0008, 4);
}

// -----------------------------------------------------------------------------

TEST_F(clear_nth_bit_unittest, TestClear8thBit)
{
  test(128, 8);
}

// -----------------------------------------------------------------------------

TEST_F(clear_nth_bit_unittest, TestClear16thBit)
{
  test(32768, 16);
}

// -----------------------------------------------------------------------------

TEST_F(clear_nth_bit_unittest, TestClear32thBit)
{
  test((1 << 31), 32);
}

// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit test for:
 * void is_bit_set(int val, char bit)
 ******************************************************************************/
class is_bit_set_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(is_bit_set_unittest, TestBitSetOnZeroValueInteger)
{
  EXPECT_FALSE(is_bit_set(0, 1));
}

// -----------------------------------------------------------------------------

TEST_F(is_bit_set_unittest, TestSetOn1stBit)
{
  EXPECT_TRUE(is_bit_set(1, 1));
}

// -----------------------------------------------------------------------------

TEST_F(is_bit_set_unittest, TestSetOn2ndBit)
{
  int val = 2;
  EXPECT_FALSE(is_bit_set(val, 1));
  EXPECT_TRUE(is_bit_set(val, 2));
}

// -----------------------------------------------------------------------------

TEST_F(is_bit_set_unittest, TestSetOn4thBit)
{
  int val = 8;
  EXPECT_FALSE(is_bit_set(val, 1));
  EXPECT_FALSE(is_bit_set(val, 2));
  EXPECT_TRUE(is_bit_set(val, 4));
}

// -----------------------------------------------------------------------------

TEST_F(is_bit_set_unittest, TestSetOn8thBit)
{
  int val = 128;
  EXPECT_FALSE(is_bit_set(val, 1));
  EXPECT_FALSE(is_bit_set(val, 2));
  EXPECT_FALSE(is_bit_set(val, 4));
  EXPECT_TRUE(is_bit_set(val, 8));
}

// -----------------------------------------------------------------------------

TEST_F(is_bit_set_unittest, TestSetOn16thBit)
{
  int val = 32768;
  EXPECT_FALSE(is_bit_set(val, 1));
  EXPECT_FALSE(is_bit_set(val, 2));
  EXPECT_FALSE(is_bit_set(val, 4));
  EXPECT_FALSE(is_bit_set(val, 8));
  EXPECT_TRUE(is_bit_set(val, 16));
}

// -----------------------------------------------------------------------------

TEST_F(is_bit_set_unittest, TestSetOn32thBit)
{
  int val = (1 << 31);
  EXPECT_FALSE(is_bit_set(val, 1));
  EXPECT_FALSE(is_bit_set(val, 2));
  EXPECT_FALSE(is_bit_set(val, 4));
  EXPECT_FALSE(is_bit_set(val, 8));
  EXPECT_FALSE(is_bit_set(val, 16));
  EXPECT_TRUE(is_bit_set(val, 32));
}

// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit tests for:
 * void set_nth_bit_uint32(uint32_t *val, char bit)
 ******************************************************************************/
class set_nth_bit_uint32_unittest : public ::testing::Test {
public:
  void test(char, uint32_t);
};

// -----------------------------------------------------------------------------

void
set_nth_bit_uint32_unittest::test(char bit, uint32_t expected_value)
{
  uint32_t val = 0;
  set_nth_bit_uint32(&val, bit);
  EXPECT_EQ(expected_value, val);
}

// -----------------------------------------------------------------------------

TEST_F(set_nth_bit_uint32_unittest, TestSet1stBit)
{
  test(1, 0x0001);
}

// -----------------------------------------------------------------------------

TEST_F(set_nth_bit_uint32_unittest, TestSet2ndBit)
{
  test(2, 0x0002);
}

// -----------------------------------------------------------------------------

TEST_F(set_nth_bit_uint32_unittest, TestSet4thBit)
{
  test(4, 0x0008);
}

// -----------------------------------------------------------------------------

TEST_F(set_nth_bit_uint32_unittest, TestSet8thBit)
{
  test(8, 128);
}

// -----------------------------------------------------------------------------

TEST_F(set_nth_bit_uint32_unittest, TestSet16thBit)
{
  test(16, 32768);
}

// -----------------------------------------------------------------------------

TEST_F(set_nth_bit_uint32_unittest, TestSet32thBit)
{
  test(32, (1u << 31));
}

// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit tests for:
 * void clear_nth_bit_uint32(uint32_t *val, char bit)
 ******************************************************************************/
class clear_nth_bit_uint32_unittest : public ::testing::Test {
public:
  void test(uint32_t, char, uint32_t=0);
};

// -----------------------------------------------------------------------------

void
clear_nth_bit_uint32_unittest::test(
  uint32_t value, char bit, uint32_t expected_value)
{
  clear_nth_bit_uint32(&value, bit);
  EXPECT_EQ(expected_value, value);
}

// -----------------------------------------------------------------------------

TEST_F(clear_nth_bit_uint32_unittest, TestClear1stBit)
{
  test(0x0001, 1);
}

// -----------------------------------------------------------------------------

TEST_F(clear_nth_bit_uint32_unittest, TestClear2ndBit)
{
  test(0x0002, 2);
}

// -----------------------------------------------------------------------------

TEST_F(clear_nth_bit_uint32_unittest, TestClear4thBit)
{
  test(0x0008, 4);
}

// -----------------------------------------------------------------------------

TEST_F(clear_nth_bit_uint32_unittest, TestClear8thBit)
{
  test(128, 8);
}

// -----------------------------------------------------------------------------

TEST_F(clear_nth_bit_uint32_unittest, TestClear16thBit)
{
  test(32768, 16);
}

// -----------------------------------------------------------------------------

TEST_F(clear_nth_bit_uint32_unittest, TestClear32thBit)
{
  test((1u << 31), 32);
}

// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit test for:
 * void is_bit_set_uint32(uint32_t val, char bit)
 ******************************************************************************/
class is_bit_set_uint32_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(is_bit_set_uint32_unittest, TestBitSetOnZeroValueInteger)
{
  EXPECT_FALSE(is_bit_set_uint32(0, 1));
}

// -----------------------------------------------------------------------------

TEST_F(is_bit_set_uint32_unittest, TestSetOn1stBit)
{
  EXPECT_TRUE(is_bit_set_uint32(1, 1));
}

// -----------------------------------------------------------------------------

TEST_F(is_bit_set_uint32_unittest, TestSetOn2ndBit)
{
  uint32_t val = 2;
  EXPECT_FALSE(is_bit_set_uint32(val, 1));
  EXPECT_TRUE(is_bit_set_uint32(val, 2));
}

// -----------------------------------------------------------------------------

TEST_F(is_bit_set_uint32_unittest, TestSetOn4thBit)
{
  uint32_t val = 8;
  EXPECT_FALSE(is_bit_set_uint32(val, 1));
  EXPECT_FALSE(is_bit_set_uint32(val, 2));
  EXPECT_TRUE(is_bit_set_uint32(val, 4));
}

// -----------------------------------------------------------------------------

TEST_F(is_bit_set_uint32_unittest, TestSetOn8thBit)
{
  uint32_t val = 128;
  EXPECT_FALSE(is_bit_set_uint32(val, 1));
  EXPECT_FALSE(is_bit_set_uint32(val, 2));
  EXPECT_FALSE(is_bit_set_uint32(val, 4));
  EXPECT_TRUE(is_bit_set_uint32(val, 8));
}

// -----------------------------------------------------------------------------

TEST_F(is_bit_set_uint32_unittest, TestSetOn16thBit)
{
  uint32_t val = 32768;
  EXPECT_FALSE(is_bit_set_uint32(val, 1));
  EXPECT_FALSE(is_bit_set_uint32(val, 2));
  EXPECT_FALSE(is_bit_set_uint32(val, 4));
  EXPECT_FALSE(is_bit_set_uint32(val, 8));
  EXPECT_TRUE(is_bit_set_uint32(val, 16));
}

// -----------------------------------------------------------------------------

TEST_F(is_bit_set_uint32_unittest, TestSetOn32thBit)
{
  uint32_t val = 1u << 31;
  EXPECT_FALSE(is_bit_set_uint32(val, 1));
  EXPECT_FALSE(is_bit_set_uint32(val, 2));
  EXPECT_FALSE(is_bit_set_uint32(val, 4));
  EXPECT_FALSE(is_bit_set_uint32(val, 8));
  EXPECT_FALSE(is_bit_set_uint32(val, 16));
  EXPECT_TRUE(is_bit_set_uint32(val, 32));
}

// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit tests for:
 * void set_nth_bit_uint8(uint8_t *val, char bit)
 ******************************************************************************/
class set_nth_bit_uint8_unittest : public ::testing::Test {
public:
  void test(char, uint8_t);
};

// -----------------------------------------------------------------------------

void
set_nth_bit_uint8_unittest::test(char bit, uint8_t expected_value)
{
  uint8_t val = 0;
  set_nth_bit_uint8(&val, bit);
  EXPECT_EQ(expected_value, val);
}

// -----------------------------------------------------------------------------

TEST_F(set_nth_bit_uint8_unittest, TestSet1stBit)
{
  test(1, 0x0001);
}

// -----------------------------------------------------------------------------

TEST_F(set_nth_bit_uint8_unittest, TestSet2ndBit)
{
  test(2, 0x0002);
}

// -----------------------------------------------------------------------------

TEST_F(set_nth_bit_uint8_unittest, TestSet4thBit)
{
  test(4, 0x0008);
}

// -----------------------------------------------------------------------------

TEST_F(set_nth_bit_uint8_unittest, TestSet8thBit)
{
  test(8, 128);
}

// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit tests for:
 * void clear_nth_bit_uint8(uint8_t *val, char bit)
 ******************************************************************************/
class clear_nth_bit_uint8_unittest : public ::testing::Test {
public:
  void test(uint8_t, char, uint8_t=0);
};

// -----------------------------------------------------------------------------

void
clear_nth_bit_uint8_unittest::test(
  uint8_t value, char bit, uint8_t expected_value)
{
  clear_nth_bit_uint8(&value, bit);
  EXPECT_EQ(expected_value, value);
}

// -----------------------------------------------------------------------------

TEST_F(clear_nth_bit_uint8_unittest, TestClear1stBit)
{
  test(0x0001, 1);
}

// -----------------------------------------------------------------------------

TEST_F(clear_nth_bit_uint8_unittest, TestClear2ndBit)
{
  test(0x0002, 2);
}

// -----------------------------------------------------------------------------

TEST_F(clear_nth_bit_uint8_unittest, TestClear4thBit)
{
  test(0x0008, 4);
}

// -----------------------------------------------------------------------------

TEST_F(clear_nth_bit_uint8_unittest, TestClear8thBit)
{
  test(128, 8);
}

// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit test for:
 * void is_bit_set_uint8(uint8_t val, char bit)
 ******************************************************************************/
class is_bit_set_uint8_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(is_bit_set_uint8_unittest, TestBitSetOnZeroValueInteger)
{
  EXPECT_FALSE(is_bit_set_uint8(0, 1));
}

// -----------------------------------------------------------------------------

TEST_F(is_bit_set_uint8_unittest, TestSetOn1stBit)
{
  EXPECT_TRUE(is_bit_set_uint8(1, 1));
}

// -----------------------------------------------------------------------------

TEST_F(is_bit_set_uint8_unittest, TestSetOn2ndBit)
{
  uint8_t val = 2;
  EXPECT_FALSE(is_bit_set_uint8(val, 1));
  EXPECT_TRUE(is_bit_set_uint8(val, 2));
}

// -----------------------------------------------------------------------------

TEST_F(is_bit_set_uint8_unittest, TestSetOn4thBit)
{
  uint8_t val = 8;
  EXPECT_FALSE(is_bit_set_uint8(val, 1));
  EXPECT_FALSE(is_bit_set_uint8(val, 2));
  EXPECT_TRUE(is_bit_set_uint8(val, 4));
}

// -----------------------------------------------------------------------------

TEST_F(is_bit_set_uint8_unittest, TestSetOn8thBit)
{
  uint8_t val = 128;
  EXPECT_FALSE(is_bit_set_uint8(val, 1));
  EXPECT_FALSE(is_bit_set_uint8(val, 2));
  EXPECT_FALSE(is_bit_set_uint8(val, 4));
  EXPECT_TRUE(is_bit_set_uint8(val, 8));
}

// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit Test for:
 * rand_top(int top)
 ******************************************************************************/
class rand_top_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(rand_top_unittest, TestRandomness)
{
  int i;
  for (i = 1; i <= 5000; i++) {
    ASSERT_LE(rand_top(i), i);
  }
}

// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit Test for:
 * rand_range(int min, int max)
 ******************************************************************************/
class rand_range_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(rand_range_unittest, TestRandomness)
{
  int i;
  int min = 20;
  for (i = min; i <= 5000; i++) {
    int _rand = rand_range(min, i);
    ASSERT_LE(min, _rand);
    ASSERT_GE(i, _rand);
  }
}

// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit Tests for:
 * randf_top(double top)
 ******************************************************************************/
class randf_top_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(randf_top_unittest, TestRandomness)
{
  int i;
  for (i = 1.0f; i <= 5000.0f; i++) {
    ASSERT_LE(rand_top(i), i);
  }
}

// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit Tests for:
 * randf_range(double min, double max)
 ******************************************************************************/
class randf_range_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(randf_range_unittest, TestRandomness)
{
  double i;
  double min = 20.0f;
  for (i = min; i <= 5000.0f; i++) {
    double _rand = randf_range(min, i);
    ASSERT_LE(min, _rand);
    ASSERT_GE(i, _rand);
  }
}

// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit Tests for:
 * generate_text(size_t len, size_t max) 
 ******************************************************************************/
class generate_text_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(generate_text_unittest, TestPassNonZeroLenAndMax)
{
  char *text = generate_text(6, 12);
  assert(text);
  ASSERT_LE(6, strlen(text));
  ASSERT_GE(12, strlen(text));
}

// -----------------------------------------------------------------------------

TEST_F(generate_text_unittest, TestPassZeroMinAndMax)
{
  char *text = generate_text(0, 0);
  assert(text == NULL);
}

// -----------------------------------------------------------------------------

TEST_F(generate_text_unittest, TestPassNonZeroLenAndZeroMax)
{
  for (size_t i = 1; i <= 5000; ++i) {
    char *text = generate_text(i, 0);
    assert(text);
    ASSERT_EQ(i, strlen(text));
    free(text);
  }
}

// -----------------------------------------------------------------------------

TEST_F(generate_text_unittest, TestPassZeroLenAndNonZeroMax)
{
  for (size_t i = 1; i <= 5000; ++i) {
    char *text = generate_text(0, i);
    assert(text);
    ASSERT_GE(i, strlen(text));
    free(text);
  }
}

// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit Tests for:
 * generate_loremipsum()
 ******************************************************************************/
class generate_loremipsum_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(generate_loremipsum_unittest, TestGenerateLoremIpsum)
{
  char *ipsum = generate_loremipsum();
  assert(ipsum);
  ASSERT_GE(strlen(ipsum), 0);
  free(ipsum);
}

// -----------------------------------------------------------------------------

#if defined(__clang__) and __clang__
  #pragma clang diagnostic pop
#endif
