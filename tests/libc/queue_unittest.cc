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

/* Unit test for `queue_t` defined in sneaker/libc/queue.h */

#include "libc/queue.h"

#include "libc/memory.h"
#include "libc/utils.h"
#include "testing/testing.h"

#include <cassert>
#include <climits>
#include <cstring>


// -----------------------------------------------------------------------------

class queue_unittest : public ::testing::Test {
protected:
  queue_unittest()
    :
    ::testing::Test(),
    m_queue(NULL)
  {
  }

  virtual void SetUp() {
    m_queue = queue_create();
    assert(m_queue);
    assert(0 == queue_size(m_queue));
  }

  virtual void TearDown() {
    assert(0 == queue_size(m_queue));
    queue_free(&m_queue);
    assert(m_queue == NULL);
  }

  queue_t m_queue;
};

// -----------------------------------------------------------------------------

TEST_F(queue_unittest, TestCreation)
{
  assert(m_queue);
  ASSERT_EQ(0, queue_size(m_queue));
  assert(queue_pop(m_queue) == NULL);
}

// -----------------------------------------------------------------------------

TEST_F(queue_unittest, TestPushAndPop1)
{
  int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int i;
  for (i = 0; i < 10; i++) {
    queue_push(m_queue, &numbers[i], sizeof(int));
  }

  ASSERT_EQ(10, queue_size(m_queue));

  for (i = 0; i < 10; i++) {
    int *p = (int*)queue_pop(m_queue);
    int val = DEREF_VOID(int, p);
    ASSERT_EQ(numbers[i], val);
    ASSERT_EQ(10 - i - 1, queue_size(m_queue));
  }

  assert(queue_pop(m_queue) == NULL);

  ASSERT_EQ(0, queue_size(m_queue));
}

// -----------------------------------------------------------------------------

TEST_F(queue_unittest, TestPushAndPop2)
{
  int odds[] = {1, 3, 5, 7, 9};
  int evens[] = {2, 4, 6, 8, 10};

  int i;
  for (i = 0; i < 5; i++) {
    queue_push(m_queue, &odds[i], sizeof(int));
  }

  ASSERT_EQ(5, queue_size(m_queue));

  for (i = 0; i < 5; i++) {
    int *p = (int*)queue_pop(m_queue);
    int val = DEREF_VOID(int, p);
    ASSERT_EQ(odds[i], val);
    ASSERT_EQ(5 - i - 1, queue_size(m_queue));
  }

  assert(queue_pop(m_queue) == NULL);
  ASSERT_EQ(0, queue_size(m_queue));

  for (i = 0; i < 5; i++) {
    queue_push(m_queue, &evens[i], sizeof(int));
  }

  ASSERT_EQ(5, queue_size(m_queue));

  for (i = 0; i < 5; i++) {
    int *p = (int*)queue_pop(m_queue);
    int val = DEREF_VOID(int, p);
    ASSERT_EQ(evens[i], val);
    ASSERT_EQ(5 - i - 1, queue_size(m_queue));
  }

  assert(queue_pop(m_queue) == NULL);
  ASSERT_EQ(0, queue_size(m_queue));
}

// -----------------------------------------------------------------------------

TEST_F(queue_unittest, TestPushAndPop3)
{
  const char *fruits[26] = {
    "Apple", "Banana", "Coconut", "Dragonfruit", "Elephant apple",
    "Finger lime", "Grape", "Honeydrew melon", "Indian prune",
    "Jasmine", "Kiwi", "Lemon", "Melon", "Nannyberry", "Orange", "Peach",
    "Quandong", "Raspberry", "Strawberry", "Tangerine", "Uva",
    "Vanilla", "Watermelon", "Xylocarp", "Yamamomo", "Zucchini"
  };

  int i;
  for (i = 0; i < 26; i++) {
    char *fruit = (char*)fruits[i];
    queue_push(m_queue, fruit, strlen(fruit)+1);
    ASSERT_EQ(i +1 , queue_size(m_queue));
  }

  ASSERT_EQ(26, queue_size(m_queue));

  for (i = 0; i < 26; i++) {
    char *fruit = (char*)queue_pop(m_queue);
    ASSERT_STREQ(fruit, fruits[i]);
    ASSERT_EQ(26 - i - 1, queue_size(m_queue));
  }

  ASSERT_EQ(0, queue_size(m_queue));
  assert(queue_pop(m_queue) == NULL);
}

// -----------------------------------------------------------------------------
