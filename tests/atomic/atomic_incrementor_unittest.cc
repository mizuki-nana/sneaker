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

/* Unit test for definitions in sneaker/atomic/atomic_incrementor.h */

#include <climits>
#include <iostream>
#include <stdexcept>
#include <pthread.h>
#include "../../include/testing/testing.h"
#include "../../include/atomic/atomic_incrementor.h"


class atomic_incrementor_unittest : public ::testing::Test {};


TEST_F(atomic_incrementor_unittest, TestDefaultConstructor)
{
  sneaker::atomic::atomic_incrementor<int, INT_MAX> incrementor;
  ASSERT_TRUE(0 == incrementor);
}

TEST_F(atomic_incrementor_unittest, TestConstrutorWithInitialValue)
{
  sneaker::atomic::atomic_incrementor<int, INT_MAX> incrementor(10);
  ASSERT_TRUE(10 == incrementor);
}

TEST_F(atomic_incrementor_unittest, TestCopyConstructor)
{
  sneaker::atomic::atomic_incrementor<int, INT_MAX> incrementor(100);
  sneaker::atomic::atomic_incrementor<int, INT_MAX> incrementor2 = incrementor;

  ASSERT_TRUE(100 == incrementor2);
}

TEST_F(atomic_incrementor_unittest, TestAssignmentOperator)
{
  sneaker::atomic::atomic_incrementor<int, INT_MAX> incrementor;
  incrementor = 100;

  ASSERT_TRUE(100 == incrementor);
}

TEST_F(atomic_incrementor_unittest, TestIncrementsSuccessful)
{
  sneaker::atomic::atomic_incrementor<int, INT_MAX> incrementor;

  for(int i = 0; i < 10; i++) {
    ++incrementor;
    ASSERT_TRUE(i+1 == incrementor);
  }
}

TEST_F(atomic_incrementor_unittest, TestIncrementUntilOverflow)
{
  sneaker::atomic::atomic_incrementor<unsigned char, UCHAR_MAX> incrementor;

  for(unsigned char i = 0; i < UCHAR_MAX; i++) {
    ++incrementor;
    ASSERT_TRUE(i+1 == incrementor);
  }

  ASSERT_THROW(
    { ++incrementor; },
    std::underflow_error
  );
}

TEST_F(atomic_incrementor_unittest, TestIncrementUntilUnderflow)
{
  sneaker::atomic::atomic_incrementor<signed char, SCHAR_MAX> incrementor;

  for(signed char i = 0; i < SCHAR_MAX; i++) {
    ++incrementor;
    ASSERT_TRUE(i+1 == incrementor);
  }

  ASSERT_THROW(
    { ++incrementor; },
    std::underflow_error
  );
}


class atomic_incrementor_equality_test : public atomic_incrementor_unittest {};


TEST_F(atomic_incrementor_equality_test, TestEquality)
{
  sneaker::atomic::atomic_incrementor<int, INT_MAX> incrementor1;
  sneaker::atomic::atomic_incrementor<int, INT_MAX> incrementor2;

  ASSERT_TRUE(incrementor1 == incrementor2);
  ASSERT_FALSE(incrementor1 != incrementor2);
  ASSERT_EQ(incrementor1, incrementor2);
}

TEST_F(atomic_incrementor_equality_test, TestInequality)
{
  sneaker::atomic::atomic_incrementor<int, INT_MAX> incrementor1(1);
  sneaker::atomic::atomic_incrementor<int, INT_MAX> incrementor2(2);

  ASSERT_TRUE(incrementor1 != incrementor2);
  ASSERT_FALSE(incrementor1 == incrementor2);
  ASSERT_NE(incrementor1, incrementor2);
}

TEST_F(atomic_incrementor_equality_test, TestEqualityOnDifferentType)
{
  sneaker::atomic::atomic_incrementor<int, INT_MAX> incrementor1(1);
  sneaker::atomic::atomic_incrementor<char, SCHAR_MAX> incrementor2(1);

  ASSERT_EQ(incrementor1, incrementor2);
}

TEST_F(atomic_incrementor_equality_test, TestInequalityOnDifferentType)
{
  sneaker::atomic::atomic_incrementor<int, INT_MAX> incrementor1(1);
  sneaker::atomic::atomic_incrementor<char, SCHAR_MAX> incrementor2(2);

  ASSERT_NE(incrementor1, incrementor2);
}


class atomic_incrementor_atomicity_test : public atomic_incrementor_unittest {
public:
  struct arg {
    sneaker::atomic::atomic_incrementor<int, INT_MAX>* ptr;
  };

  static void* increment(void* ptr) {
    struct arg *arg_ptr = (struct arg*)(ptr);
    struct arg arg = static_cast<struct arg>(*arg_ptr);

    sneaker::atomic::atomic_incrementor<int, INT_MAX> &incrementor = *arg.ptr;

    int previous_value = incrementor;

    ++incrementor;

    int actual_value = incrementor;
    assert(previous_value + 1 == actual_value);

    pthread_exit(NULL);
    return NULL;
  }
};


TEST_F(atomic_incrementor_atomicity_test, TestAtomicity)
{
  sneaker::atomic::atomic_incrementor<int, INT_MAX> incrementor;

  const int NUM_THREADS = 5;
  pthread_t threads[NUM_THREADS];
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  for(int i = 0; i < NUM_THREADS; ++i) {
    struct atomic_incrementor_atomicity_test::arg arg = {
      .ptr = &incrementor
    };

    int res = pthread_create(
      &threads[i], &attr, atomic_incrementor_atomicity_test::increment, (void*)(&arg));

    if (res) {
      std::cerr << "Failed to create thread. ERROR: " << res << std::endl; 
    }
  }

  for(int i = 0; i < NUM_THREADS; ++i) {
    void* status;
    int res = pthread_join(threads[i], &status);

    if (res) {
      std::cerr << "Failed to join thread. ERROR: " << res << std::endl; 
    }
  }
}
