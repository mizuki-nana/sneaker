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

/* Unit test for `stack_t` defined in sneaker/libc/stack.h */

#include "../../include/libc/stack.h"

#include "../../include/libc/memory.h"
#include "../../include/libc/utils.h"
#include "../../include/testing/testing.h"

#include <cassert>
#include <climits>
#include <cstring>


// -----------------------------------------------------------------------------

class stack_unittest : public ::testing::Test {
protected:
  virtual void SetUp() {
    m_stack = stack_create();
    assert(m_stack);
  }

  virtual void TearDown() {
    stack_free(&m_stack);
    assert(m_stack == NULL);
  }

  stack_t m_stack;
};

// -----------------------------------------------------------------------------

TEST_F(stack_unittest, TestCreation)
{
  assert(m_stack);
  ASSERT_EQ(0, stack_size(m_stack));
  assert(stack_pop(m_stack) == NULL);
}

// -----------------------------------------------------------------------------

TEST_F(stack_unittest, TestPushAndPop1)
{
  int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int i;

  for (i = 0; i < 10; i++) {
    stack_push(m_stack, &numbers[i], sizeof(int));
  }

  ASSERT_EQ(10, stack_size(m_stack));

  for (i = 9; i >= 0; i--) {
    int *p = (int*)stack_pop(m_stack);
    int val = DEREF_VOID(int, p);
    ASSERT_EQ(numbers[i], val);
  }

  assert(stack_pop(m_stack) == NULL);

  ASSERT_EQ(0, stack_size(m_stack));
}

// -----------------------------------------------------------------------------

TEST_F(stack_unittest, TestPushAndPop2)
{
  int odds[] = {1, 3, 5, 7, 9};
  int evens[] = {2, 4, 6, 8, 10};

  int i;
  for (i = 0; i < 5; i++) {
    stack_push(m_stack, &odds[i], sizeof(int));
  }

  ASSERT_EQ(5, stack_size(m_stack));

  for (i = 4; i >= 0; i--) {
    int *p = (int*)stack_pop(m_stack);
    int val = DEREF_VOID(int, p);
    ASSERT_EQ(odds[i], val);
  }

  assert(stack_pop(m_stack) == NULL);
  ASSERT_EQ(0, stack_size(m_stack));

  for (i = 0; i < 5; i++) {
    stack_push(m_stack, &evens[i], sizeof(int));
  }

  ASSERT_EQ(5, stack_size(m_stack));

  for (i = 4; i >= 0; i--) {
    int *p = (int*)stack_pop(m_stack);
    int val = DEREF_VOID(int, p);
    ASSERT_EQ(evens[i], val);
  }

  assert(stack_pop(m_stack) == NULL);
  ASSERT_EQ(0, stack_size(m_stack));
}

// -----------------------------------------------------------------------------

TEST_F(stack_unittest, TestPushAndPop3)
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
    stack_push(m_stack, fruit, strlen(fruit)+1);
    ASSERT_EQ(i + 1, stack_size(m_stack));
  }

  ASSERT_EQ(26, stack_size(m_stack));

  for (i = 25; i >= 0; i--) {
    char *fruit = (char*)stack_pop(m_stack);
    ASSERT_STREQ(fruit, fruits[i]);
    ASSERT_EQ(i, stack_size(m_stack));
  }

  ASSERT_EQ(0, stack_size(m_stack));
  assert(stack_pop(m_stack) == NULL);
}

// -----------------------------------------------------------------------------
