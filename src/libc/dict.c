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

#include <string.h>
#include "../../include/libc/assert.h"
#include "../../include/libc/dict.h"
#include "../../include/libc/hash.h"
#include "../../include/libc/hashmap.h"
#include "../../include/libc/memory.h"
#include "../../include/libc/utils.h"

#define DICT_DEFAULT_CAPACITY 16

struct HexDict {
  Hashmap hashmap;
};

static
inline int _dict_keycmpfunc(void* key1, void* key2)
{
  ASSERT(key1);
  ASSERT(key2);

  char *_key1 = (char*)key1;
  char *_key2 = (char*)key2;

  return strcmp(_key1, _key2) == 0;
}

static
inline hash_t _dict_hashfunc(void* key)
{
  return hash_str((char*)key);
}

Dict dict_create()
{
  Dict dict = MALLOC(struct HexDict);

  if(!dict) {
    errno = ENOMEM;
    return NULL;
  }

  Hashmap hashmap = hashmap_create(
    DICT_DEFAULT_CAPACITY,
    _dict_hashfunc,
    _dict_keycmpfunc
  );

  if(!hashmap) {
    errno = ENOMEM;
    return NULL;
  }

  dict->hashmap = hashmap;

  return dict;
}

size_t dict_size(Dict dict)
{
  ASSERT(dict);
  return hashmap_size(dict->hashmap);
}

void dict_free(Dict *dict)
{
  Dict _dict = *dict;

  ASSERT(_dict);

  hashmap_free(&_dict->hashmap);

  FREE(_dict);

  *dict = _dict;
}

void* dict_put(Dict dict, const char *key, void* val)
{
  ASSERT(dict);
  ASSERT(key);
  ASSERT(val);

  return hashmap_put(dict->hashmap, (char*)key, val);
}

void* dict_get(Dict dict, const char *key)
{
  ASSERT(dict);
  ASSERT(key);

  return hashmap_get(dict->hashmap, (char*)key);
}