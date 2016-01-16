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

/* Unit test for functions defined in sneaker/libc/cutils.h */

// This has to come before `cutils.h` because the declarations in it duplicate
// their standard library equivalents.
#include "../../include/testing/testing.h"

#include "../../include/libc/cutils.h"

#include "../../include/libc/memory.h"

#include <cassert>
#include <climits>
#include <string>


// -----------------------------------------------------------------------------

#define _INT_MIN_ -32767
#define _INT_MAX_ 32767

// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit test for:
 * char* itoa(int value, char *str, int base);
 ******************************************************************************/
class itoa_unittest : public ::testing::Test {
public:
  void test(int, int, const char*);

protected:
  void test_base_10(int);
};

// -----------------------------------------------------------------------------

void
itoa_unittest::test(int base, int num, const char* expected_str)
{
  char *str = NULL;
  str = itoa(num, str, base);
  assert(str);
  ASSERT_STREQ(expected_str, str);
  FREE(str);
}

// -----------------------------------------------------------------------------

void
itoa_unittest::test_base_10(int num)
{
  char expected_str[15];
  char *actual_str = NULL;
  snprintf(expected_str, sizeof(expected_str), "%d", num);
  actual_str = itoa(num, actual_str, 10);
  assert(actual_str);
  ASSERT_STREQ(expected_str, actual_str);
  FREE(actual_str);
}

// -----------------------------------------------------------------------------

TEST_F(itoa_unittest, TestBase10NegativeIntegerToASCII)
{
  int i;
  for (i = _INT_MIN_; i < 0; i++) {
    this->test_base_10(i);
  }
}

// -----------------------------------------------------------------------------

TEST_F(itoa_unittest, TestBase10PositiveIntegerToASCII)
{
  int i;
  for (i = 0; i < _INT_MAX_; i++) {
    this->test_base_10(i);
  }
}

// -----------------------------------------------------------------------------

TEST_F(itoa_unittest, TestBase2PositiveIntegerToASCII)
{
  test(2, 0, "0");
  test(2, 1, "1");
  test(2, 2, "10");
  test(2, 4, "100");
  test(2, 7, "111");
  test(2, 512, "1000000000");
  test(2, 1024, "10000000000");
  test(2, 1024 + 512, "11000000000");
}

// -----------------------------------------------------------------------------

TEST_F(itoa_unittest, TestBase8PositiveIntegerToASCII)
{
  test(8, 7, "7");
  test(8, 8, "10");
  test(8, 15, "17");
  test(8, 16, "20");
  test(8, 64, "100");
}

// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit test for:
 * int atoi(const char * str);
 ******************************************************************************/
class atoi_unittest : public ::testing::Test {
public:
  void test(const char*, int);

protected:
  void _test_valid_base_10(int);
};

// -----------------------------------------------------------------------------

void
atoi_unittest::test(const char* input_str, int expected_int)
{
  ASSERT_EQ(expected_int, atoi(input_str));
}

// -----------------------------------------------------------------------------

void
atoi_unittest::_test_valid_base_10(int num)
{
  int expected_int = num;
  char expected_str[10];
  snprintf(expected_str, sizeof(expected_str), "%d", num);
  int actual_int = atoi(expected_str);
  ASSERT_EQ(expected_int, actual_int);
}

// -----------------------------------------------------------------------------

TEST_F(atoi_unittest, TestNULLStringToInteger)
{
  test(NULL, 0);
}

// -----------------------------------------------------------------------------

TEST_F(atoi_unittest, TestValidBase10NegativeStringToInteger)
{
  int i;
  for (i = _INT_MIN_; i < 0; i++) {
    this->_test_valid_base_10(i);
  }
}

// -----------------------------------------------------------------------------

TEST_F(atoi_unittest, TestValidBase10PositiveStringToInteger)
{
  int i;
  for (i = 0; i <= _INT_MAX_; i++) {
    this->_test_valid_base_10(i);
  }
}

// -----------------------------------------------------------------------------

TEST_F(atoi_unittest, TestValidBase10PositiveStringWithSignToInteger)
{
  int i;
  for (i = 0; i <= _INT_MAX_; i++) {
    int expected_int = i;
    char expected_str[10];
    snprintf(expected_str + 1, sizeof(expected_str)-1, "%d", i);
    expected_str[0] = '+';
    int actual_int = atoi(expected_str);
    ASSERT_EQ(expected_int, actual_int);
  }
}

// -----------------------------------------------------------------------------

TEST_F(atoi_unittest, TestBase10PositiveStringToInteger)
{
  test("++123", 0);
  test("--123", 0);
  test("+123++", 123);
  test(" 123 ", 123);
  test("-123--", -123);
  test(" -123 ", -123);
}

// -----------------------------------------------------------------------------
