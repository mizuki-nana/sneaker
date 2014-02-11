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

/* Unit test for include/container/assorted_value_map.h */

#include "../_unittest.h"
#include "../../include/libc/assert.h"
#include "../../include/container/assorted_value_map.h"


using namespace sneaker::container;


class AssortedValueMapUnitTestBase : public ::testing::Test {};


class AssortedValueMapWithNoValueTypeUnitTest : public AssortedValueMapUnitTestBase {
protected:
  virtual void SetUp() {
    _map = new sneaker::container::assorted_value_map<int>();
  }

  virtual void TearDown() {
    delete _map;
  }

  sneaker::container::assorted_value_map<int> * _map;
};


TEST_F(AssortedValueMapWithNoValueTypeUnitTest, TestInitialization)
{
  ASSERT(_map);
  ASSERT_TRUE(_map->empty());
  ASSERT_EQ(0, _map->size());
}

TEST_F(AssortedValueMapWithNoValueTypeUnitTest, TestPutAndAt)
{
  _map->insert(1);
  _map->insert(2);
  _map->insert(3);

  ASSERT_FALSE(_map->empty());
  ASSERT_EQ(3, _map->size());
}


class AssortedValueMapWithMultiplyValueTypesUnitTest : public AssortedValueMapUnitTestBase {
protected:
  virtual void SetUp() {
    _map = new sneaker::container::assorted_value_map<char, int, long, bool>();
  }

  virtual void TearDown() {
    delete _map;
  }

  sneaker::container::assorted_value_map<char, int, long, bool> * _map;
};

TEST_F(AssortedValueMapWithMultiplyValueTypesUnitTest, TestInitialization)
{
  ASSERT(_map);
  ASSERT_TRUE(_map->empty());
  ASSERT_EQ(0, _map->size());
}

TEST_F(AssortedValueMapWithMultiplyValueTypesUnitTest, TestPutAndAt)
{
  _map->insert('a', 1, 100, true);
  _map->insert('b', 2, 200, true);
  _map->insert('c', 3, 300, false);

  ASSERT_FALSE(_map->empty());
  ASSERT_EQ(3, _map->size());

  ASSERT_EQ(1, boost::get<0>(_map->at('a')));
  ASSERT_EQ(2, boost::get<0>(_map->at('b')));
  ASSERT_EQ(3, boost::get<0>(_map->at('c')));

  ASSERT_EQ(100, boost::get<1>(_map->at('a')));
  ASSERT_EQ(200, boost::get<1>(_map->at('b')));
  ASSERT_EQ(300, boost::get<1>(_map->at('c')));

  ASSERT_EQ(true,  boost::get<2>(_map->at('a')));
  ASSERT_EQ(true,  boost::get<2>(_map->at('b')));
  ASSERT_EQ(false, boost::get<2>(_map->at('c')));
}

TEST_F(AssortedValueMapWithMultiplyValueTypesUnitTest, TestPutAndGet)
{
  _map->insert('a', 1, 100, true);
  _map->insert('b', 2, 200, true);
  _map->insert('c', 3, 300, false);

  ASSERT_FALSE(_map->empty());
  ASSERT_EQ(3, _map->size());

  int  a1 = _map->get<int,  0>('a');
  long a2 = _map->get<long, 1>('a');
  bool a3 = _map->get<bool, 2>('a');

  int  b1 = _map->get<int,  0>('b');
  long b2 = _map->get<long, 1>('b');
  bool b3 = _map->get<bool, 2>('b');

  int  c1 = _map->get<int,  0>('c');
  long c2 = _map->get<long, 1>('c');
  bool c3 = _map->get<bool, 2>('c');

  ASSERT_EQ(1, a1);
  ASSERT_EQ(100, a2);
  ASSERT_EQ(true, a3);

  ASSERT_EQ(2, b1);
  ASSERT_EQ(200, b2);
  ASSERT_EQ(true, b3);

  ASSERT_EQ(3, c1);
  ASSERT_EQ(300, c2);
  ASSERT_EQ(false, c3);
}

