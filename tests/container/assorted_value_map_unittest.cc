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

/* Unit test for `sneaker::container::assorted_value_map<K, ValueTypes ...>`
 * defined in sneaker/container/assorted_value_map.h
 */

#include "../../include/container/assorted_value_map.h"

#include "../../include/testing/testing.h"

#include <functional>
#include <memory>
#include <stdexcept>


class assorted_value_map_unittest_base : public ::testing::Test {};


class assorted_value_map_with_no_value_type_unittest : public assorted_value_map_unittest_base {
protected:
  sneaker::container::assorted_value_map<int> m_map;
};


TEST_F(assorted_value_map_with_no_value_type_unittest, TestInitialization)
{
  ASSERT_TRUE(m_map.empty());
  ASSERT_EQ(0, m_map.size());
}

TEST_F(assorted_value_map_with_no_value_type_unittest, TestPutAndAt)
{
  m_map.insert(1);
  m_map.insert(2);
  m_map.insert(3);

  ASSERT_FALSE(m_map.empty());
  ASSERT_EQ(3, m_map.size());
}


class assorted_value_map_with_multiple_value_types_unittest : public assorted_value_map_unittest_base {
public:
  typedef char T0;
  typedef int T1;
  typedef long T2;
  typedef bool T3;

protected:
  sneaker::container::assorted_value_map<T0, T1, T2, T3> m_map;
};


TEST_F(assorted_value_map_with_multiple_value_types_unittest, TestInitialization)
{
  ASSERT_TRUE(m_map.empty());
  ASSERT_EQ(0, m_map.size());
}

TEST_F(assorted_value_map_with_multiple_value_types_unittest, TestPutAndAt)
{
  m_map.insert('a', 1, 100, true);
  m_map.insert('b', 2, 200, true);
  m_map.insert('c', 3, 300, false);

  ASSERT_FALSE(m_map.empty());
  ASSERT_EQ(3, m_map.size());

  ASSERT_EQ(1, boost::get<0>(m_map.at('a')));
  ASSERT_EQ(2, boost::get<0>(m_map.at('b')));
  ASSERT_EQ(3, boost::get<0>(m_map.at('c')));

  ASSERT_EQ(100, boost::get<1>(m_map.at('a')));
  ASSERT_EQ(200, boost::get<1>(m_map.at('b')));
  ASSERT_EQ(300, boost::get<1>(m_map.at('c')));

  ASSERT_EQ(true,  boost::get<2>(m_map.at('a')));
  ASSERT_EQ(true,  boost::get<2>(m_map.at('b')));
  ASSERT_EQ(false, boost::get<2>(m_map.at('c')));
}

