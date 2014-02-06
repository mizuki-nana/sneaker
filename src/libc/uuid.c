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

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <time.h>
#include "../../include/libc/assert.h"
#include "../../include/libc/hash.h"
#include "../../include/libc/memory.h"
#include "../../include/libc/utils.h"
#include "../../include/libc/uuid.h"


/*
 * struct __sneaker_uuid_s
 * A 16-bytes structure
 */
struct __sneaker_uuid_s {
  unsigned long  time_u;  /* 4 bytes */
  unsigned long  time_l;  /* 4 bytes */
  unsigned int   time_h;  /* 4 bytes */
  unsigned short rand_1;  /* 2 bytes */
  unsigned short rand_2;  /* 2 bytes */
};


uuid_t uuid_create()
{
  uuid_t uuid = MALLOC(struct __sneaker_uuid_s);

  if(uuid == NULL) {
    errno = ENOMEM;
    return NULL;
  }

  time_t t;
  char buf[30];
  unsigned long long ll;

  unsigned long  u1=0;
  unsigned long  u2=0;
  unsigned int   u3=0;
  unsigned short u4=0;
  unsigned short u5=0;

  t = time(NULL);
  snprintf(buf, sizeof(buf), "%lld", (unsigned long long)t);
  ll = (unsigned long long)t;

  u1 = (ll & 0xFFFF0000) >> 16;
  u2 = ll & 0xFFFF;
  u3 = hash_str(buf);
  u4 = (unsigned short)rand_top(USHRT_MAX);
  u5 = (unsigned short)rand_top(USHRT_MAX);

  uuid->time_u = u1;
  uuid->time_l = u2;
  uuid->time_h = u3;
  uuid->rand_1 = u4;
  uuid->rand_2 = u5;

  return uuid;
}

int uuid_compare(uuid_t uuid1, uuid_t uuid2)
{
  RETURN_VAL_IF_NE(uuid1->time_u, uuid2->time_u, 0);
  RETURN_VAL_IF_NE(uuid1->time_l, uuid2->time_l, 0);
  RETURN_VAL_IF_NE(uuid1->time_h, uuid2->time_h, 0);
  RETURN_VAL_IF_NE(uuid1->rand_1, uuid2->rand_1, 0);
  RETURN_VAL_IF_NE(uuid1->rand_2, uuid2->rand_2, 0);

  return 1;
}

hash_t uuid_to_hash(const uuid_t uuid)
{
  hash_t hash = 0;

  hash_t time_u_hash = (hash_t)hash64shift(uuid->time_u);
  hash_t time_l_hash = (hash_t)hash64shift(uuid->time_l);
  hash_t time_h_hash = (hash_t)hash64shift(uuid->time_h);
  hash_t rand_1_hash = (hash_t)hash_robert_jenkin((unsigned int)uuid->rand_1);
  hash_t rand_2_hash = (hash_t)hash_robert_jenkin((unsigned int)uuid->rand_2);

#ifndef GOLDEN_PRIME
#define GOLDEN_PRIME 37
#endif

  hash = hash * GOLDEN_PRIME + time_u_hash;
  hash = hash * GOLDEN_PRIME + time_l_hash;
  hash = hash * GOLDEN_PRIME + time_h_hash;
  hash = hash * GOLDEN_PRIME + rand_1_hash;
  hash = hash * GOLDEN_PRIME + rand_2_hash;

  return hash;
}

hash_t uuid_create_and_hash()
{
  uuid_t uuid = uuid_create();

  return uuid_to_hash((const uuid_t)uuid);
}