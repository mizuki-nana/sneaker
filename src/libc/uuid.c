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
#include "libc/uuid.h"

#include "libc/assert.h"
#include "libc/hash.h"
#include "libc/utils.h"

#include <assert.h>
#include <limits.h>
#include <string.h>
#include <sys/time.h>


// -----------------------------------------------------------------------------

uuid128_t uuid_create()
{
  uuid128_t uuid;

  unsigned int   u1 = 0;
  unsigned int   u2 = 0;
  unsigned int   u3 = 0;
  unsigned short u4 = 0;
  unsigned short u5 = 0;

  STATIC_ASSERT(
    sizeof(u1) + sizeof(u2) + sizeof(u3) + sizeof(u4) + sizeof(u5) == 16u);

  char offsets[] = {
    0,
    sizeof(u1),
    sizeof(u2),
    sizeof(u3),
    sizeof(u4)
  };

  struct timeval tv;
  gettimeofday(&tv, NULL);

  const unsigned long long ll = (unsigned long long)(tv.tv_usec);

  u1 = ll & 0xFFFF;
  u2 = (ll & 0xFFFF0000) >> 31;
  u3 = rand_top(INT_MAX);
  u4 = (unsigned short)rand_top(USHRT_MAX);
  u5 = (unsigned short)rand_top(USHRT_MAX);

  memcpy(uuid.data+offsets[0], &u1, sizeof(u1));
  memcpy(uuid.data+offsets[1], &u2, sizeof(u2));
  memcpy(uuid.data+offsets[2], &u3, sizeof(u3));
  memcpy(uuid.data+offsets[3], &u4, sizeof(u4));
  memcpy(uuid.data+offsets[4], &u5, sizeof(u5));

  return uuid;
}

// -----------------------------------------------------------------------------

int uuid_compare(uuid128_t uuid1, uuid128_t uuid2)
{
  return memcmp(uuid1.data, uuid2.data, sizeof(uuid1.data));
}

// -----------------------------------------------------------------------------

__uint128_t uuid_to_hash(const uuid128_t uuid)
{
  __uint128_t hash = 0;
  memcpy(&hash, uuid.data, sizeof(hash));
  return hash;
}

// -----------------------------------------------------------------------------

__uint128_t uuid_create_and_hash()
{
  uuid128_t uuid = uuid_create();
  return uuid_to_hash(uuid);
}

// -----------------------------------------------------------------------------
