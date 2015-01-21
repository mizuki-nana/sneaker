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

/* Unit tests for definitions defined in sneaker/functional/decorators.h */

#include "../../include/functional/decorators.h"

#include "../../include/testing/testing.h"

#include <boost/format.hpp>

#include <stdexcept>
#include <string>


using namespace sneaker::functional;


class decorators_unittest : public ::testing::Test {};


class retry_decorator_unittest : public decorators_unittest {
public:
  virtual void SetUp() {
    retry_decorator_unittest::counter = 0;
  }

  static int counter;
};


int retry_decorator_unittest::counter = 0;


TEST_F(retry_decorator_unittest, TestRetrySuccessful)
{
  const int MAX_RETRY = 5;

  retry<void> wrapper = []() -> void {
    ++retry_decorator_unittest::counter;

    if (retry_decorator_unittest::counter < MAX_RETRY) {
      throw std::runtime_error(
        str(
          boost::format(
            "Counter has reached %d"
          ) % retry_decorator_unittest::counter
        )
      );
    }
  };

  wrapper.operator()<std::runtime_error, MAX_RETRY>();

  ASSERT_EQ(MAX_RETRY, retry_decorator_unittest::counter);
}

TEST_F(retry_decorator_unittest, TestExceptionOnOverRetryLimit)
{
  const int MAX_RETRY = 5;

  retry<void> wrapper = []() -> void {
    ++retry_decorator_unittest::counter;

    if (retry_decorator_unittest::counter < MAX_RETRY) {
      throw std::runtime_error(
        str(
          boost::format(
            "Counter has reached %d"
          ) % retry_decorator_unittest::counter
        )
      );
    }
  };

  const int RETRY_LIMIT = 3; // (MAX_RETRY - 2)

  {
    bool thrown = false;
    try {
      wrapper.operator()<std::runtime_error, RETRY_LIMIT>();
    } catch (std::runtime_error) {
      thrown = true;
    }
    ASSERT_TRUE(thrown);
  }

  ASSERT_EQ(RETRY_LIMIT + 1, retry_decorator_unittest::counter);
}

TEST_F(retry_decorator_unittest, TestDecoratorChaining)
{
  retry<std::string> inner = []() -> std::string {
    return "hello world";
  };

  retry<std::string> wrapper = inner;

  std::string expected_result = "hello world";
  std::string actual_result = wrapper.operator()<std::runtime_error, 0>();

  ASSERT_EQ(expected_result, actual_result);
}

TEST_F(retry_decorator_unittest, TestDecoratorChaining2)
{
  retry<std::string> wrapper(
    retry<std::string>(
      []() -> std::string {
        return "hello world";
      }
    )
  );

  std::string expected_result = "hello world";
  std::string actual_result = wrapper.operator()<std::runtime_error, 0>();

  ASSERT_EQ(expected_result, actual_result);
}
