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

/* Unit test for `strbuf_t` defined in sneaker/libc/strbuf.h */

#include <cassert>
#include <climits>
#include <cstring>
#include "../../include/testing/testing.h"
#include "../../include/libc/c_str.h"
#include "../../include/libc/memory.h"
#include "../../include/libc/strbuf.h"


class strbuf_unittest : public ::testing::Test {
protected:
  virtual void SetUp() {
    _strbuf = strbuf_create();
    assert(_strbuf);
  }

  virtual void TearDown() {
    strbuf_free(&_strbuf);
    assert(_strbuf == NULL);
  }

  void test_empty();
  void test_append(const c_str);
  void test_append(const c_str, const c_str, const c_str=NULL);

  strbuf_t _strbuf;
};

void
strbuf_unittest::test_empty()
{
  ASSERT_STREQ("", strbuf_cstr(_strbuf));
  ASSERT_EQ(0, strbuf_len(_strbuf));
  ASSERT_LE(0, strbuf_capacity(_strbuf));
}

void
strbuf_unittest::test_append(const c_str text)
{
  ASSERT_STREQ("", strbuf_cstr(_strbuf));

  strbuf_append(_strbuf, text);

  ASSERT_EQ(strlen(text), strbuf_len(_strbuf));
  ASSERT_STREQ(text, strbuf_cstr(_strbuf));
  ASSERT_LE(0, strbuf_capacity(_strbuf));
}

void
strbuf_unittest::test_append(
  const c_str text1, const c_str text2, const c_str expected_str)
{
  ASSERT_STREQ("", strbuf_cstr(_strbuf));

  strbuf_append(_strbuf, text1);
  strbuf_append(_strbuf, text2);

  ASSERT_EQ(strlen(text1) + strlen(text2), strbuf_len(_strbuf));
  ASSERT_LE(0, strbuf_capacity(_strbuf));

  if(expected_str) {
    ASSERT_STREQ(expected_str, strbuf_cstr(_strbuf));
  }
}


TEST_F(strbuf_unittest, TestCreation)
{
  assert(_strbuf);
  assert(strbuf_cstr(_strbuf));
  ASSERT_STREQ("", strbuf_cstr(_strbuf));
}

TEST_F(strbuf_unittest, TestAppend1)
{
  char text[] = "Testing strbuf";
  this->test_append(text);
}

TEST_F(strbuf_unittest, TestStrbufAppend2)
{
  char text1[] = "5 km a day, ";
  char text2[] = "keep the doctor away!";

  this->test_append(
    text1,
    text2,
    (const c_str)"5 km a day, keep the doctor away!"
  );
}

TEST_F(strbuf_unittest, TestStrbufAppend3)
{
  char text1[] = "Apple, banana, coconut";
  char text2[] = "Airplane, boat, car";

  this->test_append(text1);

  strbuf_empty(_strbuf);

  this->test_empty();

  this->test_append(text2);
}

TEST_F(strbuf_unittest, TestStrbufAppend4)
{
  const int length = 1020;
  char text1[length];
  int i;

  for(i = 0; i < length-1; i++) {
    text1[i] = (char)(i % 26) + '0';
  }
  text1[length-1] = '\0';

  this->test_append(text1);

  char text2[] = "Testing for appending long string to make strbuf capacity grow.....";

  size_t size = strlen(text1) + strlen(text2);

  c_str text = (c_str)malloc(size+1);
  assert(text);
  memset(text, 0, size + 1);
  text = strcat(text, text1);
  text = strcat(text, text2);

  strbuf_append(_strbuf, text2);

  ASSERT_STREQ(text, strbuf_cstr(_strbuf));
  ASSERT_EQ(size, strbuf_len(_strbuf));
}

TEST_F(strbuf_unittest, TestStrbufAppend5)
{
  char text1[3000];
  int i;

  for(i = 0; i < 2999; i++) {
    text1[i] = (char)(i % 26) + '0';
  }
  text1[2999] = '\0';

  this->test_append(text1);

  char text2[] = "Testing for appending long string for strbuf.";

  size_t size = strlen(text1) + strlen(text2);

  c_str text = (c_str)malloc(size + 1);
  assert(text);
  memset(text, 0 , size + 1);
  text = strcat(text, text1);
  text = strcat(text, text2);

  strbuf_append(_strbuf, text2);

  ASSERT_STREQ(text, strbuf_cstr(_strbuf));
  ASSERT_EQ(size, strbuf_len(_strbuf));
}

TEST_F(strbuf_unittest, TestEmpty)
{
  char text[] = "Another test for strbuf";
  strbuf_append(_strbuf, text);

  ASSERT_STREQ(text, strbuf_cstr(_strbuf));
  ASSERT_EQ(strlen(text), strbuf_len(_strbuf));

  /* Empty strbuf */
  strbuf_empty(_strbuf);

  this->test_empty();

  /* Append text after empty() */
  char text2[] = "An apple a day, keep the doctor away.";

  strbuf_append(_strbuf, text2);

  ASSERT_STREQ(text2, strbuf_cstr(_strbuf));
  ASSERT_EQ(strlen(text2), strbuf_len(_strbuf));
  ASSERT_LE(0, strbuf_capacity(_strbuf));
}