TEST_F(assorted_value_map_with_multiple_value_types_unittest, TestPutAndGet)
{
  m_map.insert('a', 1, 100, true);
  m_map.insert('b', 2, 200, true);
  m_map.insert('c', 3, 300, false);

  ASSERT_FALSE(m_map.empty());
  ASSERT_EQ(3, m_map.size());

  T1 a1 = m_map.get<T1, 0>('a');
  T2 a2 = m_map.get<T2, 1>('a');
  T3 a3 = m_map.get<T3, 2>('a');

  T1 b1 = m_map.get<T1, 0>('b');
  T2 b2 = m_map.get<T2, 1>('b');
  T3 b3 = m_map.get<T3, 2>('b');

  T1 c1 = m_map.get<T1, 0>('c');
  T2 c2 = m_map.get<T2, 1>('c');
  T3 c3 = m_map.get<T3, 2>('c');

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

TEST_F(assorted_value_map_with_multiple_value_types_unittest, TestPutAndGetByReference)
{
  m_map.insert('a', 1, 100, true);
  m_map.insert('b', 2, 200, true);
  m_map.insert('c', 3, 300, false);

  ASSERT_FALSE(m_map.empty());
  ASSERT_EQ(3, m_map.size());

  const T1& a1 = m_map.get<T1, 0>('a');
  const T2& a2 = m_map.get<T2, 1>('a');
  const T3& a3 = m_map.get<T3, 2>('a');

  const T1& b1 = m_map.get<T1, 0>('b');
  const T2& b2 = m_map.get<T2, 1>('b');
  const T3& b3 = m_map.get<T3, 2>('b');

  const T1& c1 = m_map.get<T1, 0>('c');
  const T2& c2 = m_map.get<T2, 1>('c');
  const T3& c3 = m_map.get<T3, 2>('c');

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

TEST_F(assorted_value_map_with_multiple_value_types_unittest, TestAccessByInvalidKeyFails)
{
  ASSERT_EQ(0, m_map.size());

  ASSERT_THROW(
    {
      m_map.at('X');
    },
    std::out_of_range
  );
}


class assorted_value_map_unittest : public assorted_value_map_unittest_base {};


TEST_F(assorted_value_map_unittest, TestCopyConstructor)
{
  using assorted_value_map_type = typename sneaker::container::assorted_value_map<int, bool>;

  assorted_value_map_type map1;

  map1.insert(0, false);
  map1.insert(1, true);

  ASSERT_EQ(2, map1.size());

  bool val1 = map1.get<bool, 0>(0);
  bool val2 = map1.get<bool, 0>(1);

  ASSERT_EQ(false, val1);
  ASSERT_EQ(true, val2);

  assorted_value_map_type map2 = map1;

  ASSERT_EQ(2, map2.size());

  val1 = map2.get<bool, 0>(0);
  val2 = map2.get<bool, 0>(1);

  ASSERT_EQ(false, val1);
  ASSERT_EQ(true, val2);
}

TEST_F(assorted_value_map_unittest, TestCreate)
{
  using assorted_value_map_type = typename sneaker::container::assorted_value_map<int, bool>;
  using value_type = assorted_value_map_type::value_type;

  assorted_value_map_type map = assorted_value_map_type::create<std::less<int>, std::allocator<value_type>>();

  map.insert(0, false);
  map.insert(1, true);

  ASSERT_EQ(2, map.size());

  bool val1 = map.get<bool, 0>(0);
  bool val2 = map.get<bool, 0>(1);

  ASSERT_EQ(false, val1);
  ASSERT_EQ(true, val2);
}

TEST_F(assorted_value_map_unittest, TestCreateWithArgs)
{
  using assorted_value_map_type = typename sneaker::container::assorted_value_map<int, bool>;
  using value_type = assorted_value_map_type::value_type;

  assorted_value_map_type map = assorted_value_map_type::create<std::less<int>, std::allocator<value_type>>(
    std::less<int>(),
    std::allocator<value_type>()
  );

  map.insert(0, false);
  map.insert(1, true);

  ASSERT_EQ(2, map.size());

  bool val1 = map.get<bool, 0>(0);
  bool val2 = map.get<bool, 0>(1);

  ASSERT_EQ(false, val1);
  ASSERT_EQ(true, val2);
}

TEST_F(assorted_value_map_unittest, TestSwap)
{
  typedef const char* T0_;
  typedef int T1_;
  typedef bool T2_;
  typedef const char* T3_;

  sneaker::container::assorted_value_map<T0_, T1_, T2_, T3_> map1;
  sneaker::container::assorted_value_map<T0_, T1_, T2_, T3_> map2;

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
  T1_ map1_a1 = map1.get<T1_, 0>("Apple");
  T2_ map1_a2 = map1.get<T2_, 1>("Apple");
  T3_ map1_a3 = map1.get<T3_, 2>("Apple");

  ASSERT_EQ(map1_a1, 100);
  ASSERT_EQ(map1_a2, false);
  ASSERT_EQ(map1_a3, "best thing ever :-)");

  T1_ map1_b1 = map1.get<T1_, 0>("Orange");
  T2_ map1_b2 = map1.get<T2_, 1>("Orange");
  T3_ map1_b3 = map1.get<T3_, 2>("Orange");

  ASSERT_EQ(map1_b1, 80);
  ASSERT_EQ(map1_b2, true);
  ASSERT_EQ(map1_b3, "I'm lovin' it <3");

  T1_ map1_c1 = map1.get<T1_, 0>("Bananna");
  T2_ map1_c2 = map1.get<T2_, 1>("Bananna");
  T3_ map1_c3 = map1.get<T3_, 2>("Bananna");

  ASSERT_EQ(map1_c1, 90);
  ASSERT_EQ(map1_c2, false);
  ASSERT_EQ(map1_c3, "get ALL the Potassium!!!");

  // check map2
  T1_ map2_a1 = map2.get<T1_, 0>("Grape");
  T2_ map2_a2 = map2.get<T2_, 1>("Grape");
  T3_ map2_a3 = map2.get<T3_, 2>("Grape");

  ASSERT_EQ(map2_a1, 60);
  ASSERT_EQ(map2_a2, false);
  ASSERT_EQ(map2_a3, "Too sour, don't like...");

  T1_ map2_b1 = map2.get<T1_, 0>("Watermalon");
  T2_ map2_b2 = map2.get<T2_, 1>("Watermalon");
  T3_ map2_b3 = map2.get<T3_, 2>("Watermalon");

  ASSERT_EQ(map2_b1, 90);
  ASSERT_EQ(map2_b2, false);
  ASSERT_EQ(map2_b3, "The best in the summer");
}

TEST_F(assorted_value_map_unittest, TestFind)
{
  typedef sneaker::container::assorted_value_map<const char*, int, bool> map_type;
  map_type map;

  map_type::iterator itr = map.find("Apple");
  ASSERT_EQ(itr, map.end());

  map.insert("Apple", 100, true);

  itr = map.find("Apple");
  ASSERT_NE(itr, map.end());
  ASSERT_STREQ("Apple", map.find("Apple")->first);

  map_type::mapped_type value = map.find("Apple")->second;

  ASSERT_EQ(100,  value.get<0>());
  ASSERT_EQ(true, value.get<1>());
}
