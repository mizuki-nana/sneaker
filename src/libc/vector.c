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

#include "../../include/libc/vector.h"

#include "../../include/libc/memory.h"
#include "../../include/libc/utils.h"

#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>


#define DEFAULT_VECTOR_INITIAL_CAPACITY 4

#define VECTOR_MAX_CAPACITY ((int)UINT_MAX/sizeof(void*))


struct __sneaker_vector_s {
  void** content;
  size_t size;
  size_t capacity;
};


/*
 * Expand the capacity of the vector, if the specified
 * capacity is greater than capacity of the vector.
 */
static int
_vector_ensure_capacity(vector_t vector, size_t capacity)
{
  assert(vector);

  int oldCapacity = vector->capacity;

  if(capacity > oldCapacity) {
    size_t newCapcity = (oldCapacity == 0) ?
      DEFAULT_VECTOR_INITIAL_CAPACITY : oldCapacity;

    while(newCapcity < capacity) {
      size_t newCap = newCapcity << 1;
      if(newCap < newCapcity || newCap > VECTOR_MAX_CAPACITY) {
        newCap = VECTOR_MAX_CAPACITY;
      }
      newCapcity = newCap;
    }

    void **newContent = NULL;
    if(!vector->content) {
      newContent = malloc(newCapcity * sizeof(void*));
      if(!newContent) {
        errno = ENOMEM;
        return -1;
      }
    } else {
      newContent = realloc(vector->content, sizeof(void*) * newCapcity);
      if(!newContent) {
        errno = ENOMEM;
        return -1;
      }
    }

    vector->capacity = newCapcity;
    vector->content = newContent;
  }

  return 1;
}

static
inline void _vector_check_bound(vector_t vector, int index)
{
  assert(vector);
  assert(index >= 0 && index < vector->size);
}

vector_t vector_create()
{
  vector_t vector = NULL;

  vector = MALLOC(struct __sneaker_vector_s);

  if(!vector) {
    errno = ENOMEM;
    return NULL;
  }

  vector->content = NULL;
  vector->size = 0;
  vector->capacity = 0;

  _vector_ensure_capacity(vector, DEFAULT_VECTOR_INITIAL_CAPACITY);

  return vector;
}

void vector_free(vector_t *vector)
{
  vector_t _vector = *vector;

  assert(_vector);
  FREE(_vector->content);
  FREE(_vector);

  *vector = _vector;
}

int
vector_append(vector_t vector, void* ptr)
{
  assert(vector);

  size_t size = vector->size;
  int res = _vector_ensure_capacity(vector, size+1);

  RETURN_VAL_IF_FALSE(res, 0);

  vector->content[size] = ptr;
  vector->size++;

  return 1;
}

void*
vector_get(vector_t vector, int index)
{
  assert(vector);

  _vector_check_bound(vector, index);

  return vector->content[index];
}

void*
vector_remove(vector_t vector, int index)
{
  assert(vector);

  _vector_check_bound(vector, index);

  void *ptr = vector->content[index];

  int newSize = vector->size - 1;

  if(index != newSize) {
    memmove(
      vector->content+index,
      vector->content+index+1,
      (sizeof(void*)) * (newSize - index)
    );
  }

  vector->size = newSize;

  return ptr;
}

void*
vector_set(vector_t vector, int index, void* ptr)
{
  assert(vector);

  _vector_check_bound(vector, index);

  void* old = vector->content[index];
  vector->content[index] = ptr;

  return old;
}

int
vector_size(vector_t vector)
{
  assert(vector);
  return vector->size;
}

const void**
vector_content(vector_t vector)
{
  assert(vector);
  return (const void**)vector->content;
}
