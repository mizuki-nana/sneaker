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

/* Unit test for definitions in sneaker/allocator/allocator.h */

#include "../../include/allocator/allocator.h"

#include "../../include/testing/testing.h"

#include <cassert>
#include <climits>
#include <string>
#include <vector>


/*******************************************************************************
 * Unit test for:
 * sneaker::allocator::object_traits<T>
 ******************************************************************************/
class object_traits_unittest : public ::testing::Test {
public:
  class Dummy {
    public:
      Dummy(int i_, double d_) : i(i_), d(d_)
      {
      }

      int i;
      double d;
  };
};


TEST_F(object_traits_unittest, TestInitialization)
{
  sneaker::allocator::object_traits<int> object_trait;
}

TEST_F(object_traits_unittest, TestConstructAndDestroy)
{
  sneaker::allocator::object_traits<int> object_trait;
  int *p = static_cast<int*>( malloc(sizeof(int)) );

  object_trait.construct(p, 1);

  ASSERT_NE(nullptr, p);

  ASSERT_EQ(1, *p);

  object_trait.destroy(p);
}

TEST_F(object_traits_unittest, TestConstructAndDestroyCPP11)
{
  typedef object_traits_unittest::Dummy _Dummy;

  sneaker::allocator::object_traits<_Dummy> object_trait;
  _Dummy *p = static_cast<Dummy*>( malloc(sizeof(_Dummy)) );

  object_trait.construct<_Dummy, int, double>(p, 1, 1.0);

  ASSERT_NE(nullptr, p);

  ASSERT_EQ(1, p->i);
  ASSERT_EQ(1.0, p->d);

  object_trait.destroy(p);
}

TEST_F(object_traits_unittest, TestAddress)
{
  sneaker::allocator::object_traits<int> object_trait;

  int i1 = 1;
  const int i2 = 2;

  ASSERT_EQ(&i1, object_trait.address(i1));
  ASSERT_EQ(&i2, object_trait.address(i2));
}


/*******************************************************************************
 * Unit test for:
 * sneaker::allocator::standard_alloc_policy<T>
 ******************************************************************************/
class standard_alloc_policy_unittest : public ::testing::Test {
public:
  class Dummy {
    public:
      Dummy(int i_, double d_) : i(i_), d(d_) {}
      int i;
      double d;
  };
};


TEST_F(standard_alloc_policy_unittest, TestMaxSize)
{
  sneaker::allocator::standard_alloc_policy<int> alloc_policy;

  ASSERT_GT(alloc_policy.max_size(), 0);
  ASSERT_LE(alloc_policy.max_size(), ULONG_MAX);
}

TEST_F(standard_alloc_policy_unittest, TestAllocateAndDeallocate)
{
  sneaker::allocator::standard_alloc_policy<int> alloc_policy;
  const int N = 5;

  int *nums = alloc_policy.allocate(N);
  assert(nums);

  for (int i = 0; i < N; ++i) nums[i] = i;
  for (int i = 0; i < N; ++i) ASSERT_EQ(i, nums[i]);

  alloc_policy.deallocate(nums, N);
}

TEST_F(standard_alloc_policy_unittest, TestEquality)
{
  ASSERT_TRUE(
    sneaker::allocator::standard_alloc_policy<int>() ==
    sneaker::allocator::standard_alloc_policy<double>()
  );

  ASSERT_FALSE(
    sneaker::allocator::standard_alloc_policy<int>() !=
    sneaker::allocator::standard_alloc_policy<double>()
  );
}

TEST_F(standard_alloc_policy_unittest, TestInequality)
{
  typedef standard_alloc_policy_unittest::Dummy _Dummy;

  ASSERT_FALSE(
    sneaker::allocator::standard_alloc_policy<int>() ==
    _Dummy(1, 1.0)
  );

  ASSERT_TRUE(
    sneaker::allocator::standard_alloc_policy<int>() !=
    _Dummy(1, 1.0)
  );
}


/*******************************************************************************
 * Unit test for:
 * sneaker::allocator::allocator<T, Policy, Traits>
 ******************************************************************************/
class allocator_simple_unittest : public ::testing::Test {
public:
  template<typename T>
  class DummyAllocPolicy1 : public sneaker::allocator::standard_alloc_policy<T> {
    public:
      DummyAllocPolicy1():i(1) {}
      template<typename U>
      DummyAllocPolicy1(U const&) {}
      int i;
  };

