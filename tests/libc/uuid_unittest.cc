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

/* Unit test for `uuid128_t` defined in include/libc/uuid.h */

#include <climits>
#include <set>
#include "../../include/testing/testing.h"
#include "../../include/libc/uuid.h"


class UUIDUnitTest : public ::testing::Test {};


TEST_F(UUIDUnitTest, TestUUIDCompareSameID)
{
  uuid128_t id = uuid_create();
  ASSERT_EQ(0, uuid_compare(id, id));
}

TEST_F(UUIDUnitTest, TestUUIDCompareDifferentID)
{
  uuid128_t uuid1 = uuid_create();
  uuid128_t uuid2 = uuid_create();

  ASSERT_NE(0, uuid_compare(uuid1, uuid2));
}

TEST_F(UUIDUnitTest, TestHashOnDifferentUUID_1)
{
  const uuid128_t uuid1 = uuid_create();
  const uuid128_t uuid2 = uuid_create();

  ASSERT_NE(0, uuid_compare(uuid1, uuid2));

  uint64_t hash1 = uuid_to_hash(uuid1);
  uint64_t hash2 = uuid_to_hash(uuid2);

  ASSERT_NE(hash1, hash2);
}

TEST_F(UUIDUnitTest, TestHashOnDifferentUUID_2)
{
  int i;
  for(i = 0; i < USHRT_MAX; i++) {
    const uuid128_t uuid1 = uuid_create();
    const uuid128_t uuid2 = uuid_create();

    ASSERT_NE(0, uuid_compare(uuid1, uuid2));

    ASSERT_NE(
      uuid_to_hash(uuid1),
      uuid_to_hash(uuid2)
    );
  }
}

TEST_F(UUIDUnitTest, TestUUIDCreateAndHash)
{
  int i;
  for(i = 0; i < USHRT_MAX; i++) {
    ASSERT_NE(
      uuid_create_and_hash(),
      uuid_create_and_hash()
    );
  }
}

TEST_F(UUIDUnitTest, TestUniqueness)
{
  std::set<uint64_t> hashes;

  int i;
  for(i = 0; i < 50000; i++) {
    uint64_t hash = uuid_create_and_hash();

    if(!hashes.empty()) {
      ASSERT_TRUE(hashes.end() == hashes.find(hash));
    }

    hashes.insert(hash);
  }
}