TEST_F(AssortedValueMapWithMultiplyValueTypesUnitTest, TestPutAndGetByReference)
{
  _map->insert('a', 1, 100, true);
  _map->insert('b', 2, 200, true);
  _map->insert('c', 3, 300, false);

  ASSERT_FALSE(_map->empty());
  ASSERT_EQ(3, _map->size());

  const int  &a1 = _map->get<int,  0>('a');
  const long &a2 = _map->get<long, 1>('a');
  const bool &a3 = _map->get<bool, 2>('a');

  const int  &b1 = _map->get<int,  0>('b');
  const long &b2 = _map->get<long, 1>('b');
  const bool &b3 = _map->get<bool, 2>('b');

  const int  &c1 = _map->get<int,  0>('c');
  const long &c2 = _map->get<long, 1>('c');
  const bool &c3 = _map->get<bool, 2>('c');

  ASSERT_EQ(1, a1);
  ASSERT_EQ(100, a2);
  ASSERT_EQ(true, a3);

  ASSERT_EQ(2, b1);
  ASSERT_EQ(200, b2);
  ASSERT_EQ(true, b3);

  ASSERT_EQ(3, c1);
  ASSERT_EQ(300, c2);
  ASSERT_EQ(false, c3);
}


class AssortedValueMapUnitTest : public AssortedValueMapUnitTestBase {};


TEST_F(AssortedValueMapUnitTest, TestSwap)
{
  sneaker::container::assorted_value_map<char*, int, bool, char*> map1;
  sneaker::container::assorted_value_map<char*, int, bool, char*> map2;

  map1.insert("Grape",      60,  false, "Too sour, don't like...");
  map1.insert("Watermalon", 90,  false, "The best in the summer");

  map2.insert("Apple",      100, false, "best thing ever :-)");
  map2.insert("Orange",     80,  true,  "I'm lovin' it <3");
  map2.insert("Bananna",    90,  false, "get ALL the Potassium!!!");

  ASSERT_EQ(2, map1.size());
  ASSERT_EQ(3, map2.size());

  // do the swap magic...
  map1.swap(map2);

  ASSERT_EQ(3, map1.size());
  ASSERT_EQ(2, map2.size());

  // check map1
  int   map1_a1 = map1.get<int,   0>("Apple");      ASSERT_EQ(map1_a1, 100);
  bool  map1_a2 = map1.get<bool,  1>("Apple");      ASSERT_EQ(map1_a2, false);
  char* map1_a3 = map1.get<char*, 2>("Apple");      ASSERT_EQ(map1_a3, "best thing ever :-)");

  int   map1_b1 = map1.get<int,   0>("Orange");     ASSERT_EQ(map1_b1, 80);
  bool  map1_b2 = map1.get<bool,  1>("Orange");     ASSERT_EQ(map1_b2, true);
  char* map1_b3 = map1.get<char*, 2>("Orange");     ASSERT_EQ(map1_b3, "I'm lovin' it <3");

  int   map1_c1 = map1.get<int,   0>("Bananna");    ASSERT_EQ(map1_c1, 90);
  bool  map1_c2 = map1.get<bool,  1>("Bananna");    ASSERT_EQ(map1_c2, false);
  char* map1_c3 = map1.get<char*, 2>("Bananna");    ASSERT_EQ(map1_c3, "get ALL the Potassium!!!");

  // check map2
  int   map2_a1 = map2.get<int,   0>("Grape");      ASSERT_EQ(map2_a1, 60);
  bool  map2_a2 = map2.get<bool,  1>("Grape");      ASSERT_EQ(map2_a2, false);
  char* map2_a3 = map2.get<char*, 2>("Grape");      ASSERT_EQ(map2_a3, "Too sour, don't like...");

  int   map2_b1 = map2.get<int,   0>("Watermalon"); ASSERT_EQ(map2_b1, 90);
  bool  map2_b2 = map2.get<bool,  1>("Watermalon"); ASSERT_EQ(map2_b2, false);
  char* map2_b3 = map2.get<char*, 2>("Watermalon"); ASSERT_EQ(map2_b3, "The best in the summer");
}

TEST_F(AssortedValueMapUnitTest, TestFind)
{
  typedef sneaker::container::assorted_value_map<char*, int, bool> _map_type;
  _map_type map;

  _map_type::iterator itr = map.find("Apple");
  ASSERT_EQ(itr, map.end());

  map.insert("Apple", 100, true);

  itr = map.find("Apple");
  ASSERT_NE(itr, map.end());
  ASSERT_STREQ("Apple", map.find("Apple")->first);

  _map_type::value_type value = map.find("Apple")->second;

  ASSERT_EQ(100,  value.get<0>());
  ASSERT_EQ(true, value.get<1>());
}