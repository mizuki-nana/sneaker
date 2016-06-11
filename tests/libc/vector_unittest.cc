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

/* Unit test for `vector_t` defined in sneaker/libc/vector.h */

#include "libc/vector.h"

#include "libc/memory.h"
#include "libc/utils.h"
#include "testing/testing.h"

#include <cassert>
#include <climits>
#include <string>


// -----------------------------------------------------------------------------

class vector_unittest : public ::testing::Test {
protected:
  virtual void SetUp() {
    m_vector = vector_create();
    assert(m_vector);
  }

  virtual void TearDown() {
    vector_free(&m_vector);
    assert(m_vector == NULL);
  }

  vector_t m_vector;
};

// -----------------------------------------------------------------------------

TEST_F(vector_unittest, TestCreation)
{
  assert(m_vector);
  ASSERT_EQ(0, vector_size(m_vector));
}

// -----------------------------------------------------------------------------

TEST_F(vector_unittest, TestAppendAndGet)
{
  int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  for (size_t i = 0; i < 10; ++i) {
    vector_append(m_vector, &numbers[i]);
    ASSERT_EQ(i + 1, vector_size(m_vector));
  }

  const void** data = vector_content(m_vector);

  for (size_t i = 0; i < 10; ++i) {
    int actual_value = DEREF_VOID(int, data[i]);
    ASSERT_EQ(numbers[i], actual_value);
  }

  for (size_t i = 0; i < 10; ++i) {
    void *p = vector_get(m_vector, (int)i);
    int val = DEREF_VOID(int, p);
    ASSERT_EQ(numbers[i], val);
    ASSERT_EQ(10, vector_size(m_vector));
  }

  ASSERT_EQ(10, vector_size(m_vector));
}

// -----------------------------------------------------------------------------

TEST_F(vector_unittest, TestAppendAndRemove)
{
  int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int i;

  for (i = 0; i < 10; i++) {
    vector_append(m_vector, &numbers[i]);
    ASSERT_EQ(i + 1, vector_size(m_vector));
  }

  for (i = 0; i < 10; i++) {
    void *p = vector_remove(m_vector, 0);
    int val = DEREF_VOID(int, p);
    ASSERT_EQ(numbers[i], val);
    ASSERT_EQ(10 - i - 1, vector_size(m_vector));
  }

  ASSERT_EQ(0, vector_size(m_vector));
}

// -----------------------------------------------------------------------------

TEST_F(vector_unittest, TestRemove)
{
  int odds[] = {1, 3, 5, 7, 9};
  int i;

  for (i = 0; i < 5; i++) {
    vector_append(m_vector, &odds[i]);
    ASSERT_EQ(i + 1, vector_size(m_vector));
  }

  /* vector = {1, 3, 5, 7, 9} */
  {
    int val = DEREF_VOID(int, vector_remove(m_vector, 1));
    ASSERT_EQ(3, val);
    ASSERT_EQ(4, vector_size(m_vector));
  }

  /* vector = {1, 5, 7, 9} */
  {
    int val = DEREF_VOID(int, vector_remove(m_vector, 2));
    ASSERT_EQ(7, val);
    ASSERT_EQ(3, vector_size(m_vector));
  }

  /* vector = {1, 5, 9} */
  {
    int val = DEREF_VOID(int, vector_remove(m_vector, 2));
    ASSERT_EQ(9, val);
    ASSERT_EQ(2, vector_size(m_vector));
  }

  /* vector = {1, 5} */
  {
    int val = DEREF_VOID(int, vector_remove(m_vector, 0));
    ASSERT_EQ(1, val);
    ASSERT_EQ(1, vector_size(m_vector));
  }

  /* vector = {5} */
  {
    int val = DEREF_VOID(int, vector_remove(m_vector, 0));
    ASSERT_EQ(5, val);
    ASSERT_EQ(0, vector_size(m_vector));
  }
}

// -----------------------------------------------------------------------------

TEST_F(vector_unittest, TestRemoveAndSet)
{
  int odds[] = {1,3,5,7,9};
  int evens[] = {2,4,6,8,10};
  int i;

  for (i = 0; i < 5; i++) {
    vector_append(m_vector, &odds[i]);
    ASSERT_EQ(i + 1, vector_size(m_vector));
  }

  for (i = 0; i < 5; i++) {
    int val = DEREF_VOID(int, vector_set(m_vector, i, &evens[i]));
    ASSERT_EQ(odds[i], val);
    val = DEREF_VOID(int, vector_get(m_vector, i));
    ASSERT_EQ(evens[i], val);
    ASSERT_EQ(5, vector_size(m_vector));
  }

  ASSERT_EQ(5, vector_size(m_vector));

  for (i = 4; i >= 0; i--) {
    int val = DEREF_VOID(int, vector_remove(m_vector, i));
    ASSERT_EQ(evens[i], val);
    ASSERT_EQ(i, vector_size(m_vector));
  }

  ASSERT_EQ(0, vector_size(m_vector));
}

// -----------------------------------------------------------------------------
