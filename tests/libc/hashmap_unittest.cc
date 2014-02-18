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

/* Unit test for `hashmap_t` defined in include/libc/hashmap.h */


#include "../../include/testing/testing.h"
#include "../../include/libc/assert.h"
#include "../../include/libc/c_str.h"
#include "../../include/libc/hash.h"
#include "../../include/libc/hashmap.h"
#include "../../include/libc/utils.h"


hash_t simple_hash(c_str s) {
  hash_t h = 0;
  int i;

  for(i = 0; i < strlen(s); i++) {
    h = 127 * h + s[i];
  }

  return h;
}

hash_t hashfunc(void *key) {
  RETURN_VAL_IF_NULL(key, 0);
  c_str s = (c_str)key;
  return simple_hash(s);
}

int keycmpfunc(void *key1, void *key2) {
  return strcmp((c_str)key1, (c_str)key2) == 0;  
}

#define HASHMAP_INITIAL_CAPACITY 10

typedef struct {
  c_str key;
  c_str val;
} KeyVal;


KeyVal fruits[] = {
  {
    (c_str)"a",
    (c_str)"apple"
  },
  {
    (c_str)"b",
    (c_str)"banana"
  },
  {
    (c_str)"c",
    (c_str)"coconut"
  }
};

KeyVal sky[] = {
  {
    (c_str)"a",
    (c_str)"air"
  },
  {
    (c_str)"b",
    (c_str)"balloon"
  },
  {
    (c_str)"c",
    (c_str)"cloud"
  }
};

KeyVal vehicles[] = {
  {
    (c_str)"a",
    (c_str)"airplane"
  },
  {
    (c_str)"b",
    (c_str)"boat"
  },
  {
    (c_str)"c",
    (c_str)"car"
  }
};


class HashmapUnitTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    _hashmap = hashmap_create(
      HASHMAP_INITIAL_CAPACITY, hashfunc, keycmpfunc);
  }

  virtual void TearDown() {
    if(_hashmap) {
      hashmap_free(&_hashmap);
    }
  }

  hashmap_t _hashmap;
};


TEST_F(HashmapUnitTest, TestHashmapCreationSuccessful)
{
  ASSERT(_hashmap);
  ASSERT_EQ(hashmap_size(_hashmap), 0);
  ASSERT_EQ(hashmap_bucketcount(_hashmap), 8);
}

TEST_F(HashmapUnitTest, TestHashmapPutSingleItem)
{
  ASSERT(_hashmap);

  hashmap_put(_hashmap, fruits[0].key, fruits[0].val);
  hashmap_put(_hashmap, fruits[1].key, fruits[1].val); 
  hashmap_put(_hashmap, fruits[2].key, fruits[2].val);

  ASSERT_EQ(3, hashmap_size(_hashmap));
  ASSERT_EQ(8, hashmap_bucketcount(_hashmap));

  ASSERT_TRUE(hashmap_contains_key(_hashmap, fruits[0].key));
  ASSERT_TRUE(hashmap_contains_key(_hashmap, fruits[1].key));
  ASSERT_TRUE(hashmap_contains_key(_hashmap, fruits[2].key));

  ASSERT_STREQ(fruits[0].val, (c_str)hashmap_get(_hashmap, fruits[0].key));
  ASSERT_STREQ(fruits[1].val, (c_str)hashmap_get(_hashmap, fruits[1].key));
  ASSERT_STREQ(fruits[2].val, (c_str)hashmap_get(_hashmap, fruits[2].key));

  ASSERT_STREQ(fruits[0].val, (c_str)hashmap_remove(_hashmap, fruits[0].key));
  ASSERT_STREQ(fruits[1].val, (c_str)hashmap_remove(_hashmap, fruits[1].key));
  ASSERT_STREQ(fruits[2].val, (c_str)hashmap_remove(_hashmap, fruits[2].key));

  ASSERT_EQ(0, hashmap_size(_hashmap));
  ASSERT_EQ(6, hashmap_capacity(_hashmap));
}

