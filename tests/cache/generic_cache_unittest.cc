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

/* Unit test for `generic_cache` in sneaker/cache/generic_cache.h */

#include "cache/generic_cache.h"

#include "testing/testing.h"

#include <cassert>
#include <map>
#include <string>


// -----------------------------------------------------------------------------

struct generic_cache_unittest_fixture
{
  typedef char* K;
  typedef char* T;

  std::map<K, T> truth_map;

  generic_cache_unittest_fixture()
    :
    m_cache(
      &generic_cache_unittest_fixture::StaticCreateHandler,
      &generic_cache_unittest_fixture::StaticDestroyHandler)
  {
  }

  static bool StaticCreateHandler(K key, T* value)
  {
    return instance_->CreateHandler(key, value);
  }

  static bool StaticDestroyHandler(K key, T* value)
  {
    return instance_->DestroyHandler(key, value);
  }

  static void set_instance(generic_cache_unittest_fixture* instance)
  {
    instance_ = instance;
  }

  bool CreateHandler(K key, T* value)
  {
    char* value_ = strdup(key);
    *value = value_;

    truth_map.insert(std::pair<char*, char*>(key, *value));

    return true;
  }

  bool DestroyHandler(K key, T* /* value */)
  {
    truth_map.erase(key);

    return true;
  }

  sneaker::cache::generic_cache<K, T, bool(*)(K, T*), bool(*)(K, T*)> m_cache;

private:
  static generic_cache_unittest_fixture *instance_;
};

// -----------------------------------------------------------------------------

generic_cache_unittest_fixture* generic_cache_unittest_fixture::instance_ = NULL;

// -----------------------------------------------------------------------------

class generic_cache_unittest : public ::testing::Test {
protected:

  virtual void SetUp() {
    generic_cache_unittest_fixture::set_instance(&m_fixture);
  }

  void check_item_created(char* item) const {
    check_item_in_truth_table(item, true);
  }

  void check_item_destroyed(char* item) const {
    check_item_in_truth_table(item, false);
  }

  void check_item_in_truth_table(char* item, bool in) const {
    auto itr = m_fixture.truth_map.find(item);
    ASSERT_EQ(itr != m_fixture.truth_map.end(), in);
  }

  generic_cache_unittest_fixture m_fixture;
};

// -----------------------------------------------------------------------------

TEST_F(generic_cache_unittest, TestInitialization)
{
  assert(m_fixture.m_cache.empty());
  assert(m_fixture.m_cache.size() == 0);
}

// -----------------------------------------------------------------------------

TEST_F(generic_cache_unittest, TestInsertAndFindOneItem)
{
  char* item = const_cast<char*>("abc");

  bool res = m_fixture.m_cache.put(item);
  ASSERT_EQ(true, res);
  ASSERT_EQ(1, m_fixture.m_cache.size());

  this->check_item_created(item);

  ASSERT_EQ(true, m_fixture.m_cache.member(item));

  char* item_ = NULL;
  res = m_fixture.m_cache.get(item, &item_);
  ASSERT_EQ(true, res);
  assert(item_);
  ASSERT_STREQ(item, item_);
}

// -----------------------------------------------------------------------------

TEST_F(generic_cache_unittest, TestPutAndFindMultipleItems)
{
  char* item1 = const_cast<char*>("Today is a good day!");
  char* item2 = const_cast<char*>("An apple a day, keep the doctor away.");
  char* item3 = const_cast<char*>("Better be crippled in body, than be corrputed in mind.");

  bool res = m_fixture.m_cache.put(item1);
  ASSERT_EQ(true, res);

  res = m_fixture.m_cache.put(item2);
  ASSERT_EQ(true, res);

  res = m_fixture.m_cache.put(item3);
  ASSERT_EQ(true, res);

  ASSERT_EQ(3, m_fixture.m_cache.size());

  char* item1_ = NULL;
  char* item2_ = NULL;
  char* item3_ = NULL;

  res = m_fixture.m_cache.get(item1, &item1_);
  ASSERT_EQ(true, res);
  assert(item1_);

  res = m_fixture.m_cache.get(item2, &item2_);
  ASSERT_EQ(true, res);
  assert(item2_);

  res = m_fixture.m_cache.get(item3, &item3_);
  ASSERT_EQ(true, res);
  assert(item3_);

  ASSERT_STREQ(item1, item1_);
  ASSERT_STREQ(item2, item2_);
  ASSERT_STREQ(item3, item3_);

  m_fixture.m_cache.clear();

  ASSERT_EQ(0, m_fixture.m_cache.size());

  this->check_item_destroyed(item1);
  this->check_item_destroyed(item2);
  this->check_item_destroyed(item3);

  res = m_fixture.m_cache.member(item1);
  ASSERT_EQ(false, res);

  res = m_fixture.m_cache.member(item2);
  ASSERT_EQ(false, res);

  res = m_fixture.m_cache.member(item3);
  ASSERT_EQ(false, res);
}

// -----------------------------------------------------------------------------

TEST_F(generic_cache_unittest, TestPutWithTheSameKey)
{
  char* item = const_cast<char*>("xyz");

  bool res = m_fixture.m_cache.put(item);
  ASSERT_EQ(true, res);
  ASSERT_EQ(1, m_fixture.m_cache.size());
  this->check_item_created(item);

  res = m_fixture.m_cache.put(item);
  ASSERT_EQ(false, res);
  ASSERT_EQ(1, m_fixture.m_cache.size());
  this->check_item_created(item);

  res = m_fixture.m_cache.erase(item);
  ASSERT_EQ(true, res);

  this->check_item_destroyed(item);
}

// -----------------------------------------------------------------------------

TEST_F(generic_cache_unittest, TestPutWithUpdate)
{
  char* item = const_cast<char*>("abc");

  bool res = m_fixture.m_cache.put(item);
  ASSERT_EQ(true, res);
  ASSERT_EQ(1, m_fixture.m_cache.size());
  this->check_item_created(item);

  char* item_old = nullptr;

  m_fixture.m_cache.get(item, &item_old);

  res = m_fixture.m_cache.put(item, true);
  ASSERT_EQ(true, res);
  ASSERT_EQ(1, m_fixture.m_cache.size());
  this->check_item_created(item);

  char* item_new = nullptr;

  m_fixture.m_cache.get(item, &item_new);

  /* The two values should not be equal because it's forced to be updated. */
  ASSERT_NE(item_old, item_new);
}

// -----------------------------------------------------------------------------

TEST_F(generic_cache_unittest, TestEraseOnNonExistentKey)
{
  char* non_existent_key_1 = const_cast<char*>("some key 1");
  char* non_existent_key_2 = const_cast<char*>("some key 2");
  char* non_existent_key_3 = const_cast<char*>("some key 3");

  ASSERT_EQ(0, m_fixture.m_cache.size());

  bool res = m_fixture.m_cache.erase(non_existent_key_1);
  ASSERT_EQ(false, res);

  res = m_fixture.m_cache.erase(non_existent_key_2);
  ASSERT_EQ(false, res);

  res = m_fixture.m_cache.erase(non_existent_key_3);
  ASSERT_EQ(false, res);

  ASSERT_EQ(0, m_fixture.m_cache.size());
}

// -----------------------------------------------------------------------------
