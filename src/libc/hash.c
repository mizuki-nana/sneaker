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

/*
 * Some methods of hashing inspired from the following sources:
 *
 * http://www.concentric.net/~ttwang/tech/inthash.htm
 * http://www.burtleburtle.net/bob/hash/doobs.html
 */

#include "../../include/libc/hash.h"

#include "../../include/libc/utils.h"

#include <stddef.h>
#include <string.h>


// -----------------------------------------------------------------------------

#define GOLDEN_PRIME 1237

// -----------------------------------------------------------------------------

unsigned long int linear_horners_rule_str_hash(const char * str)
{
  RETURN_VAL_IF_NULL(str, 0);

  size_t len = strlen(str);

  unsigned long int h = 0;

  int i;
  for (i = 0; i < len; i++) {
    h = GOLDEN_PRIME * h + str[i];
  }

  return h;
}

// -----------------------------------------------------------------------------

unsigned long int hash32shift(unsigned int key)
{
  key = (key << 15) - key - 1;
  key = key ^ (key >> 12);
  key = key + (key << 2);
  key = key ^ (key >> 4);
  key = (key + (key << 3)) + (key << 11);
  key = key ^ (key >> 16);

  return (unsigned long int)key;
}

// -----------------------------------------------------------------------------

unsigned long int hash64shift(unsigned long key)
{
  key = (key << 21) - key - 1;
  key = key ^ (key >> 24);
  key = (key + (key << 3)) + (key << 8);
  key = key ^ (key >> 14);
  key = (key + (key << 2)) + (key << 4);
  key = key ^ (key >> 28);
  key = key + (key << 31);

  return (unsigned long int)key;
}

// -----------------------------------------------------------------------------

unsigned long int hash_str_jenkins_one_at_a_time(const char * str)
{
  RETURN_VAL_IF_NULL(str, 0);

  unsigned long int hash=0;

  int i;
  for (i = 0; i < strlen(str); i++) {
    hash += str[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

// -----------------------------------------------------------------------------

unsigned long int hash_robert_jenkin(unsigned int k)
{
  k = (k + 0x7ed55d16) + (k << 12);
  k = (k ^ 0xc761c23c) ^ (k >> 19);
  k = (k + 0x165667b1) + (k << 5);
  k = (k + 0xd3a2646c) ^ (k << 9);
  k = (k + 0xfd7046c5) + (k << 3);
  k = (k ^ 0xb55a4f09) ^ (k >> 16);

  return (unsigned long int)k;
}

// -----------------------------------------------------------------------------
