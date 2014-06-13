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

/* Unit test for `queue_t` defined in sneaker/libc/queue.h */

#include <cassert>
#include <climits>
#include <cstring>
#include "../../include/testing/testing.h"
#include "../../include/libc/memory.h"
#include "../../include/libc/queue.h"
#include "../../include/libc/utils.h"


class queue_unittest : public ::testing::Test {
protected:
  virtual void SetUp() {
    _queue = queue_create();
    assert(_queue);
  }

  virtual void TearDown() {
    queue_free(&_queue);
    assert(_queue == NULL);
  }

  queue_t _queue;
};


TEST_F(queue_unittest, TestCreation)
{
  assert(_queue);
  ASSERT_EQ(0, queue_size(_queue));
  assert(queue_pop(_queue) == NULL);
}

TEST_F(queue_unittest, TestPushAndPop1)
{
  int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int i;
  for(i = 0; i < 10; i++) {
    queue_push(_queue, &numbers[i], sizeof(int));
  }

  ASSERT_EQ(10, queue_size(_queue));

  for(i = 0; i < 10; i++) {
    int *p = (int*)queue_pop(_queue);
    int val = DEREF_VOID(int, p);
    ASSERT_EQ(numbers[i], val);
  }

  assert(queue_pop(_queue) == NULL);

  ASSERT_EQ(0, queue_size(_queue));
}

TEST_F(queue_unittest, TestPushAndPop2)
{
  int odds[] = {1, 3, 5, 7, 9};
  int evens[] = {2, 4, 6, 8, 10};
  
  int i;
  for(i = 0; i < 5; i++) {
    queue_push(_queue, &odds[i], sizeof(int));
  }

  ASSERT_EQ(5, queue_size(_queue));

  for(i = 0; i < 5; i++) {
    int *p = (int*)queue_pop(_queue);
    int val = DEREF_VOID(int, p);
    ASSERT_EQ(odds[i], val);
  }

  assert(queue_pop(_queue) == NULL);
  ASSERT_EQ(0, queue_size(_queue));

  for(i = 0; i < 5; i++) {
    queue_push(_queue, &evens[i], sizeof(int));
  }

  ASSERT_EQ(5, queue_size(_queue));

  for(i = 0; i < 5; i++) {
    int *p = (int*)queue_pop(_queue);
    int val = DEREF_VOID(int, p);
    ASSERT_EQ(evens[i], val);
  }

  assert(queue_pop(_queue) == NULL);
  ASSERT_EQ(0, queue_size(_queue));
}

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
  for(i = 0; i < 26; i++) {
    char *fruit = (char*)fruits[i];
    queue_push(_queue, fruit, strlen(fruit)+1); 
    ASSERT_EQ(i +1 , queue_size(_queue));
  }

  ASSERT_EQ(26, queue_size(_queue));

  for(i = 0; i < 26; i++) {
    char *fruit = (char*)queue_pop(_queue);
    ASSERT_STREQ(fruit, fruits[i]);
    ASSERT_EQ(26 - i - 1, queue_size(_queue));
  }

  ASSERT_EQ(0, queue_size(_queue));
  assert(queue_pop(_queue) == NULL);
}