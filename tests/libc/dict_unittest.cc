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

/* Unit test for src/libc/dict.c */


#include "../_unittest.h"
#include "../../src/libc/assert.h"
#include "../../src/libc/dict.h"
#include "../../src/libc/memory.h"

class DictTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    _dict = dict_create();
    ASSERT(_dict);
  }

  virtual void Teardown() {
    dict_free(&_dict);
    ASSERT(_dict == NULL);
  }

  Dict _dict;
};

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


TEST_F(DictTest, dict_createTest) {
  ASSERT(_dict);
  ASSERT_EQ(0, dict_size(_dict));
}

TEST_F(DictTest, dict_putTest1) {
  dict_put(_dict, fruits[0].key, fruits[0].val);
  ASSERT_EQ(1, dict_size(_dict));
  dict_put(_dict, fruits[1].key, fruits[1].val);
  ASSERT_EQ(2, dict_size(_dict));
  dict_put(_dict, fruits[2].key, fruits[2].val);
  ASSERT_EQ(3, dict_size(_dict));

  char *val1 = (char*)dict_get(_dict, fruits[0].key);
  char *val2 = (char*)dict_get(_dict, fruits[1].key);
  char *val3 = (char*)dict_get(_dict, fruits[2].key);

  ASSERT(val1);
  ASSERT(val2);
  ASSERT(val3);

  ASSERT_STREQ(val1, fruits[0].val);
  ASSERT_STREQ(val2, fruits[1].val);
  ASSERT_STREQ(val3, fruits[2].val);
}

TEST_F(DictTest, dict_getTest) {
  dict_put(_dict, fruits[0].key, fruits[0].val);
  dict_put(_dict, fruits[1].key, fruits[1].val);
  dict_put(_dict, fruits[2].key, fruits[2].val);

  ASSERT_EQ(3, dict_size(_dict));

  ASSERT_EQ(fruits[0].val, dict_get(_dict, fruits[0].key));
  ASSERT_EQ(fruits[1].val, dict_get(_dict, fruits[1].key));
  ASSERT_EQ(fruits[2].val, dict_get(_dict, fruits[2].key));

  ASSERT(dict_put(_dict, sky[1].key, sky[1].val));

  ASSERT_EQ(3, dict_size(_dict));

  ASSERT_EQ(fruits[0].val, dict_get(_dict, fruits[0].key));
  ASSERT_EQ(sky[1].val, dict_get(_dict, sky[1].key));
  ASSERT_EQ(fruits[2].val, dict_get(_dict, fruits[2].key));

  ASSERT(dict_put(_dict, vehicles[2].key, vehicles[2].val));

  ASSERT_EQ(3, dict_size(_dict));

  ASSERT_EQ(fruits[0].val, (char*)dict_get(_dict, fruits[0].key));
  ASSERT_EQ(sky[1].val, (char*)dict_get(_dict, sky[1].key));
  ASSERT_EQ(vehicles[2].val, (char*)dict_get(_dict, vehicles[2].key));
}