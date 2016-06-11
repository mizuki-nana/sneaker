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

/* Unit tests for definitions defined in sneaker/utility/util.numeric.h */

#include "utility/util.numeric.h"

#include "testing/testing.h"


// -----------------------------------------------------------------------------

using namespace sneaker::utility;

// -----------------------------------------------------------------------------

class util_numeric_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(util_numeric_unittest, TestFloatsEqualWithDefaultTolerance)
{
  ASSERT_TRUE(floats_equal(1.0, 1.0001));
  ASSERT_FALSE(floats_equal(1.0, 2.0));
}

// -----------------------------------------------------------------------------

TEST_F(util_numeric_unittest, TestFloatsEqualWithCustomTolerance)
{
  ASSERT_FALSE(floats_equal(1.0, 1.0001, 0.0000001));
  ASSERT_TRUE(floats_equal(1.0, 1.05, 0.06));
}

// -----------------------------------------------------------------------------