  template<typename T>
  class DummyAllocPolicy2 : public sneaker::allocator::standard_alloc_policy<T> {
    public:
      DummyAllocPolicy2():i(2) {}
      template<typename U>
      DummyAllocPolicy2(U const&) {}
      int i;
  };
};


TEST_F(allocator_simple_unittest, TestCopyConstructor1)
{
  sneaker::allocator::allocator<int, allocator_simple_unittest::DummyAllocPolicy1<int>>  allocator1;
  sneaker::allocator::allocator<double, allocator_simple_unittest::DummyAllocPolicy2<double>> allocator2;

  ASSERT_EQ(1, allocator1.i);
  ASSERT_EQ(2, allocator2.i);

  allocator1 = allocator2;

  ASSERT_NE(1, allocator1.i);
  ASSERT_EQ(2, allocator2.i);
}

TEST_F(allocator_simple_unittest, TestAllocation)
{
  sneaker::allocator::allocator<int> allocator;
  int *a = allocator.allocate(10);

  assert(a);

  for (int i = 0; i < 10; ++i) a[i] = i;
  for (int i = 0; i < 10; ++i) ASSERT_EQ(i, a[i]);

  allocator.deallocate(a, 10);
}

TEST_F(allocator_simple_unittest, TestAllocationAndDestroy)
{
  sneaker::allocator::allocator<std::string> allocator;
  std::string* s = allocator.allocate(2);

  allocator.construct(s, "foo");
  allocator.construct(s + 1, "bar");

  ASSERT_STREQ("foo", s[0].c_str());
  ASSERT_STREQ("bar", s[1].c_str());

  allocator.destroy(s);
  allocator.destroy(s + 1);
  allocator.deallocate(s, 2);
}


/*******************************************************************************
 * Unit test for:
 * sneaker::allocator::allocator<T, Policy, Traits>
 ******************************************************************************/
class allocator_in_std_container_unittest : public ::testing::Test {
public:
  template<typename T>
  using vector_type = typename std::vector<T, sneaker::allocator::allocator<T>>;
};


TEST_F(allocator_in_std_container_unittest, TestInitialization)
{
  vector_type<int> v(10);
  ASSERT_EQ(10, v.capacity());
  ASSERT_EQ(10, v.size());
}

TEST_F(allocator_in_std_container_unittest, TestPushback)
{
  vector_type<char> *v = new vector_type<char>();
  assert(v);

  v->push_back('a');
  v->push_back('b');
  v->push_back('c');

  ASSERT_EQ(3, v->size());

  delete v;
}

TEST_F(allocator_in_std_container_unittest, TestPushBackWithPointers)
{
  vector_type<int*> v;

  v.push_back(new int(0));
  v.push_back(new int(1));
  v.push_back(new int(2));

  ASSERT_EQ(0, static_cast<int>(*v[0]));
  ASSERT_EQ(1, static_cast<int>(*v[1]));
  ASSERT_EQ(2, static_cast<int>(*v[2]));

  ASSERT_EQ(3, v.size());
}


/*******************************************************************************
 * Unit test for:
 * sneaker::allocator::allocator<T, Policy, Traits>
 ******************************************************************************/
class allocator_equality_unittest : public allocator_simple_unittest {};


TEST_F(allocator_equality_unittest, TestEquality1)
{
  const sneaker::allocator::allocator<int> allocator1;
  const sneaker::allocator::allocator<int> allocator2;

  ASSERT_TRUE(allocator1 == allocator2);
  ASSERT_FALSE(allocator1 != allocator2);
}

TEST_F(allocator_equality_unittest, TestEquality2)
{
  sneaker::allocator::allocator<int, allocator_equality_unittest::DummyAllocPolicy1<int>> allocator1;
  sneaker::allocator::allocator<int, allocator_equality_unittest::DummyAllocPolicy2<int>> allocator2;

  ASSERT_FALSE(allocator1 == allocator2);
  ASSERT_TRUE(allocator1 != allocator2);
}

TEST_F(allocator_equality_unittest, TestEquality3)
{
  sneaker::allocator::allocator<int> allocator1;
  std::allocator<int>                allocator2;

  ASSERT_FALSE(allocator1 == allocator2);
  ASSERT_TRUE(allocator1 != allocator2);
}
