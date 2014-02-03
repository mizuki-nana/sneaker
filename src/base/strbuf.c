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

#include <stddef.h>
#include "assert.h"
#include "c_str.h"
#include "memory.h"
#include "strbuf.h"
#include "utils.h"


#define DEFAULT_STRBUF_INITIAL_ALLOC 1024

static const size_t _strbuf_alloc_size = DEFAULT_STRBUF_INITIAL_ALLOC;


struct HexStrbuf_s {
  char*  c_str;               /* pointer to the raw string */
  size_t  capacity;           /* size of allocated data */
  size_t  size;               /* amount of space used */
};


static
void _strbuf_init(Strbuf *strbuf, size_t min_len)
{
  Strbuf _strbuf = *strbuf;

  RETURN_IF_NULL(_strbuf);

  c_str _str = NULL;

  size_t len = MAX(min_len, _strbuf_alloc_size);
  size_t capacity = len;

  _str = (c_str)malloc(len);

  if(!_str) {
    errno = ENOMEM;
    exit(5);
  }

  memset(_str, 0, len);

  _strbuf->c_str = _str;
  _strbuf->size = 0;
  _strbuf->capacity = capacity;

  *strbuf = _strbuf;
}

static
void _strbuf_allocate_more(Strbuf *strbuf, size_t len)
{
  Strbuf _strbuf = *strbuf;

  ASSERT(_strbuf);

  size_t new_capacity = _strbuf->capacity + len;
  c_str c_str = NULL;

  new_capacity = MAX(new_capacity, _strbuf_alloc_size);

  c_str = realloc(_strbuf->c_str, new_capacity);

  if(!c_str) {
    errno = ENOMEM;
    exit(-5);
  }

  _strbuf->c_str = c_str;
  _strbuf->capacity = new_capacity;

  *strbuf = _strbuf;
}

Strbuf strbuf_create()
{
  Strbuf strbuf = NULL;

  strbuf = MALLOC(struct HexStrbuf_s);

  if(!strbuf) {
    errno = ENOMEM;
    return NULL;
  }

  _strbuf_init(&strbuf, _strbuf_alloc_size);

  ASSERT(strbuf->c_str);

  return strbuf;
}

void
strbuf_free(Strbuf *strbuf)
{
  Strbuf _strbuf = *strbuf;

  ASSERT(_strbuf);

  FREE(_strbuf->c_str);
  _strbuf->size = 0;
  _strbuf->capacity = 0;

  FREE(_strbuf);

  *strbuf = _strbuf;
}

void
strbuf_empty(Strbuf strbuf)
{
  ASSERT(strbuf);

  FREE(strbuf->c_str);
  strbuf->size = 0;
  strbuf->capacity = 0;
  _strbuf_init(&strbuf, _strbuf_alloc_size);
}

size_t
strbuf_len(Strbuf strbuf)
{
  ASSERT(strbuf);
  return strbuf->size;
}

const c_str
strbuf_cstr(Strbuf strbuf)
{
  ASSERT(strbuf);
  return (const c_str)strbuf->c_str;
}

size_t
strbuf_capacity(Strbuf strbuf)
{
  ASSERT(strbuf);
  return strbuf->capacity;
}

int
strbuf_append(Strbuf strbuf, const c_str in_str)
{
  ASSERT(strbuf);
  ASSERT(in_str);

  size_t len = strlen(in_str);
  size_t needed = len + 1;

  if(strbuf->capacity == 0) { 
    _strbuf_init(&strbuf, MAX(len, _strbuf_alloc_size));
  }

  if(strbuf->size + len > strbuf->capacity) {
    _strbuf_allocate_more(&strbuf, needed);
  }

  c_str str = strcat(strbuf->c_str, in_str);
  strbuf->c_str = str;
  strbuf->size = strlen(strbuf->c_str);

  return 1;
}