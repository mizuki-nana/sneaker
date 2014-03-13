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

/* Unit test for functions defined in include/libc/math.h */


#include "../../include/testing/testing.h"
#include "../../include/libc/math.h"


class NearestExp2CeilUnitTest : public ::testing::Test {
public:
  void test(int input_value, int expected_value) {
    ASSERT_EQ(expected_value, nearest_exp2_ceil(input_value));
  }
};


TEST_F(NearestExp2CeilUnitTest, TestOnZero)
{
  test(0, 0);
}

TEST_F(NearestExp2CeilUnitTest, TestOnOne)
{
  test(1, 1);
}

TEST_F(NearestExp2CeilUnitTest, TestOnTwo)
{
  test(2, 2);
}

TEST_F(NearestExp2CeilUnitTest, TestOnThree)
{
  test(3, 4);
}

TEST_F(NearestExp2CeilUnitTest, TestOn250)
{
  test(250, 256);
}

TEST_F(NearestExp2CeilUnitTest, TestOn1023)
{
  test(1023, 1024);
}

TEST_F(NearestExp2CeilUnitTest, TestOn1024)
{
  test(1024, 1024);
}

TEST_F(NearestExp2CeilUnitTest, TestOn2ToThePowerOf20)
{
  test(1024 * 1024 - 1000, 1024 * 1024);
}


class NearestExp2FloorUnitTest : public ::testing::Test {
public:
  void test(int input_value, int expected_value) {
    ASSERT_EQ(expected_value, nearest_exp2_floor(input_value));
  }
};


TEST_F(NearestExp2FloorUnitTest, TestOnZero)
{
  test(0, 0);
}

TEST_F(NearestExp2FloorUnitTest, TestOnOne)
{
  test(1, 1);
}

TEST_F(NearestExp2FloorUnitTest, TestOnTwo)
{
  test(2, 2);
}

TEST_F(NearestExp2FloorUnitTest, TestOnThree)
{
  test(3, 2);
}

TEST_F(NearestExp2FloorUnitTest, TestOn255)
{
  test(255, 128);
}

TEST_F(NearestExp2FloorUnitTest, TestOn1023)
{
  test(1023, 512);
}

TEST_F(NearestExp2FloorUnitTest, TestOn1024)
{
  test(1024, 1024);
}

TEST_F(NearestExp2FloorUnitTest, TestOn2ToThePowerOf20)
{
  test(1024 * 1024 + 1000, 1024 * 1024);
}