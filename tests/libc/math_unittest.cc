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

/* Unit test for functions defined in sneaker/libc/math.h */

#include "../../include/libc/math.h"

#include "../../include/testing/testing.h"


// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit test for:
 * unsigned int nearest_exp2_ceil(unsigned int)
 ******************************************************************************/
class nearest_exp2_ceil_unittest : public ::testing::Test {
public:
  void test(int input_value, int expected_value) {
    ASSERT_EQ(expected_value, nearest_exp2_ceil(input_value));
  }
};

// -----------------------------------------------------------------------------

TEST_F(nearest_exp2_ceil_unittest, TestOnValue)
{
  test(0, 0);
  test(1, 1);
  test(2, 2);
  test(3, 4);
  test(250, 256);
  test(1023, 1024);
  test(1024, 1024);
}

// -----------------------------------------------------------------------------

TEST_F(nearest_exp2_ceil_unittest, TestOn2ToThePowerOf20)
{
  test(1024 * 1024 - 1000, 1024 * 1024);
}

// -----------------------------------------------------------------------------

/*******************************************************************************
 * Unit test for:
 * unsigned int nearest_exp2_floor(unsigned int)
 ******************************************************************************/
class nearest_exp2_floor_unittest : public ::testing::Test {
public:
  void test(int input_value, int expected_value) {
    ASSERT_EQ(expected_value, nearest_exp2_floor(input_value));
  }
};

// -----------------------------------------------------------------------------

TEST_F(nearest_exp2_floor_unittest, TestOnValue)
{
  test(0, 0);
  test(1, 1);
  test(2, 2);
  test(3, 2);
  test(255, 128);
  test(1023, 512);
  test(1024, 1024);
}

// -----------------------------------------------------------------------------

TEST_F(nearest_exp2_floor_unittest, TestOn2ToThePowerOf20)
{
  test(1024 * 1024 + 1000, 1024 * 1024);
}

// -----------------------------------------------------------------------------