TEST_F(HashmapUnitTest, TestHashmapPutMultipleValues)
{
  ASSERT(_hashmap);

  hashmap_put(_hashmap, fruits[0].key, fruits[0].val);
  hashmap_put(_hashmap, fruits[1].key, fruits[1].val);
  hashmap_put(_hashmap, fruits[2].key, fruits[2].val);

  /* Fruits */
  ASSERT_TRUE(hashmap_contains_key(_hashmap, fruits[0].key));
  ASSERT_TRUE(hashmap_contains_key(_hashmap, fruits[1].key));
  ASSERT_TRUE(hashmap_contains_key(_hashmap, fruits[2].key));

  ASSERT_EQ(3, hashmap_size(_hashmap));
  ASSERT_EQ(8, hashmap_bucketcount(_hashmap));

  ASSERT_STREQ(fruits[0].val, (c_str)hashmap_get(_hashmap, fruits[0].key));
  ASSERT_STREQ(fruits[1].val, (c_str)hashmap_get(_hashmap, fruits[1].key));
  ASSERT_STREQ(fruits[2].val, (c_str)hashmap_get(_hashmap, fruits[2].key));

  ASSERT_STREQ(fruits[0].val, (c_str)hashmap_remove(_hashmap, fruits[0].key));
  ASSERT_STREQ(fruits[1].val, (c_str)hashmap_remove(_hashmap, fruits[1].key));
  ASSERT_STREQ(fruits[2].val, (c_str)hashmap_remove(_hashmap, fruits[2].key));
 
  ASSERT_EQ(0, hashmap_size(_hashmap));

  /* Vehicles */
  hashmap_put(_hashmap, vehicles[0].key, vehicles[0].val);
  hashmap_put(_hashmap, vehicles[1].key, vehicles[1].val);
  hashmap_put(_hashmap, vehicles[2].key, vehicles[2].val);
  
  ASSERT_TRUE(hashmap_contains_key(_hashmap, vehicles[0].key));
  ASSERT_TRUE(hashmap_contains_key(_hashmap, vehicles[1].key));
  ASSERT_TRUE(hashmap_contains_key(_hashmap, vehicles[2].key));

  ASSERT_EQ(3, hashmap_size(_hashmap));
  
  ASSERT_STREQ(vehicles[0].val, (c_str)hashmap_get(_hashmap, vehicles[0].key));
  ASSERT_STREQ(vehicles[1].val, (c_str)hashmap_get(_hashmap, vehicles[1].key));
  ASSERT_STREQ(vehicles[2].val, (c_str)hashmap_get(_hashmap, vehicles[2].key));

  ASSERT_STREQ(vehicles[0].val, (c_str)hashmap_remove(_hashmap, vehicles[0].key));
  ASSERT_STREQ(vehicles[1].val, (c_str)hashmap_remove(_hashmap, vehicles[1].key));
  ASSERT_STREQ(vehicles[2].val, (c_str)hashmap_remove(_hashmap, vehicles[2].key));

  ASSERT_EQ(0, hashmap_size(_hashmap));

  /* Sky */
  hashmap_put(_hashmap, sky[0].key, sky[0].val);
  hashmap_put(_hashmap, sky[1].key, sky[1].val);
  hashmap_put(_hashmap, sky[2].key, sky[2].val);

  ASSERT_TRUE(hashmap_contains_key(_hashmap, sky[0].key));
  ASSERT_TRUE(hashmap_contains_key(_hashmap, sky[1].key));
  ASSERT_TRUE(hashmap_contains_key(_hashmap, sky[2].key));

  ASSERT_EQ(3, hashmap_size(_hashmap));

  ASSERT_STREQ(sky[0].val, (c_str)hashmap_get(_hashmap, sky[0].key));
  ASSERT_STREQ(sky[1].val, (c_str)hashmap_get(_hashmap, sky[1].key));
  ASSERT_STREQ(sky[2].val, (c_str)hashmap_get(_hashmap, sky[2].key));

  ASSERT_STREQ(sky[0].val, (c_str)hashmap_remove(_hashmap, sky[0].key));
  ASSERT_STREQ(sky[1].val, (c_str)hashmap_remove(_hashmap, sky[1].key));
  ASSERT_STREQ(sky[2].val, (c_str)hashmap_remove(_hashmap, sky[2].key));

  ASSERT_EQ(0, hashmap_size(_hashmap));
}

TEST_F(HashmapUnitTest, TestHashmapGetWithNonExistentKeys)
{
  ASSERT(_hashmap);

  hashmap_put(_hashmap, fruits[0].key, fruits[0].val);
  hashmap_put(_hashmap, fruits[1].key, fruits[1].val);
  hashmap_put(_hashmap, fruits[2].key, fruits[2].val);

  ASSERT_TRUE(hashmap_contains_key(_hashmap, fruits[0].key));
  ASSERT_TRUE(hashmap_contains_key(_hashmap, fruits[1].key));
  ASSERT_TRUE(hashmap_contains_key(_hashmap, fruits[2].key));

  ASSERT_EQ(3, hashmap_size(_hashmap));
  ASSERT_EQ(8, hashmap_bucketcount(_hashmap));

  ASSERT_STREQ(NULL, (c_str)hashmap_get(_hashmap, (char*)"x"));
  ASSERT_STREQ(NULL, (c_str)hashmap_get(_hashmap, (char*)"y"));
  ASSERT_STREQ(NULL, (c_str)hashmap_get(_hashmap, (char*)"Z"));

  ASSERT_EQ(3, hashmap_size(_hashmap));
}

TEST_F(HashmapUnitTest, TestHashmapFree)
{
  ASSERT(_hashmap);

  hashmap_put(_hashmap, fruits[0].key, fruits[0].val);
  hashmap_put(_hashmap, fruits[1].key, fruits[1].val);
  hashmap_put(_hashmap, fruits[2].key, fruits[2].val);

  ASSERT_TRUE(hashmap_contains_key(_hashmap, fruits[0].key));
  ASSERT_TRUE(hashmap_contains_key(_hashmap, fruits[1].key));
  ASSERT_TRUE(hashmap_contains_key(_hashmap, fruits[2].key));

  ASSERT_EQ(3, hashmap_size(_hashmap));
  ASSERT_EQ(8, hashmap_bucketcount(_hashmap));

  hashmap_free(&_hashmap);

  ASSERT_EQ(NULL, _hashmap);
}