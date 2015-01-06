/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2015 Yanzheng Li

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

/* Unit test for functions defined in sneaker/libc/strutils.h */

#include "../../include/libc/strutils.h"

#include "../../include/testing/testing.h"

#include <cstring>


/*******************************************************************************
 * Unit test for:
 * char* strtoupper(char*)
 ******************************************************************************/
class strtoupper_unittest : public ::testing::Test {};


TEST_F(strtoupper_unittest, TestNullInput)
{
  ASSERT_EQ(NULL, strtoupper(NULL));
}

TEST_F(strtoupper_unittest, TestValidSingleWord)
{
  char s[] = "test";
  ASSERT_STREQ("TEST", strtoupper(s));
}

TEST_F(strtoupper_unittest, TestMixedSingleWord)
{
  char s[] = "TeSt";
  ASSERT_STREQ("TEST", strtoupper(s));
}

TEST_F(strtoupper_unittest, TestValidSentence)
{
  char s[] = "this is a test";
  ASSERT_STREQ("THIS IS A TEST", strtoupper(s));
}

TEST_F(strtoupper_unittest, TestSentenceWithMixedCases)
{
  char s[] = "tHis IS a TesT";
  ASSERT_STREQ("THIS IS A TEST", strtoupper(s));
}

TEST_F(strtoupper_unittest, TestSentenceWithNumbers)
{
  char s[] = "five = 5";
  ASSERT_STREQ("FIVE = 5", strtoupper(s));
}

TEST_F(strtoupper_unittest, TestSentenceWithSpecialCharacters)
{
  char s[] = "This quarter's revenue is 100% off the chart, more $$ coming in!!!";
  ASSERT_STREQ(
    "THIS QUARTER'S REVENUE IS 100% OFF THE CHART, MORE $$ COMING IN!!!",
    strtoupper(s)
  );
}


/*******************************************************************************
 * Unit test for:
 * char* strtolower(char*)
 ******************************************************************************/
class strtolower_unittest : public ::testing::Test {};


TEST_F(strtolower_unittest, TestNULLInput)
{
 ASSERT_EQ(NULL, strtolower(NULL));
}

TEST_F(strtolower_unittest, TestValidSingleWord)
{
  char s[] = "TEST"; 
  ASSERT_STREQ("test", strtolower(s));
}

TEST_F(strtolower_unittest, TestSingleWordWithMixedCases)
{
  char s[] = "TeSt";
  ASSERT_STREQ("test", strtolower(s));
}

TEST_F(strtolower_unittest, TestValidSentence)
{
  char s[] = "THIS IS A TEST";
  ASSERT_STREQ("this is a test", strtolower(s));
}

TEST_F(strtolower_unittest, TestSentenceWithMixedCases)
{
  char s[] = "tHis IS a TesT";
  ASSERT_STREQ("this is a test", strtolower(s));
}

TEST_F(strtolower_unittest, TestMixedSentenceNumbers)
{
  char s[] = "FIVE = 5";
  ASSERT_STREQ("five = 5", strtolower(s));
}

TEST_F(strtolower_unittest, TestSentenceWithSpecialCharacters)
{
  char s[] = "THIS QUARTER'S REVENUE IS 100% OFF THE CHART, MORE $$ COMING IN!!!";
  ASSERT_STREQ(
    "this quarter's revenue is 100% off the chart, more $$ coming in!!!",
    strtolower(s)
  );
}


/*******************************************************************************
 * Unit test for:
 * char* strcpy_hard(char* dst, char *src)
 ******************************************************************************/
class strcpy_hard_unittest : public ::testing::Test {};


TEST_F(strcpy_hard_unittest, TestNullInputSrc)
{
  ASSERT_EQ(NULL, strcpy_hard(NULL, NULL));
}

TEST_F(strcpy_hard_unittest, TestNullDstAndValidSrc)
{
  char src[] = "this is a test";
  char *dst = NULL;

  dst = strcpy_hard(dst, src);

  ASSERT_STREQ(dst, src);
  ASSERT_EQ(strlen(dst), strlen(src));

  free(dst);
}

TEST_F(strcpy_hard_unittest, TestValidDstAndValidSrc)
{
  char src[] = "this is another test";
  char *dst = (char*)malloc(20);
  strcpy(dst, "some string");

  dst = strcpy_hard(dst, src);

  ASSERT_STREQ(dst, src);
  ASSERT_EQ(strlen(dst), strlen(src));

  free(dst);
}


/*******************************************************************************
 * Unit test for:
 * char* strtrim(char *s)
 ******************************************************************************/
