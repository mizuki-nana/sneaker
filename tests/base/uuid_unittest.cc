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

/* Unit test for src/base/uuid.c */


#include <limits.h>
#include "../_unittest.h"
#include "../../src/base/assert.h"
#include "../../src/base/uuid.h"


/**********************************
 * Test for:
 * int uuid_create(uuid*)
 **********************************/
TEST(uuid_createTest, UUIDCreationTest) {
  uuid id;

  ASSERT_TRUE(uuid_create(&id));
}


/**********************************
 * Test for:
 * int uuid_compare(uuid, uuid)
 **********************************/
TEST(uuid_compareTest, UUIDCompareSameIDTest) {
  uuid id;
  ASSERT_TRUE(uuid_create(&id));
  ASSERT_TRUE(uuid_compare(id, id));
}

TEST(uuid_compareTest, UUIDCompareDifferentIDTest) {
  uuid id1;
  uuid id2;

  ASSERT_TRUE(uuid_create(&id1));
  ASSERT_TRUE(uuid_create(&id2));

  /* two UUIDs should NOT be the same!!! */
  ASSERT_FALSE(uuid_compare(id1, id2));
}

/****************************************
 * Test for:
 * hash_t uuid_to_hash(const uuid)
 ****************************************/
TEST(uuid_to_hashTest, HashOnDifferentUUIDTest1) {
  uuid uuid1;
  uuid uuid2;

  ASSERT_TRUE(uuid_create(&uuid1));
  ASSERT_TRUE(uuid_create(&uuid2));

  ASSERT_FALSE(uuid_compare(uuid1, uuid2));

  hash_t hash1 = uuid_to_hash((const uuid)uuid1);
  hash_t hash2 = uuid_to_hash((const uuid)uuid2);

  ASSERT_NE(hash1, hash2);
}

TEST(uuid_to_hashTest, HashOnDifferentUUID_Test) {
  int i;
  for(i = 0; i < 100000; i++) {
    uuid uuid1;
    uuid uuid2;

    ASSERT_TRUE(uuid_create(&uuid1));
    ASSERT_TRUE(uuid_create(&uuid2));

    ASSERT_FALSE(uuid_compare(uuid1, uuid2));

    ASSERT_NE(
      uuid_to_hash((const uuid)uuid1),
      uuid_to_hash((const uuid)uuid2)
    );
  }
}

TEST(uuid_create_and_hashTest, UUIDCreateAndHash_Test) {
  int i;
  for(i = 0; i < 100000; i++) {
    ASSERT_NE(
      uuid_create_and_hash(),
      uuid_create_and_hash()
    );
  }
}