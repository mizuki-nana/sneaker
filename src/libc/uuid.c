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

#include <limits.h>
#include <string.h>
#include <time.h>
#include "../../include/libc/hash.h"
#include "../../include/libc/utils.h"
#include "../../include/libc/uuid.h"


uuid128_t uuid_create()
{
  uuid128_t uuid;

  time_t t;
  uint64_t ll;

  unsigned long  u1 = 0; /* 4 bytes */
  unsigned long  u2 = 0; /* 4 bytes */
  unsigned int   u3 = 0; /* 4 bytes */
  unsigned short u4 = 0; /* 2 bytes */
  unsigned short u5 = 0; /* 2 bytes */

  char offsets[] = {0, sizeof(u1), sizeof(u2), sizeof(u3), sizeof(u4)};

  t = time(NULL);
  ll = (uint64_t)t;

  u1 = (ll & 0xFFFF0000) >> 16;
  u2 = ll & 0xFFFF;
  u3 = hash64shift(ll);
  u4 = (unsigned short)rand_top(USHRT_MAX);
  u5 = (unsigned short)rand_top(USHRT_MAX);

  memcpy(uuid.data+offsets[0], &u1, sizeof(u1));
  memcpy(uuid.data+offsets[1], &u2, sizeof(u2));
  memcpy(uuid.data+offsets[2], &u3, sizeof(u3));
  memcpy(uuid.data+offsets[3], &u4, sizeof(u4));
  memcpy(uuid.data+offsets[4], &u5, sizeof(u5));

  return uuid;
}

int uuid_compare(uuid128_t uuid1, uuid128_t uuid2)
{
  return memcmp(uuid1.data, uuid2.data, 16);
}

uint64_t uuid_to_hash(const uuid128_t uuid)
{
  const unsigned int GOLDEN_PRIME = 37;
  uint64_t upper = 0;
  uint64_t lower = 0;

  memcpy(&upper, uuid.data, 8);
  memcpy(&lower, uuid.data+8, 8);

  return hash64shift(upper) * GOLDEN_PRIME + hash64shift(lower);
}

uint64_t uuid_create_and_hash()
{
  uuid128_t uuid = uuid_create();

  return uuid_to_hash((const uuid128_t)uuid);
}