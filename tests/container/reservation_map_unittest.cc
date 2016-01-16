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

/* Unit test for `reservation_map<T>` defined in
 * sneaker/container/m_reservation_map.h
 */

#include "../../include/container/reservation_map.h"

#include "../../include/testing/testing.h"

#include <boost/uuid/uuid.hpp>

#include <cassert>


// -----------------------------------------------------------------------------

class reservation_unittest : public ::testing::Test {
public:
  typedef class fixture {
  public:
    explicit fixture(int i) : i(i)
    {
    }

    bool operator < (const fixture& rhs) const {
      return i < rhs.i;
    }

    bool operator ==(const fixture& rhs) const {
      return i == rhs.i;
    }

    int i;
  } T;

  sneaker::container::reservation_map<T>::token_t generate_token() {
    return sneaker::container::reservation_map<T>::generator_type()();
  }

protected:
  sneaker::container::reservation_map<T> m_reservation_map;
};

// -----------------------------------------------------------------------------

TEST_F(reservation_unittest, TestInitialization)
{
  ASSERT_EQ(0, this->m_reservation_map.size());
}

// -----------------------------------------------------------------------------

TEST_F(reservation_unittest, TestMemberTestOnInvalidKeys)
{
  sneaker::container::reservation_map<reservation_unittest::T>::token_t invalid_id_1 = generate_token();
  sneaker::container::reservation_map<reservation_unittest::T>::token_t invalid_id_2 = generate_token();
  sneaker::container::reservation_map<reservation_unittest::T>::token_t invalid_id_3 = generate_token();

  ASSERT_FALSE(this->m_reservation_map.member(invalid_id_1));
  ASSERT_FALSE(this->m_reservation_map.member(invalid_id_2));
  ASSERT_FALSE(this->m_reservation_map.member(invalid_id_3));

  ASSERT_FALSE(this->m_reservation_map.get(invalid_id_1, NULL));
  ASSERT_FALSE(this->m_reservation_map.get(invalid_id_2, NULL));
  ASSERT_FALSE(this->m_reservation_map.get(invalid_id_3, NULL));
}

// -----------------------------------------------------------------------------

TEST_F(reservation_unittest, TestReservationsSuccessfulOnValidKeys)
{
  reservation_unittest::T expected_value_1(1);
  reservation_unittest::T expected_value_2(2);
  reservation_unittest::T expected_value_3(3);

  sneaker::container::reservation_map<reservation_unittest::T>::token_t id_1 = this->m_reservation_map.reserve();
  sneaker::container::reservation_map<reservation_unittest::T>::token_t id_2 = this->m_reservation_map.reserve();
  sneaker::container::reservation_map<reservation_unittest::T>::token_t id_3 = this->m_reservation_map.reserve();

  ASSERT_TRUE(this->m_reservation_map.member(id_1));
  ASSERT_TRUE(this->m_reservation_map.member(id_2));
  ASSERT_TRUE(this->m_reservation_map.member(id_3));

  ASSERT_TRUE(this->m_reservation_map.put(id_1, expected_value_1));
  ASSERT_TRUE(this->m_reservation_map.put(id_2, expected_value_2));
  ASSERT_TRUE(this->m_reservation_map.put(id_3, expected_value_3));

  ASSERT_EQ(3, this->m_reservation_map.size());

  reservation_unittest::T actual_value_1(0);
  reservation_unittest::T actual_value_2(0);
  reservation_unittest::T actual_value_3(0);

  ASSERT_TRUE(this->m_reservation_map.get(id_1, &actual_value_1));
  ASSERT_TRUE(this->m_reservation_map.get(id_2, &actual_value_2));
  ASSERT_TRUE(this->m_reservation_map.get(id_3, &actual_value_3));

  ASSERT_EQ(expected_value_1, actual_value_1);
  ASSERT_EQ(expected_value_2, actual_value_2);
  ASSERT_EQ(expected_value_3, actual_value_3);
}

// -----------------------------------------------------------------------------

TEST_F(reservation_unittest, TestReserveAndUnreserve)
{
  reservation_unittest::T expected_value(100);
  sneaker::container::reservation_map<reservation_unittest::T>::token_t id = this->m_reservation_map.reserve();

  this->m_reservation_map.put(id, expected_value);

  ASSERT_TRUE(this->m_reservation_map.member(id));

  reservation_unittest::T actual_value(0);
  ASSERT_TRUE(this->m_reservation_map.get(id, &actual_value));

  ASSERT_EQ(expected_value, actual_value);

  ASSERT_TRUE(this->m_reservation_map.unreserve(id));

  reservation_unittest::T invalid_value(0);

  ASSERT_FALSE(this->m_reservation_map.get(id, &invalid_value));

  ASSERT_EQ(0, invalid_value.i);
}

// -----------------------------------------------------------------------------

TEST_F(reservation_unittest, TestUnrserveOnInvalidKey)
{
  sneaker::container::reservation_map<reservation_unittest::T>::token_t invalid_id = generate_token();

  ASSERT_FALSE(this->m_reservation_map.unreserve(invalid_id));
}

// -----------------------------------------------------------------------------

TEST_F(reservation_unittest, TestClear)
{
  sneaker::container::reservation_map<reservation_unittest::T>::token_t id_1 = this->m_reservation_map.reserve();
  sneaker::container::reservation_map<reservation_unittest::T>::token_t id_2 = this->m_reservation_map.reserve();
  sneaker::container::reservation_map<reservation_unittest::T>::token_t id_3 = this->m_reservation_map.reserve();

  reservation_unittest::T value_1(1);
  reservation_unittest::T value_2(20);
  reservation_unittest::T value_3(300);

  ASSERT_TRUE(this->m_reservation_map.put(id_1, value_1));
  ASSERT_TRUE(this->m_reservation_map.put(id_2, value_2));
  ASSERT_TRUE(this->m_reservation_map.put(id_3, value_3));

  ASSERT_EQ(3, this->m_reservation_map.size());

  this->m_reservation_map.clear();

  ASSERT_EQ(0, this->m_reservation_map.size());
}

// -----------------------------------------------------------------------------
