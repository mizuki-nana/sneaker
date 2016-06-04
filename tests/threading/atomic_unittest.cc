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

/* Unit test for definitions in sneaker/threading/atomic.h */

#include "threading/atomic.h"
#include "libc/assert.h"

#include "testing/testing.h"

#include <climits>
#include <iostream>
#include <pthread.h>


// -----------------------------------------------------------------------------

class atomic_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(atomic_unittest, TestDefaultConstructor)
{
  sneaker::threading::atomic<int, INT_MAX> value;
  ASSERT_EQ(0, value);
}

// -----------------------------------------------------------------------------

TEST_F(atomic_unittest, TestConstrutorWithInitialValue)
{
  sneaker::threading::atomic<int, INT_MAX> value(10);
  ASSERT_EQ(10, value);
}

// -----------------------------------------------------------------------------

TEST_F(atomic_unittest, TestCopyConstructor)
{
  sneaker::threading::atomic<int, INT_MAX> value(100);
  sneaker::threading::atomic<int, INT_MAX> value2 = value;

  ASSERT_EQ(100, value2);
}

// -----------------------------------------------------------------------------

TEST_F(atomic_unittest, TestCopyConstructor2)
{
  sneaker::threading::atomic<int, INT_MAX> value(INT_MAX);
  sneaker::threading::atomic<int, 200> value2(value);

  ASSERT_EQ(0, value2);
}

// -----------------------------------------------------------------------------

TEST_F(atomic_unittest, TestAssignmentOperator)
{
  sneaker::threading::atomic<int, INT_MAX> value;
  value = 100;

  ASSERT_EQ(100, value);
}

// -----------------------------------------------------------------------------

TEST_F(atomic_unittest, TestAssignmentOperator2)
{
  sneaker::threading::atomic<int, 100> value(100);
  sneaker::threading::atomic<int, 200> value2(200);
  value = value2;

  ASSERT_EQ(0, value);
}

// -----------------------------------------------------------------------------

TEST_F(atomic_unittest, TestIncrementsSuccessful)
{
  sneaker::threading::atomic<int, INT_MAX> value;

  for (int i = 0; i < 10; i++) {
    ++value;
    ASSERT_EQ(i + 1, value);
  }
}

// -----------------------------------------------------------------------------

TEST_F(atomic_unittest, TestIncrementUntilOverflow)
{
  sneaker::threading::atomic<unsigned char, 100> value;

  for (unsigned char i = 0; i < 100; i++) {
    ++value;
    ASSERT_TRUE(i + 1 == value);
  }

  ++value;

  ASSERT_EQ(0, value);
}

// -----------------------------------------------------------------------------

class atomic_equality_test : public atomic_unittest {};

// -----------------------------------------------------------------------------

TEST_F(atomic_equality_test, TestEquality)
{
  sneaker::threading::atomic<int, INT_MAX> value1;
  sneaker::threading::atomic<int, INT_MAX> value2;

  ASSERT_TRUE(value1 == value2);
  ASSERT_FALSE(value1 != value2);
  ASSERT_EQ(value1, value2);
}

// -----------------------------------------------------------------------------

TEST_F(atomic_equality_test, TestInequality)
{
  sneaker::threading::atomic<int, INT_MAX> value1(1);
  sneaker::threading::atomic<int, INT_MAX> value2(2);

  ASSERT_TRUE(value1 != value2);
  ASSERT_FALSE(value1 == value2);
  ASSERT_NE(value1, value2);
}

// -----------------------------------------------------------------------------

TEST_F(atomic_equality_test, TestEqualityOnDifferentType)
{
  sneaker::threading::atomic<int, INT_MAX> value1(1);
  sneaker::threading::atomic<char, SCHAR_MAX> value2(1);

  ASSERT_NE(value1, value2);
}

// -----------------------------------------------------------------------------

TEST_F(atomic_equality_test, TestInequalityOnDifferentType)
{
  sneaker::threading::atomic<int, INT_MAX> value1(1);
  sneaker::threading::atomic<char, SCHAR_MAX> value2(2);

  ASSERT_NE(value1, value2);
}

// -----------------------------------------------------------------------------

class atomic_atomicity_test : public atomic_unittest {
public:
  struct arg {
    sneaker::threading::atomic<int, INT_MAX>* ptr;

    arg(sneaker::threading::atomic<int, INT_MAX>* ptr_)
      :
      ptr(ptr_)
    {
    }
  };

  static void* increment(void* ptr) {
    struct arg *arg_ptr = (struct arg*)(ptr);
    struct arg arg = static_cast<struct arg>(*arg_ptr);

    sneaker::threading::atomic<int, INT_MAX> &value = *arg.ptr;

    const int previous_value = value.value();

    ++value;

    const int actual_value = value.value();
    ASSERT(previous_value + 1 == actual_value);

    pthread_exit(NULL);
    return NULL;
  }
};

// -----------------------------------------------------------------------------

TEST_F(atomic_atomicity_test, TestAtomicity)
{
  sneaker::threading::atomic<int, INT_MAX> value;

  const size_t NUM_THREADS = 5;
  pthread_t threads[NUM_THREADS] = {0};
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  for (size_t i = 0; i < NUM_THREADS; ++i) {
    struct atomic_atomicity_test::arg arg(&value);

    const int res = pthread_create(&threads[i], &attr,
      atomic_atomicity_test::increment, (void*)(&arg));

    if (res) {
      std::cerr << "Failed to create thread. ERROR: " << res << std::endl;
    }
  }

  for (size_t i = 0; i < NUM_THREADS; ++i) {
    void* status;
    int res = pthread_join(threads[i], &status);

    if (res) {
      std::cerr << "Failed to join thread. ERROR: " << res << std::endl;
    }
  }
}

// -----------------------------------------------------------------------------
