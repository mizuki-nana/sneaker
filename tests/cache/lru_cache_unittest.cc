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

/* Unit test for `generic_cache` in sneaker/cache/lru_cache.h */

#include "cache/cache_interface.h"
#include "cache/lru_cache.h"

#include "testing/testing.h"

#include <unordered_map>


// -----------------------------------------------------------------------------

namespace {

const size_t N = 3;

} /* anonymous namespace */

// -----------------------------------------------------------------------------

struct lru_cache_unittest_fixture
{
  typedef const char* K;
  typedef const char* T;

  std::unordered_map<K, T> truth_map;

  lru_cache_unittest_fixture()
    :
    m_cache(
      &lru_cache_unittest_fixture::StaticOnInsert,
      &lru_cache_unittest_fixture::StaticOnErase)
  {
  }

  static void StaticOnInsert(K key, const T& value)
  {
    instance_->OnInsert(key, value);
  }

  static void StaticOnErase(K key, const T& value)
  {
    instance_->OnErase(key, value);
  }

  static void set_instance(lru_cache_unittest_fixture* instance)
  {
    instance_ = instance;
  }

  void OnInsert(K key, const T& value)
  {
    truth_map.insert(std::make_pair(key, value));
  }

  void OnErase(K key, const T& /* value */)
  {
    truth_map.erase(key);
  }

  sneaker::cache::cache_interface<sneaker::cache::lru_cache<K, T, N>,
    void(*)(K, const T&), void(*)(K, const T&)> m_cache;

private:
  static lru_cache_unittest_fixture *instance_;
};

// -----------------------------------------------------------------------------

lru_cache_unittest_fixture* lru_cache_unittest_fixture::instance_ = NULL;

// -----------------------------------------------------------------------------

class lru_cache_unittest : public ::testing::Test {
protected:

  virtual void SetUp() {
    lru_cache_unittest_fixture::set_instance(&m_fixture);
  }

  void check_item_created(const char* item) const {
    check_item_in_truth_table(item, true);
  }

  void check_item_destroyed(const char* item) const {
    check_item_in_truth_table(item, false);
  }

  void check_item_in_truth_table(const char* item, bool in) const {
    auto itr = m_fixture.truth_map.find(item);
    ASSERT_EQ(itr != m_fixture.truth_map.end(), in);
  }

  lru_cache_unittest_fixture m_fixture;
};

// -----------------------------------------------------------------------------

TEST_F(lru_cache_unittest, TestInitialization)
{
  assert(m_fixture.m_cache.empty());
  assert(m_fixture.m_cache.size() == 0);
}

// -----------------------------------------------------------------------------

TEST_F(lru_cache_unittest, TestInsertAndFindOneItem)
{
  const char* key = "a";
  const char* value = "apple";

  m_fixture.m_cache.insert(key, value);
  ASSERT_EQ(1, m_fixture.m_cache.size());

  check_item_created(key);

  ASSERT_EQ(true, m_fixture.m_cache.find(key));

  const char* actual_value = NULL;
  const bool res = m_fixture.m_cache.get(key, actual_value);
  ASSERT_EQ(true, res);
  assert(actual_value);
  ASSERT_STREQ(value, actual_value);
}

// -----------------------------------------------------------------------------

TEST_F(lru_cache_unittest, TestInsertLessThanNItems)
{
  const size_t M = N - 1;

  const char* keys[M] = { "a", "b" };
  const char* values[M] = { "airplane", "boat" };

  for (size_t i = 0; i < M; ++i)
  {
    m_fixture.m_cache.insert(keys[i], values[i]);
  }

  ASSERT_EQ(false, m_fixture.m_cache.empty());

  ASSERT_EQ(M, m_fixture.m_cache.size());
  ASSERT_EQ(M, m_fixture.truth_map.size());

  const char* actual_value = NULL;
  bool res = m_fixture.m_cache.get("a", actual_value);

  ASSERT_EQ(true, res);
  assert(actual_value);
  ASSERT_STREQ("airplane", actual_value);

  res = m_fixture.m_cache.get("b", actual_value);

  ASSERT_EQ(true, res);
  assert(actual_value);
  ASSERT_STREQ("boat", actual_value);

  m_fixture.m_cache.insert("c", "car");

  ASSERT_EQ(N, m_fixture.m_cache.size());
  ASSERT_EQ(N, m_fixture.truth_map.size());

  ASSERT_EQ(true, m_fixture.m_cache.find("c"));

  res = m_fixture.m_cache.get("c", actual_value);

  ASSERT_EQ(true, res);
  assert(actual_value);
  ASSERT_STREQ("car", actual_value);

  // Test clearing on the cache.
  m_fixture.m_cache.clear();

  ASSERT_EQ(0, m_fixture.m_cache.size());

  // Truth map's size is still `N` because calling `clear()` does not
  // invoke the erase handler.
  ASSERT_EQ(N, m_fixture.truth_map.size());

  ASSERT_EQ(false, m_fixture.m_cache.find("a"));
  ASSERT_EQ(false, m_fixture.m_cache.find("b"));
  ASSERT_EQ(false, m_fixture.m_cache.find("c"));
}

// -----------------------------------------------------------------------------

TEST_F(lru_cache_unittest, TestInsertOverNItems)
{
  const char* keys[N] = { "a", "b", "c" };
  const char* values[N] = { "apple", "banana", "coconut" };

  for (size_t i = 0; i < N; ++i)
  {
    m_fixture.m_cache.insert(keys[i], values[i]);
  }

  ASSERT_EQ(false, m_fixture.m_cache.empty());

  ASSERT_EQ(N, m_fixture.m_cache.size());
  ASSERT_EQ(N, m_fixture.truth_map.size());

  m_fixture.m_cache.insert("g", "grape");

  ASSERT_EQ(N, m_fixture.m_cache.size());
  ASSERT_EQ(N, m_fixture.truth_map.size());

  const char* actual_value = NULL;
  const bool res = m_fixture.m_cache.get("g", actual_value);
  ASSERT_EQ(true, res);
  assert(actual_value);
  ASSERT_STREQ("grape", actual_value);

  ASSERT_EQ(false, m_fixture.m_cache.find("a"));
}

// -----------------------------------------------------------------------------
