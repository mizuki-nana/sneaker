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

/* Unit test for `lru_cache` in sneaker/cache/lru_cache.h */

#include "cache/lru_cache.h"

#include "testing/testing.h"

#include <string>


// -----------------------------------------------------------------------------

namespace {

const size_t N = 3;

} /* anonymous namespace */

// -----------------------------------------------------------------------------

class lru_cache_unittest : public ::testing::Test
{
protected:
  sneaker::cache::lru_cache<std::string, size_t, N> m_cache;
};

// -----------------------------------------------------------------------------

TEST_F(lru_cache_unittest, TestInitialization)
{
  ASSERT_EQ(true, m_cache.empty());
  ASSERT_EQ(false, m_cache.full());
  ASSERT_EQ(0, m_cache.size());
}

// -----------------------------------------------------------------------------

TEST_F(lru_cache_unittest, TestInsertAndGet)
{
  std::string key1("apple");
  size_t val1 = key1.size();

  std::string key2("orange");
  size_t val2 = key2.size();

  std::string key3("banana");
  size_t val3 = key3.size();

  m_cache.insert(key1, val1);

  ASSERT_EQ(false, m_cache.empty());
  ASSERT_EQ(false, m_cache.full());
  ASSERT_EQ(1, m_cache.size());

  size_t actual_val1 = 0;
  bool res = m_cache.get(key1, actual_val1);
  ASSERT_EQ(true, res);
  ASSERT_EQ(val1, actual_val1);

  m_cache.insert(key2, val2);

  ASSERT_EQ(false, m_cache.empty());
  ASSERT_EQ(false, m_cache.full());
  ASSERT_EQ(2, m_cache.size());

  size_t actual_val2 = 0;
  res = m_cache.get(key2, actual_val2);
  ASSERT_EQ(true, res);
  ASSERT_EQ(val2, actual_val2);

  m_cache.insert(key3, val3);

  ASSERT_EQ(false, m_cache.empty());
  ASSERT_EQ(true, m_cache.full());
  ASSERT_EQ(3, m_cache.size());

  size_t actual_val3 = 0;
  res = m_cache.get(key3, actual_val3);
  ASSERT_EQ(true, res);
  ASSERT_EQ(val3, actual_val3);

  std::string* key_ptr = NULL;
  size_t* val_ptr = NULL;

  m_cache.next_erasure_pair(&key_ptr, &val_ptr);
  ASSERT_NE(nullptr, key_ptr);
  ASSERT_NE(nullptr, val_ptr);

  ASSERT_EQ(key1, *key_ptr);
  ASSERT_EQ(val1, *val_ptr);

  std::string key4("watermelon");
  size_t val4 = key4.size();

  m_cache.insert(key4, val4);

  ASSERT_EQ(false, m_cache.empty());
  ASSERT_EQ(true, m_cache.full());
  ASSERT_EQ(3, m_cache.size());

  res = m_cache.find(key1);
  ASSERT_EQ(false, res);

  size_t actual_val4 = 0;
  res = m_cache.get(key4, actual_val4);
  ASSERT_EQ(true, res);
  ASSERT_EQ(val4, actual_val4);

  m_cache.clear();

  ASSERT_EQ(true, m_cache.empty());
  ASSERT_EQ(false, m_cache.full());
  ASSERT_EQ(0, m_cache.size());
}

// -----------------------------------------------------------------------------

TEST_F(lru_cache_unittest, TestInsertAndErase)
{
  std::string key("apple");
  size_t val = key.size();

  m_cache.insert(key, val);

  ASSERT_EQ(false, m_cache.empty());
  ASSERT_EQ(false, m_cache.full());
  ASSERT_EQ(1, m_cache.size());

  bool res = m_cache.find(key);
  ASSERT_EQ(true, res);

  std::string fake_key("grape");
  res = m_cache.erase(fake_key);
  ASSERT_EQ(false, res);

  ASSERT_EQ(false, m_cache.empty());
  ASSERT_EQ(false, m_cache.full());
  ASSERT_EQ(1, m_cache.size());

  res = m_cache.erase(key);
  ASSERT_EQ(true, res);

  ASSERT_EQ(true, m_cache.empty());
  ASSERT_EQ(false, m_cache.full());
  ASSERT_EQ(0, m_cache.size());
}

// -----------------------------------------------------------------------------
