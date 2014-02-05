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

/* Unit test for src/libc/strutils.c */

#include <string.h>
#include "../_unittest.h"
#include "../../include/libc/strutils.h"


/***************************************
 * Test for:
 * char* strtoupper(char*)
 ***************************************/
TEST(strtoupperTest, NullInputTest) {
  EXPECT_EQ(NULL, strtoupper(NULL));
}

TEST(strtoupperTest, ValidSingleWordTest) {
  char s[] = "test";
  ASSERT_STREQ("TEST", strtoupper(s));
}

TEST(strtoupperTest, MixedSingleWordTest) {
  char s[] = "TeSt";
  ASSERT_STREQ("TEST", strtoupper(s));
}

TEST(strtoupperTest, ValidSentenceTest) {
  char s[] = "this is a test";
  ASSERT_STREQ("THIS IS A TEST", strtoupper(s));
}

TEST(strtoupperTest, MixedSentenceTest) {
  char s[] = "tHis IS a TesT";
  ASSERT_STREQ("THIS IS A TEST", strtoupper(s));
}

TEST(strtoupperTest, MixedSentenceNumbersTest) {
  char s[] = "five = 5";
  ASSERT_STREQ("FIVE = 5", strtoupper(s));
}


/***************************************
 * Test for:
 * char* strtoupper(char*)
 ***************************************/
TEST(strtolowerTest, NULLInputTest) {
 EXPECT_EQ(NULL, strtolower(NULL));
}

TEST(strtolowerTest, ValidSingleWordTest) {
  char s[] = "TEST"; 
  ASSERT_STREQ("test", strtolower(s));
}

TEST(strtolowerTest, MixedSingleWordTest) {
  char s[] = "TeSt";
  ASSERT_STREQ("test", strtolower(s));
}

TEST(strtolowerTest, ValidSentenceTest) {
  char s[] = "THIS IS A TEST";
  ASSERT_STREQ("this is a test", strtolower(s));
}

TEST(strtolowerTest, MixedSentenceTest) {
  char s[] = "tHis IS a TesT";
  ASSERT_STREQ("this is a test", strtolower(s));
}

TEST(strtolowerTest, MixedSentenceNumbersTest) {
  char s[] = "FIVE = 5";
  ASSERT_STREQ("five = 5", strtolower(s));
}


/***************************************
 * Test for:
 * char* strcpy_hard(char* dst, char *src)
 ***************************************/
TEST(strcpy_hardTest, NullInputSrcTest) {
  ASSERT_EQ(NULL, strcpy_hard(NULL, NULL));
}

TEST(strcpy_hardTest, NullDstAndValidSrcTest) {
  char src[] = "this is a test";
  char *dst = NULL;

  dst = strcpy_hard(dst, src);
  ASSERT_STREQ(dst, src);
  ASSERT_EQ(strlen(dst), strlen(src));
}

TEST(strcpy_hardTest, ValidDstAndValidSrcTest) {
  char src[] = "this is another test";
  char *dst = (char*)malloc(20);
  strcpy(dst, "some string");

  dst = strcpy_hard(dst, src);
  ASSERT_STREQ(dst, src);
  ASSERT_EQ(strlen(dst), strlen(src));
}


/***************************************
 * Test for:
 * char* strtrim(char *s)
 ***************************************/
TEST(strtrimTest, NullInputTest) {
  char *s = NULL;
  ASSERT_EQ(NULL, strtrim(s));
}

TEST(strtrimTest, InputWithNoOuterWhitespacesTest) {
  char s[] = "this is a test";
  char *d = strtrim(s);
  ASSERT_STREQ(s, d);
  ASSERT_EQ(strlen(s), strlen(d));
}

TEST(strtrimTest, InputWithLeadingWhitespacesTest) {
  char s[] = "  this is a test";
  char *d = strtrim(s);
  ASSERT_STREQ("this is a test", d);
  ASSERT_EQ(strlen("this is a test"), strlen(d));
}

TEST(strtrimTest, InputWithTrailingWhitespacesTest) {
  char s[] = "this is a test   ";
  char *d = strtrim(s);
  ASSERT_STREQ("this is a test", d);
  ASSERT_EQ(strlen("this is a test"), strlen(d));
}

TEST(strtrimTest, InputWithBothLeadingAndTrailingWhitespacesTest) {
  char s[] = "   this is a test   ";
  char *d = strtrim(s);
  ASSERT_STREQ("this is a test", d);
  ASSERT_EQ(strlen("this is a test"), strlen(d));
}


/********************************************************
 * Test for:
 * char* strncpy_safe(char *dst, char *src, size_t size)
 ********************************************************/
TEST(strncpy_safeTest, NullDstInputTest) {
  char *dst = NULL;
  char *src = (char*)malloc(10);
  ASSERT_EQ(NULL, strncpy_safe(dst, src, 10));
  free(src);
}

TEST(strncpy_safeTest, NullSrcInputTest) {
  char *dst = (char*)malloc(10);
  char *src = NULL;
  ASSERT_EQ(NULL, strncpy_safe(dst, src, 10));
  free(dst);
}

TEST(strncpy_safeTest, NullDstAndSrcInputTest) {
  char *dst = NULL;
  const char *src = NULL;
  ASSERT_EQ(NULL, strncpy_safe(dst, src, 10));
}

TEST(strncpy_safeTest, EqualSizeDstAndSrcTest) {
  char *dst = (char*)malloc(10);
  const char src[] = "123456789";
  dst = strncpy_safe(dst, src, strlen(src));
  ASSERT_STREQ(dst, src);
  ASSERT_EQ(strlen(dst), strlen(src));
}

TEST(strncpy_safeTest, SmallerSizeDstThanSrcTest) {
  char *dst = (char*)malloc(5);
  const char src[] = "123456789";
  dst = strncpy_safe(dst, src, 4);
  ASSERT_STREQ("1234", dst);
  ASSERT_EQ(strlen("1234"), strlen(dst));
}

TEST(strncpy_safeTest, LargerSizeDstThanSrcTest) {
  char *dst = (char*)malloc(20);
  const char src[] = "123456789";
  dst = strncpy_safe(dst, src, 19);
  ASSERT_STREQ(src, dst);
  ASSERT_EQ(strlen(src), strlen(dst));
}