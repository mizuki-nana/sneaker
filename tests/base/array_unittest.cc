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

/* Unit test for src/base/array.c */

#include <string.h>
#include <limits.h>
#include "../_unittest.h"
#include "../../src/base/utils.h"
#include "../../src/base/memory.h"
#include "../../src/base/assert.h"
#include "../../src/base/array.h"


class ArrayTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    _array = array_create();
    ASSERT(_array);
  }

  virtual void TearDown() {
    array_free(&_array);
    ASSERT(_array == NULL);
  }

  Array _array;
};

TEST_F(ArrayTest, ArrayCreationTest) {
  ASSERT(_array);
  ASSERT_EQ(0, array_size(_array));
}

TEST_F(ArrayTest, ArrayAppendTest1) {
  int numbers[] = {1,2,3,4,5,6,7,8,9,10};
  int i;

  for(i = 0; i < 10; i++) {
    array_append(_array, &numbers[i]);
    ASSERT_EQ(i+1, array_size(_array));
  }

  for(i = 0; i < 10; i++) {
    void *p = array_get(_array, i);
    int val = DEREF_VOID(int, p);
    ASSERT_EQ(numbers[i], val);
    ASSERT_EQ(10, array_size(_array));
  }

  ASSERT_EQ(10, array_size(_array));
}

TEST_F(ArrayTest, ArrayAppendTest2) {
  int numbers[] = {1,2,3,4,5,6,7,8,9,10};
  int i;

  for(i = 0; i < 10; i++) {
    array_append(_array, &numbers[i]);
    ASSERT_EQ(i+1, array_size(_array));
  }

  for(i = 0; i < 10; i++) {
    void *p = array_remove(_array, 0);
    int val = DEREF_VOID(int, p);
    ASSERT_EQ(numbers[i], val);
    ASSERT_EQ(10-i-1, array_size(_array));
  }

  ASSERT_EQ(0, array_size(_array));
}

TEST_F(ArrayTest, ArrayRemoveTest1) {
  int odds[] = {1,3,5,7,9};
  int evens[] = {2,4,6,8,10};
  int i;

  for(i = 0; i < 5; i++) {
    array_append(_array, &odds[i]);
    ASSERT_EQ(i+1, array_size(_array));
  }

  /* array = {1,3,5,7,9} */
  {
    int val = DEREF_VOID(int, array_remove(_array, 1));
    ASSERT_EQ(3, val);
    ASSERT_EQ(4, array_size(_array));
  }

  /* array = {1,5,7,9} */
  {
    int val = DEREF_VOID(int, array_remove(_array, 2));
    ASSERT_EQ(7, val);
    ASSERT_EQ(3, array_size(_array));
  }

  /* array = {1,5,9} */
  {
    int val = DEREF_VOID(int, array_remove(_array, 2));
    ASSERT_EQ(9, val);
    ASSERT_EQ(2, array_size(_array));
  }

  /* array = {1,5} */
  {
    int val = DEREF_VOID(int, array_remove(_array, 0));
    ASSERT_EQ(1, val);
    ASSERT_EQ(1, array_size(_array));
  }

  /* array = {5} */
  {
    int val = DEREF_VOID(int, array_remove(_array, 0));
    ASSERT_EQ(5, val);
    ASSERT_EQ(0, array_size(_array));
  }
}

TEST_F(ArrayTest, ArrayRemoveAndSetTest) {
  int odds[] = {1,3,5,7,9};
  int evens[] = {2,4,6,8,10};
  int i;

  for(i = 0; i < 5; i++) {
    array_append(_array, &odds[i]);
    ASSERT_EQ(i+1, array_size(_array));
  }

  for(i = 0; i < 5; i++) {
    int val = DEREF_VOID(int, array_set(_array, i, &evens[i]));
    ASSERT_EQ(odds[i], val);
    val = DEREF_VOID(int, array_get(_array, i));
    ASSERT_EQ(evens[i], val);
    ASSERT_EQ(5, array_size(_array));
  }

  ASSERT_EQ(5, array_size(_array));

  for(i = 4; i >= 0; i--) {
    int val = DEREF_VOID(int, array_remove(_array, i));
    ASSERT_EQ(evens[i], val);
    ASSERT_EQ(i, array_size(_array));
  }

  ASSERT_EQ(0, array_size(_array));
}