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


/* Unit test for functions defined in include/libc/cutils.h */


#include <string.h>
#include <limits.h>
#include "../../include/testing/testing.h"
#include "../../include/libc/assert.h"
#include "../../include/libc/cutils.h"
#include "../../include/libc/memory.h"

#define _INT_MIN_ -32767
#define _INT_MAX_ 32767


/**********************************************
 * Unit test for:
 * char* itoa(int value, char *str, int base);
 **********************************************/
class ItoaUnitTest : public ::testing::Test {
public:
  void test(int, int, char*);
};


void
ItoaUnitTest::test(int base, int n, char* expected_str)
{
  char *str = NULL;
  str = itoa(n, str, base);
  ASSERT_STREQ(expected_str, str);
  FREE(str);
}


TEST_F(ItoaUnitTest, TestBase10NegativeIntegerToASCII)
{
  int i;
  for(i = _INT_MIN_; i < 0; i++) {
    char expected_str[15];
    char *actual_str = NULL;
    snprintf(expected_str, sizeof(expected_str), "%d", i);
    actual_str = itoa(i, actual_str, 10);
    ASSERT_STREQ(expected_str, actual_str);
    FREE(actual_str);
  }
}

TEST_F(ItoaUnitTest, TestBase10PositiveIntegerToASCII)
{
  int i;
  for(i = 0; i < _INT_MAX_; i++) {
    char expected_str[15];
    char *actual_str = NULL;
    snprintf(expected_str, sizeof(expected_str), "%d", i);
    actual_str = itoa(i, actual_str, 10);
    ASSERT_STREQ(expected_str, actual_str);
    FREE(actual_str);
  }
}

TEST_F(ItoaUnitTest, TestBase2PositiveIntegerToASCII_1)
{
  test(2, 0, "0");
}

TEST_F(ItoaUnitTest, TestBase2PositiveIntegerToASCII_2)
{
  test(2, 1, "1");
}

TEST_F(ItoaUnitTest, TestBase2PositiveIntegerToASCII_3)
{
  test(2, 2, "10");
}

TEST_F(ItoaUnitTest, TestBase2PositiveIntegerToASCII_4)
{
  test(2, 4, "100");
}

TEST_F(ItoaUnitTest, TestBase2PositiveIntegerToASCII_5)
{
  test(2, 7, "111");
}

TEST_F(ItoaUnitTest, TestBase2PositiveIntegerToASCII_6)
{
  test(2, 512, "1000000000");
}

TEST_F(ItoaUnitTest, TestBase2PositiveIntegerToASCII_7)
{
  test(2, 1024, "10000000000");
}

TEST_F(ItoaUnitTest, TestBase2PositiveIntegerToASCII_8)
{
  test(2, 1024 + 512, "11000000000");
}

TEST_F(ItoaUnitTest, TestBase8PositiveIntegerToASCII_1)
{
  test(8, 7, "7");
}

TEST_F(ItoaUnitTest, TestBase8PositiveIntegerToASCII_2)
{
  test(8, 8, "10");
}

TEST_F(ItoaUnitTest, TestBase8PositiveIntegerToASCII_3)
{
  test(8, 15, "17");
}

TEST_F(ItoaUnitTest, TestBase8PositiveIntegerToASCII_4)
{
  test(8, 16, "20");
}

TEST_F(ItoaUnitTest, TestBase8PositiveIntegerToASCII_5)
{
  test(8, 64, "100");
}


/***************************************
 * Unit test for:
 * int atoi(const char * str);
 ***************************************/
class AtoiUnitTest : public ::testing::Test {
public:
  void test(char*, int);
};


void
AtoiUnitTest::test(char* input_str, int expected_int)
{
  ASSERT_EQ(expected_int, atoi(input_str));
}


TEST_F(AtoiUnitTest, TestNULLStringToInteger)
{
  test(NULL, 0);
}

TEST_F(AtoiUnitTest, TestValidBase10NegativeIntegerToASCII)
{
  int i;
  for(i = _INT_MIN_; i < 0; i++) {
    int expected_int = i;
    char expected_str[10];
    snprintf(expected_str, sizeof(expected_str), "%d", i);
    int actual_int = atoi(expected_str);
    ASSERT_EQ(expected_int, actual_int);
  }
}

TEST_F(AtoiUnitTest, TestValidBase10PositiveIntegerToASCII)
{
  int i;
  for(i = 0; i <= _INT_MAX_; i++) {
    int expected_int = i;
    char expected_str[10];
    snprintf(expected_str, sizeof(expected_str), "%d", i);
    int actual_int = atoi(expected_str);
    ASSERT_EQ(expected_int, actual_int);
  }
}

TEST_F(AtoiUnitTest, TestValidBase10PositiveIntegerWithSignToASCII)
{
  int i;
  for(i = 0; i <= _INT_MAX_; i++) {
    int expected_int = i;
    char expected_str[10];
    snprintf(expected_str + 1, sizeof(expected_str)-1, "%d", i);
    expected_str[0] = '+';
    int actual_int = atoi(expected_str);
    ASSERT_EQ(expected_int, actual_int);
  }
}

TEST_F(AtoiUnitTest, TestInvalidBase10PositiveIntegerToASCII_1)
{
  test("++123", 0);
}

TEST_F(AtoiUnitTest, TestInvalidBase10NegativeIntegerToASCII_2)
{
  test("--123", 0);
}

TEST_F(AtoiUnitTest, TestValidBase10PositiveIntegerToASCII_1)
{
  test("+123++", 123);
}

TEST_F(AtoiUnitTest, TestValidBase10PositiveIntegerToASCII_2)
{
  test(" 123 ", 123);
}

TEST_F(AtoiUnitTest, TestValidBase10NegativeIntegerToASCII_1)
{
  test("-123--", -123);
}

TEST_F(AtoiUnitTest, TestValidBase10NegativeIntegerToASCII_2)
{
  test(" -123 ", -123);
}