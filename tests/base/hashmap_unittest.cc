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

/* Unit test for src/base/hashmap.c */


#include "../_unittest.h"
#include "../../src/base/assert.h"
#include "../../src/base/hash.h"
#include "../../src/base/hashmap.h"
#include "../../src/base/utils.h"


hash_t simple_hash(char *s) {
  hash_t h = 0;
  int i;

  for(i = 0; i < strlen(s); i++) {
    h = 127 * h + s[i];
  }

  return h;
}

hash_t hashfunc(void *key) {
  RETURN_VAL_IF_NULL(key, 0);
  char *s = (char*)key;
  return simple_hash(s);
}

int keycmpfunc(void *key1, void *key2) {
  return strcmp((char*)key1, (char*)key2) == 0;  
}

#define HASHMAP_INITIAL_CAPACITY 10

typedef struct {
  char *key;
  char *val;
} KeyVal;

KeyVal fruits[] = {
  {"a", "apple"},
  {"b", "banana"},
  {"c", "coconut"}
};

KeyVal sky[] = {
  {"a", "air"},
  {"b", "balloon"},
  {"c", "cloud"}
};

KeyVal vehicles[] = {
  {"a", "airplane"},
  {"b", "boat"},
  {"c", "car"}
};


class HashmapTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    _hashmap = hashmap_create(
      HASHMAP_INITIAL_CAPACITY,
      hashfunc,
      keycmpfunc
    );
  }

  virtual void TearDown() {
    if(_hashmap) {
      hashmap_free(&_hashmap);
    }
  }

  Hashmap _hashmap;
};


TEST_F(HashmapTest, HashmapCreateTest) {
  ASSERT(_hashmap);
  ASSERT_EQ(hashmap_size(_hashmap), 0);
  ASSERT_EQ(
      hashmap_bucketcount(_hashmap),
      8
  );
}

TEST_F(HashmapTest, HashmapPutSingleTest) {
  ASSERT(_hashmap);

  hashmap_put(_hashmap, fruits[0].key, fruits[0].val);
  hashmap_put(_hashmap, fruits[1].key, fruits[1].val); 
  hashmap_put(_hashmap, fruits[2].key, fruits[2].val);

  ASSERT_EQ(3, hashmap_size(_hashmap));
  ASSERT_EQ(8, hashmap_bucketcount(_hashmap));

  ASSERT_TRUE(hashmap_contains_key(_hashmap, fruits[0].key));
  ASSERT_TRUE(hashmap_contains_key(_hashmap, fruits[1].key));
  ASSERT_TRUE(hashmap_contains_key(_hashmap, fruits[2].key));

  ASSERT_STREQ(fruits[0].val, (char*)hashmap_get(_hashmap, fruits[0].key));
  ASSERT_STREQ(fruits[1].val, (char*)hashmap_get(_hashmap, fruits[1].key));
  ASSERT_STREQ(fruits[2].val, (char*)hashmap_get(_hashmap, fruits[2].key));

  ASSERT_STREQ(fruits[0].val, (char*)hashmap_remove(_hashmap, fruits[0].key));
  ASSERT_STREQ(fruits[1].val, (char*)hashmap_remove(_hashmap, fruits[1].key));
  ASSERT_STREQ(fruits[2].val, (char*)hashmap_remove(_hashmap, fruits[2].key));

  ASSERT_EQ(0, hashmap_size(_hashmap));
  ASSERT_EQ(6, hashmap_capacity(_hashmap));
}

