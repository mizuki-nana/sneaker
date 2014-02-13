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

/* Unit test for include/container/reservation_map.h */

#include "../../include/libc/uuid.h" /* this needs to come before _unittest.h */
#include "../../include/testing/testing.h"
#include "../../include/libc/assert.h"
#include "../../include/container/reservation_map.h"

class ReservationMapUnitTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    _reservation_map = new sneaker::container::reservation_map<int>();
  }

  virtual void TearDown() {
    delete _reservation_map;
  }

  sneaker::container::reservation_map<int> * _reservation_map;
};

TEST_F(ReservationMapUnitTest, TestInitialization)
{
  ASSERT(_reservation_map);
  ASSERT_EQ(0, _reservation_map->size());
}

TEST_F(ReservationMapUnitTest, TestMemberTestOnInvalidKeys)
{
  token_t invalid_id_1 = uuid_create_and_hash();
  token_t invalid_id_2 = uuid_create_and_hash();
  token_t invalid_id_3 = uuid_create_and_hash();

  ASSERT(invalid_id_1);
  ASSERT(invalid_id_2);
  ASSERT(invalid_id_3);

  ASSERT_FALSE(_reservation_map->member(invalid_id_1));
  ASSERT_FALSE(_reservation_map->member(invalid_id_2));
  ASSERT_FALSE(_reservation_map->member(invalid_id_3));

  ASSERT_FALSE(_reservation_map->get(invalid_id_1, NULL));
  ASSERT_FALSE(_reservation_map->get(invalid_id_2, NULL));
  ASSERT_FALSE(_reservation_map->get(invalid_id_3, NULL));
}

TEST_F(ReservationMapUnitTest, TestReservationsSuccessfulOnValidKeys)
{
  int expected_value_1 = 1;
  int expected_value_2 = 20;
  int expected_value_3 = 300;

  token_t id_1 = _reservation_map->reserve();
  token_t id_2 = _reservation_map->reserve();
  token_t id_3 = _reservation_map->reserve();

  ASSERT(id_1);
  ASSERT(id_2);
  ASSERT(id_3);

  ASSERT_TRUE(_reservation_map->member(id_1));
  ASSERT_TRUE(_reservation_map->member(id_2));
  ASSERT_TRUE(_reservation_map->member(id_3));

  ASSERT_TRUE(_reservation_map->put(id_1, expected_value_1));
  ASSERT_TRUE(_reservation_map->put(id_2, expected_value_2));
  ASSERT_TRUE(_reservation_map->put(id_3, expected_value_3));

  ASSERT_EQ(3, _reservation_map->size());

  int actual_value_1 = 0;
  int actual_value_2 = 0;
  int actual_value_3 = 0;

  ASSERT_TRUE(_reservation_map->get(id_1, &actual_value_1));
  ASSERT_TRUE(_reservation_map->get(id_2, &actual_value_2));
  ASSERT_TRUE(_reservation_map->get(id_3, &actual_value_3));

  ASSERT_EQ(expected_value_1, actual_value_1);
  ASSERT_EQ(expected_value_2, actual_value_2);
  ASSERT_EQ(expected_value_3, actual_value_3);
}

TEST_F(ReservationMapUnitTest, TestClear)
{
  token_t id_1 = _reservation_map->reserve();
  token_t id_2 = _reservation_map->reserve();
  token_t id_3 = _reservation_map->reserve();

  int expected_value_1 = 1;
  int expected_value_2 = 20;
  int expected_value_3 = 300;

  ASSERT_TRUE(_reservation_map->put(id_1, expected_value_1));
  ASSERT_TRUE(_reservation_map->put(id_2, expected_value_2));
  ASSERT_TRUE(_reservation_map->put(id_3, expected_value_3));

  ASSERT_EQ(3, _reservation_map->size());

  _reservation_map->clear();

  ASSERT_EQ(0, _reservation_map->size());
}