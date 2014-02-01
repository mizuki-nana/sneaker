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

/* Unit test for `GenericCache` in src/cache/generic_cache.h */

#include <map>
#include <string>
#include "../_unittest.h"
#include "../../src/base/assert.h"
#include "../../src/base/c_str.h"
#include "../../src/cache/generic_cache.h"

#ifndef GENERIC_CACHE_UNITTEST
#define GENERIC_CACHE_UNITTEST

std::map<c_str, c_str> truth_map;

typedef c_str K;
typedef c_str T;

bool CreateHandler(K key, T* value)
{
  c_str value_ = strdup(key);
  *value = value_;

  truth_map.insert(std::pair<c_str, c_str>(key, *value));

  return true;
}

bool DestroyHandler(K key, T* value)
{
  truth_map.erase(key);

  return true;
}

class GenericCacheTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    _cache = new GenericCache<K, T, bool(*)(K, T*), bool(*)(K, T*)>(
      CreateHandler,
      DestroyHandler
    );
  }

  virtual void TearDown() {
    _cache->clear();
    delete _cache;
    truth_map.clear();
  }

  void check_item_created(c_str item) {
    _check_item_in_truth_table(item, true);
  }

  void check_item_destroyed(c_str item) {
    _check_item_in_truth_table(item, false);
  }

  void _check_item_in_truth_table(c_str item, bool in) {
    typename std::map<c_str, c_str>::iterator itr;
    itr = truth_map.find(item);
    ASSERT_EQ(itr != truth_map.end(), in);
  }

  GenericCache<K, T, bool(*)(K, T*), bool(*)(K, T*)> * _cache;
};


TEST_F(GenericCacheTest, TestInitialization)
{
  ASSERT(_cache);
  ASSERT(_cache->empty());
  ASSERT(_cache->size() == 0);
}

TEST_F(GenericCacheTest, TestInsertAndFindOneItem)
{
  bool res;

  c_str item = "abc";

  res = _cache->put(item);
  ASSERT_EQ(true, res);
  ASSERT_EQ(1, _cache->size());
  this->check_item_created(item);

  ASSERT_EQ(true, _cache->member(item));

  c_str item_ = NULL;
  res = _cache->get(item, &item_);
  ASSERT_EQ(true, res);
  ASSERT(item_);
  ASSERT_STREQ(item, item_);
}

TEST_F(GenericCacheTest, TestInsertAndFindMultipleItems)
{
  bool res;

  c_str item1 = "Today is a good day!";
  c_str item2 = "An apple a day, keep the doctor away.";
  c_str item3 = "Better be crippled in body, than be corrputed in mind.";

  res = _cache->put(item1); ASSERT_EQ(true, res);
  res = _cache->put(item2); ASSERT_EQ(true, res);
  res = _cache->put(item3); ASSERT_EQ(true, res);

  ASSERT_EQ(3, _cache->size());

  c_str item1_ = NULL;
  c_str item2_ = NULL;
  c_str item3_ = NULL;

  res = _cache->get(item1, &item1_); ASSERT_EQ(true, res); ASSERT(item1_);
  res = _cache->get(item2, &item2_); ASSERT_EQ(true, res); ASSERT(item2_);
  res = _cache->get(item3, &item3_); ASSERT_EQ(true, res); ASSERT(item3_);

  ASSERT_STREQ(item1, item1_);
  ASSERT_STREQ(item2, item2_);
  ASSERT_STREQ(item3, item3_);

  _cache->clear();

  ASSERT_EQ(0, _cache->size());

  this->check_item_destroyed(item1);
  this->check_item_destroyed(item2);
  this->check_item_destroyed(item3);

  res = _cache->member(item1); ASSERT_EQ(false, res);
  res = _cache->member(item2); ASSERT_EQ(false, res);
  res = _cache->member(item3); ASSERT_EQ(false, res);
}

TEST_F(GenericCacheTest, TestInsertWithTheSameKey)
{
  bool res;

  c_str item = "xyz";

  res = _cache->put(item);
  ASSERT_EQ(true, res);
  ASSERT_EQ(1, _cache->size());
  this->check_item_created(item);

  res = _cache->put(item);
  ASSERT_EQ(false, res);
  ASSERT_EQ(1, _cache->size());
  this->check_item_created(item);

  res = _cache->erase(item);
  ASSERT_EQ(true, res);

  this->check_item_destroyed(item);
}

TEST_F(GenericCacheTest, TestEraseOnNonExistentKey)
{
  bool res;

  ASSERT_EQ(0, _cache->size());

  res = _cache->erase("some key 1");
  ASSERT_EQ(false, res);

  res = _cache->erase("some key 2");
  ASSERT_EQ(false, res);

  res = _cache->erase("some key 3");
  ASSERT_EQ(false, res);

  ASSERT_EQ(0, _cache->size());
}

#endif /* GENERIC_CACHE_UNITTEST */

#undef GENERIC_CACHE_UNITTEST