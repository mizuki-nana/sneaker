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

/*
 * Fixture-based test that allows test fixtures to be added in tests,
 * which are collected and deallocated in teardown.
 */

#ifndef SNEAKER_FIXTURE_BASED_TEST_H_
#define SNEAKER_FIXTURE_BASED_TEST_H_

#include <gtest/gtest.h>

#include <vector>


namespace sneaker {
namespace testing {

template<class T>
class fixture_based_test : public ::testing::Test {
  typedef void(*FixtureTeardownHandler)(T);

public:
  fixture_based_test(FixtureTeardownHandler);

  void add_fixture(T);

protected:
  virtual void TearDown();

  std::vector<T> m_fixtures;
  FixtureTeardownHandler m_teardown_handler;
};

// -----------------------------------------------------------------------------

template<class T>
fixture_based_test<T>::fixture_based_test(
  FixtureTeardownHandler teardown_handler
):
  m_teardown_handler(teardown_handler)
{
}

// -----------------------------------------------------------------------------

template<class T>
void
fixture_based_test<T>::add_fixture(T fixture)
{
  m_fixtures.push_back(fixture);
}

// -----------------------------------------------------------------------------

template<class T>
void
fixture_based_test<T>::TearDown()
{
  typename std::vector<T>::iterator itr;
  for (itr = m_fixtures.begin(); itr != m_fixtures.end(); itr++) {
    m_teardown_handler(static_cast<T>(*itr));
  }
}

// -----------------------------------------------------------------------------

} /* end namespace testing */
} /* end namespace sneaker */


#endif /* SNEAKER_FIXTURE_BASED_TEST_H_ */
