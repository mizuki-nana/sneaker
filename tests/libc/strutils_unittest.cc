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

/* Unit test for functions defined in include/libc/strutils.h */

#include <string.h>
#include "../_unittest.h"
#include "../../include/libc/strutils.h"


/***************************************
 * Unit test for:
 * char* strtoupper(char*)
 ***************************************/
class StrToUpperUnitTest : public ::testing::Test {};


TEST_F(StrToUpperUnitTest, TestNullInput)
{
  ASSERT_EQ(NULL, strtoupper(NULL));
}

TEST_F(StrToUpperUnitTest, TestValidSingleWord)
{
  char s[] = "test";
  ASSERT_STREQ("TEST", strtoupper(s));
}

TEST_F(StrToUpperUnitTest, TestMixedSingleWord)
{
  char s[] = "TeSt";
  ASSERT_STREQ("TEST", strtoupper(s));
}

TEST_F(StrToUpperUnitTest, TestValidSentence)
{
  char s[] = "this is a test";
  ASSERT_STREQ("THIS IS A TEST", strtoupper(s));
}

TEST_F(StrToUpperUnitTest, TestSentenceWithMixedCases)
{
  char s[] = "tHis IS a TesT";
  ASSERT_STREQ("THIS IS A TEST", strtoupper(s));
}

TEST_F(StrToUpperUnitTest, TestSentenceWithNumbers)
{
  char s[] = "five = 5";
  ASSERT_STREQ("FIVE = 5", strtoupper(s));
}

TEST_F(StrToUpperUnitTest, TestSentenceWithSpecialCharacters)
{
  char s[] = "This quarter's revenue is 100% off the chart, more $$ coming in!!!";
  ASSERT_STREQ(
    "THIS QUARTER'S REVENUE IS 100% OFF THE CHART, MORE $$ COMING IN!!!",
    strtoupper(s)
  );
}


/***************************************
 * Unit test for:
 * char* strtolower(char*)
 ***************************************/
class StrToLowerUnitTest : public ::testing::Test {};


TEST_F(StrToLowerUnitTest, TestNULLInput)
{
 ASSERT_EQ(NULL, strtolower(NULL));
}

TEST_F(StrToLowerUnitTest, TestValidSingleWord)
{
  char s[] = "TEST"; 
  ASSERT_STREQ("test", strtolower(s));
}

TEST_F(StrToLowerUnitTest, TestSingleWordWithMixedCases)
{
  char s[] = "TeSt";
  ASSERT_STREQ("test", strtolower(s));
}

TEST_F(StrToLowerUnitTest, TestValidSentence)
{
  char s[] = "THIS IS A TEST";
  ASSERT_STREQ("this is a test", strtolower(s));
}

TEST_F(StrToLowerUnitTest, TestSentenceWithMixedCases)
{
  char s[] = "tHis IS a TesT";
  ASSERT_STREQ("this is a test", strtolower(s));
}

TEST_F(StrToLowerUnitTest, TestMixedSentenceNumbers)
{
  char s[] = "FIVE = 5";
  ASSERT_STREQ("five = 5", strtolower(s));
}

TEST_F(StrToLowerUnitTest, TestSentenceWithSpecialCharacters)
{
  char s[] = "THIS QUARTER'S REVENUE IS 100% OFF THE CHART, MORE $$ COMING IN!!!";
  ASSERT_STREQ(
    "this quarter's revenue is 100% off the chart, more $$ coming in!!!",
    strtolower(s)
  );
}


/***************************************
 * Unit test for:
 * char* strcpy_hard(char* dst, char *src)
 ***************************************/
class StrcpyHardUnitTest : public ::testing::Test {};


TEST_F(StrcpyHardUnitTest, TestNullInputSrc)
{
  ASSERT_EQ(NULL, strcpy_hard(NULL, NULL));
}

