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

/* Unit test for `dict_t` defined in sneaker/libc/dict.h */

#include "libc/dict.h"

#include "libc/c_str.h"
#include "libc/memory.h"
#include "testing/testing.h"

#include <cassert>
#include <unordered_map>


// -----------------------------------------------------------------------------

class dict_unittest : public ::testing::Test {
protected:
  virtual void SetUp() {
    m_dict = dict_create();
    assert(m_dict);
  }

  virtual void TearDown() {
    dict_free(&m_dict);
    assert(m_dict == NULL);
  }

  dict_t m_dict;
};

// -----------------------------------------------------------------------------

typedef struct {
  c_str key;
  c_str val;
} KeyVal;

// -----------------------------------------------------------------------------

namespace {

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

// -----------------------------------------------------------------------------

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

// -----------------------------------------------------------------------------

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

} /* end anonymous namespace */

// -----------------------------------------------------------------------------

TEST_F(dict_unittest, TestCreation)
{
  assert(m_dict);
  ASSERT_EQ(0, dict_size(m_dict));
}

// -----------------------------------------------------------------------------

TEST_F(dict_unittest, TestPut)
{
  dict_put(m_dict, fruits[0].key, fruits[0].val);
  ASSERT_EQ(1, dict_size(m_dict));

  dict_put(m_dict, fruits[1].key, fruits[1].val);
  ASSERT_EQ(2, dict_size(m_dict));

  dict_put(m_dict, fruits[2].key, fruits[2].val);
  ASSERT_EQ(3, dict_size(m_dict));

  c_str val1 = (c_str)dict_get(m_dict, fruits[0].key);
  c_str val2 = (c_str)dict_get(m_dict, fruits[1].key);
  c_str val3 = (c_str)dict_get(m_dict, fruits[2].key);

  assert(val1);
  assert(val2);
  assert(val3);

  ASSERT_STREQ(val1, fruits[0].val);
  ASSERT_STREQ(val2, fruits[1].val);
  ASSERT_STREQ(val3, fruits[2].val);
}

// -----------------------------------------------------------------------------

TEST_F(dict_unittest, TestGet)
{
  dict_put(m_dict, fruits[0].key, fruits[0].val);
  dict_put(m_dict, fruits[1].key, fruits[1].val);
  dict_put(m_dict, fruits[2].key, fruits[2].val);

  ASSERT_EQ(3, dict_size(m_dict));

  ASSERT_EQ(fruits[0].val, dict_get(m_dict, fruits[0].key));
  ASSERT_EQ(fruits[1].val, dict_get(m_dict, fruits[1].key));
  ASSERT_EQ(fruits[2].val, dict_get(m_dict, fruits[2].key));

  assert(dict_put(m_dict, sky[1].key, sky[1].val));

  ASSERT_EQ(3, dict_size(m_dict));

  ASSERT_EQ(fruits[0].val, dict_get(m_dict, fruits[0].key));
  ASSERT_EQ(sky[1].val,    dict_get(m_dict, sky[1].key));
  ASSERT_EQ(fruits[2].val, dict_get(m_dict, fruits[2].key));

  assert(dict_put(m_dict, vehicles[2].key, vehicles[2].val));

  ASSERT_EQ(3, dict_size(m_dict));

  ASSERT_EQ(fruits[0].val,   (c_str)dict_get(m_dict, fruits[0].key));
  ASSERT_EQ(sky[1].val,      (c_str)dict_get(m_dict, sky[1].key));
  ASSERT_EQ(vehicles[2].val, (c_str)dict_get(m_dict, vehicles[2].key));
}

// -----------------------------------------------------------------------------

TEST_F(dict_unittest, TestStress)
{
  const int TOP = 500000;
  std::unordered_map<int, char*> map;

  for (int i = 0; i < TOP; i++) {
    char buf[10];
    snprintf(buf, sizeof(buf), "%d", i);
    char* s = strdup(buf);
    assert(s);

    dict_put(m_dict, s, s);
    ASSERT_EQ(i + 1, dict_size(m_dict));

    map[i] = s;
  }

  for (int i = 0; i < TOP; i++) {
    char buf[10];
    snprintf(buf, sizeof(buf), "%d", i);

    ASSERT_STREQ(map[i], (char*)dict_get(m_dict, buf));
  }
}

// -----------------------------------------------------------------------------
