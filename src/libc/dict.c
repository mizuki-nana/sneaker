/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2015 Yanzheng Li

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
#include "../../include/libc/dict.h"

#include "../../include/libc/c_str.h"
#include "../../include/libc/hash.h"
#include "../../include/libc/hashmap.h"
#include "../../include/libc/memory.h"
#include "../../include/libc/utils.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>


// -----------------------------------------------------------------------------

#define DICT_DEFAULT_CAPACITY 16

// -----------------------------------------------------------------------------

struct __sneaker_dict_s {
  hashmap_t hashmap;
};

// -----------------------------------------------------------------------------

static
inline int _dict_keycmpfunc(void* key1, void* key2)
{
  assert(key1);
  assert(key2);

  c_str _key1 = (c_str)key1;
  c_str _key2 = (c_str)key2;

  return strcmp(_key1, _key2) == 0;
}

// -----------------------------------------------------------------------------

static
inline unsigned long int _dict_hashfunc(void* key)
{
  return linear_horners_rule_str_hash((c_str)key);
}

// -----------------------------------------------------------------------------

dict_t dict_create()
{
  dict_t dict = MALLOC(struct __sneaker_dict_s);

  if (!dict) {
    errno = ENOMEM;
    return NULL;
  }

  hashmap_t hashmap = hashmap_create(
    DICT_DEFAULT_CAPACITY,
    _dict_hashfunc,
    _dict_keycmpfunc
  );

  if (!hashmap) {
    errno = ENOMEM;
    return NULL;
  }

  dict->hashmap = hashmap;

  return dict;
}

// -----------------------------------------------------------------------------

size_t dict_size(dict_t dict)
{
  assert(dict);
  return hashmap_size(dict->hashmap);
}

// -----------------------------------------------------------------------------

void dict_free(dict_t *dict)
{
  dict_t _dict = *dict;

  assert(_dict);

  hashmap_free(&_dict->hashmap);

  FREE(_dict);

  *dict = _dict;
}

// -----------------------------------------------------------------------------

void* dict_put(dict_t dict, const char *key, void* val)
{
  assert(dict);
  assert(key);
  assert(val);

  return hashmap_put(dict->hashmap, (c_str)key, val);
}

// -----------------------------------------------------------------------------

void* dict_get(dict_t dict, const char *key)
{
  assert(dict);
  assert(key);

  return hashmap_get(dict->hashmap, (c_str)key);
}

// -----------------------------------------------------------------------------