TEST_F(StrcpyHardUnitTest, TestNullDstAndValidSrc)
{
  char src[] = "this is a test";
  char *dst = NULL;

  dst = strcpy_hard(dst, src);

  ASSERT_STREQ(dst, src);
  ASSERT_EQ(strlen(dst), strlen(src));

  free(dst);
}

TEST_F(StrcpyHardUnitTest, TestValidDstAndValidSrc)
{
  char src[] = "this is another test";
  char *dst = (char*)malloc(20);
  strcpy(dst, "some string");

  dst = strcpy_hard(dst, src);

  ASSERT_STREQ(dst, src);
  ASSERT_EQ(strlen(dst), strlen(src));

  free(dst);
}


/***************************************
 * Unit test for:
 * char* strtrim(char *s)
 ***************************************/
class StrTrimUnitTest : public ::testing::Test {};


TEST_F(StrTrimUnitTest, TestNullInputTest)
{
  char *s = NULL;
  ASSERT_EQ(NULL, strtrim(s));
}

TEST_F(StrTrimUnitTest, TestInputWithNoOuterWhitespaces)
{
  char s[] = "this is a test";
  char *d = strtrim(s);

  ASSERT_STREQ(s, d);
  ASSERT_EQ(strlen(s), strlen(d));
}

TEST_F(StrTrimUnitTest, TestInputWithLeadingWhitespaces)
{
  char s[] = "  this is a test";
  char *d = strtrim(s);

  ASSERT_STREQ("this is a test", d);
  ASSERT_EQ(strlen("this is a test"), strlen(d));
}

TEST_F(StrTrimUnitTest, TestInputWithTrailingWhitespaces)
{
  char s[] = "this is a test   ";
  char *d = strtrim(s);

  ASSERT_STREQ("this is a test", d);
  ASSERT_EQ(strlen("this is a test"), strlen(d));
}

TEST_F(StrTrimUnitTest, TestInputWithBothLeadingAndTrailingWhitespacesTest)
{
  char s[] = "   this is a test   ";
  char *d = strtrim(s);

  ASSERT_STREQ("this is a test", d);
  ASSERT_EQ(strlen("this is a test"), strlen(d));
}


/********************************************************
 * Unit test for:
 * char* strncpy_safe(char *dst, char *src, size_t size)
 ********************************************************/
class StrncpySafeUnitTest : public ::testing::Test {};


TEST_F(StrncpySafeUnitTest, TestNullDstInputTest)
{
  char *dst = NULL;
  char *src = (char*)malloc(10);

  ASSERT_EQ(NULL, strncpy_safe(dst, src, 10));
  free(src);
}

TEST_F(StrncpySafeUnitTest, TestNullSrcInput)
{
  char *dst = (char*)malloc(10);
  char *src = NULL;

  ASSERT_EQ(NULL, strncpy_safe(dst, src, 10));
  free(dst);
}

TEST_F(StrncpySafeUnitTest, TestNullDstAndSrcInput)
{
  char *dst = NULL;
  const char *src = NULL;

  ASSERT_EQ(NULL, strncpy_safe(dst, src, 10));
}

TEST_F(StrncpySafeUnitTest, TestEqualSizeDstAndSrcTest)
{
  char *dst = (char*)malloc(10);
  const char src[] = "123456789";
  dst = strncpy_safe(dst, src, strlen(src));

  ASSERT_STREQ(dst, src);
  ASSERT_EQ(strlen(dst), strlen(src));
}

TEST_F(StrncpySafeUnitTest, TestSmallerSizeDstThanSrc)
{
  char *dst = (char*)malloc(5);
  const char src[] = "123456789";
  dst = strncpy_safe(dst, src, 4);

  ASSERT_STREQ("1234", dst);
  ASSERT_EQ(strlen("1234"), strlen(dst));
}

TEST_F(StrncpySafeUnitTest, TestLargerSizeDstThanSrcTest)
{
  char *dst = (char*)malloc(20);
  const char src[] = "123456789";
  dst = strncpy_safe(dst, src, 19);

  ASSERT_STREQ(src, dst);
  ASSERT_EQ(strlen(src), strlen(dst));
}