class strtrim_unittest : public ::testing::Test {};


TEST_F(strtrim_unittest, TestNullInputTest)
{
  char *s = NULL;
  ASSERT_EQ(NULL, strtrim(s));
}

TEST_F(strtrim_unittest, TestInputWithNoOuterWhitespaces)
{
  char s[] = "this is a test";
  char *d = strtrim(s);

  ASSERT_STREQ(s, d);
  ASSERT_EQ(strlen(s), strlen(d));
}

TEST_F(strtrim_unittest, TestInputWithLeadingWhitespaces)
{
  char s[] = "  this is a test";
  char *d = strtrim(s);

  ASSERT_STREQ("this is a test", d);
  ASSERT_EQ(strlen("this is a test"), strlen(d));
}

TEST_F(strtrim_unittest, TestInputWithTrailingWhitespaces)
{
  char s[] = "this is a test   ";
  char *d = strtrim(s);

  ASSERT_STREQ("this is a test", d);
  ASSERT_EQ(strlen("this is a test"), strlen(d));
}

TEST_F(strtrim_unittest, TestInputWithBothLeadingAndTrailingWhitespacesTest)
{
  char s[] = "   this is a test   ";
  char *d = strtrim(s);

  ASSERT_STREQ("this is a test", d);
  ASSERT_EQ(strlen("this is a test"), strlen(d));
}


/*******************************************************************************
 * Unit test for:
 * char* strncpy_safe(char *dst, char *src, size_t size)
 ******************************************************************************/
class strncpy_safe_unittest : public ::testing::Test {};


TEST_F(strncpy_safe_unittest, TestNullDstInputTest)
{
  char *dst = NULL;
  char *src = (char*)malloc(10);

  ASSERT_EQ(NULL, strncpy_safe(dst, src, 10));
  free(src);
}

TEST_F(strncpy_safe_unittest, TestNullSrcInput)
{
  char *dst = (char*)malloc(10);
  char *src = NULL;

  ASSERT_EQ(NULL, strncpy_safe(dst, src, 10));
  free(dst);
}

TEST_F(strncpy_safe_unittest, TestNullDstAndSrcInput)
{
  char *dst = NULL;
  const char *src = NULL;

  ASSERT_EQ(NULL, strncpy_safe(dst, src, 10));
}

TEST_F(strncpy_safe_unittest, TestEqualSizeDstAndSrcTest)
{
  char *dst = (char*)malloc(10);
  memset(dst, 0, 10);
  const char src[] = "123456789";
  dst = strncpy_safe(dst, src, strlen(src));

  ASSERT_STREQ(dst, src);
  ASSERT_EQ(strlen(dst), strlen(src));
}

TEST_F(strncpy_safe_unittest, TestSmallerSizeDstThanSrc)
{
  char *dst = (char*)malloc(5);
  memset(dst, 0, 5);
  const char src[] = "123456789";
  dst = strncpy_safe(dst, src, 4);

  ASSERT_STREQ("1234", dst);
  ASSERT_EQ(strlen("1234"), strlen(dst));
}

TEST_F(strncpy_safe_unittest, TestLargerSizeDstThanSrcTest)
{
  char *dst = (char*)malloc(20);
  memset(dst, 0, 20);
  const char src[] = "123456789";
  dst = strncpy_safe(dst, src, 19);

  ASSERT_STREQ(src, dst);
  ASSERT_EQ(strlen(src), strlen(dst));
}


/*******************************************************************************
 * Unit test for:
 * size_t strlcpy2(char *dst, const char *src, size_t size)
 ******************************************************************************/
class strlcpy2_unittest : public ::testing::Test {};


TEST_F(strlcpy2_unittest, TestNullDstAndSrc)
{
  size_t copied = strlcpy2(NULL, NULL, 10);
  ASSERT_EQ(0, copied);
}

TEST_F(strlcpy2_unittest, TestSrcLargerThanDst)
{
  char dst[5];
  char src[] = "Hello World!";

  size_t copied = strlcpy2(dst, src, 5);
  size_t expectec_copied = strlen(src);

  ASSERT_EQ(expectec_copied, copied);
  ASSERT_STREQ("Hell", dst);
}

TEST_F(strlcpy2_unittest, TestSrcLessThanDst)
{
  char dst[20];
  char src[] = "Hello World!";

  size_t copied = strlcpy2(dst, src, 20);
  size_t expectec_copied = strlen(src);

  ASSERT_EQ(expectec_copied, copied);
  ASSERT_STREQ(src, dst);
}