TEST_F(HashmapTest, HashmapPutMultipleValPerKeyTest) {
  ASSERT(_hashmap);

  /************ fruits ************/
  hashmap_put(_hashmap, fruits[0].key, fruits[0].val);
  hashmap_put(_hashmap, fruits[1].key, fruits[1].val);
  hashmap_put(_hashmap, fruits[2].key, fruits[2].val);

  ASSERT_TRUE(hashmap_contains_key(_hashmap, fruits[0].key));
  ASSERT_TRUE(hashmap_contains_key(_hashmap, fruits[1].key));
  ASSERT_TRUE(hashmap_contains_key(_hashmap, fruits[2].key));

  ASSERT_EQ(3, hashmap_size(_hashmap));
  ASSERT_EQ(8, hashmap_bucketcount(_hashmap));

  ASSERT_STREQ(fruits[0].val, (char*)hashmap_get(_hashmap, fruits[0].key));
  ASSERT_STREQ(fruits[1].val, (char*)hashmap_get(_hashmap, fruits[1].key));
  ASSERT_STREQ(fruits[2].val, (char*)hashmap_get(_hashmap, fruits[2].key));

  ASSERT_STREQ(fruits[0].val, (char*)hashmap_remove(_hashmap, fruits[0].key));
  ASSERT_STREQ(fruits[1].val, (char*)hashmap_remove(_hashmap, fruits[1].key));
  ASSERT_STREQ(fruits[2].val, (char*)hashmap_remove(_hashmap, fruits[2].key));
 
  ASSERT_EQ(0, hashmap_size(_hashmap));

  /************* vehicles **************/ 
  hashmap_put(_hashmap, vehicles[0].key, vehicles[0].val);
  hashmap_put(_hashmap, vehicles[1].key, vehicles[1].val);
  hashmap_put(_hashmap, vehicles[2].key, vehicles[2].val);
  
  ASSERT_TRUE(hashmap_contains_key(_hashmap, vehicles[0].key));
  ASSERT_TRUE(hashmap_contains_key(_hashmap, vehicles[1].key));
  ASSERT_TRUE(hashmap_contains_key(_hashmap, vehicles[2].key));

  ASSERT_EQ(3, hashmap_size(_hashmap));
  
  ASSERT_STREQ(vehicles[0].val, (char*)hashmap_get(_hashmap, vehicles[0].key));
  ASSERT_STREQ(vehicles[1].val, (char*)hashmap_get(_hashmap, vehicles[1].key));
  ASSERT_STREQ(vehicles[2].val, (char*)hashmap_get(_hashmap, vehicles[2].key));

  ASSERT_STREQ(vehicles[0].val, (char*)hashmap_remove(_hashmap, vehicles[0].key));
  ASSERT_STREQ(vehicles[1].val, (char*)hashmap_remove(_hashmap, vehicles[1].key));
  ASSERT_STREQ(vehicles[2].val, (char*)hashmap_remove(_hashmap, vehicles[2].key));

  ASSERT_EQ(0, hashmap_size(_hashmap));

  /************* sky *****************/
  hashmap_put(_hashmap, sky[0].key, sky[0].val);
  hashmap_put(_hashmap, sky[1].key, sky[1].val);
  hashmap_put(_hashmap, sky[2].key, sky[2].val);

  ASSERT_TRUE(hashmap_contains_key(_hashmap, sky[0].key));
  ASSERT_TRUE(hashmap_contains_key(_hashmap, sky[1].key));
  ASSERT_TRUE(hashmap_contains_key(_hashmap, sky[2].key));

  ASSERT_EQ(3, hashmap_size(_hashmap));

  ASSERT_STREQ(sky[0].val, (char*)hashmap_get(_hashmap, sky[0].key));
  ASSERT_STREQ(sky[1].val, (char*)hashmap_get(_hashmap, sky[1].key));
  ASSERT_STREQ(sky[2].val, (char*)hashmap_get(_hashmap, sky[2].key));

  ASSERT_STREQ(sky[0].val, (char*)hashmap_remove(_hashmap, sky[0].key));
  ASSERT_STREQ(sky[1].val, (char*)hashmap_remove(_hashmap, sky[1].key));
  ASSERT_STREQ(sky[2].val, (char*)hashmap_remove(_hashmap, sky[2].key));

  ASSERT_EQ(0, hashmap_size(_hashmap));
}

TEST_F(HashmapTest, TestHashmapGetWithNonExistentKeys) {
  ASSERT(_hashmap);

  hashmap_put(_hashmap, fruits[0].key, fruits[0].val);
  hashmap_put(_hashmap, fruits[1].key, fruits[1].val);
  hashmap_put(_hashmap, fruits[2].key, fruits[2].val);

  ASSERT_TRUE(hashmap_contains_key(_hashmap, fruits[0].key));
  ASSERT_TRUE(hashmap_contains_key(_hashmap, fruits[1].key));
  ASSERT_TRUE(hashmap_contains_key(_hashmap, fruits[2].key));

  ASSERT_EQ(3, hashmap_size(_hashmap));
  ASSERT_EQ(8, hashmap_bucketcount(_hashmap));

  ASSERT_STREQ(NULL, (char*)hashmap_get(_hashmap, (char*)"x"));
  ASSERT_STREQ(NULL, (char*)hashmap_get(_hashmap, (char*)"y"));
  ASSERT_STREQ(NULL, (char*)hashmap_get(_hashmap, (char*)"Z"));

  ASSERT_EQ(3, hashmap_size(_hashmap));
}

TEST_F(HashmapTest, TestHashmapFree) {
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