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
#include <limits.h>
#include "../../include/libc/array.h"
#include "../../include/libc/assert.h"
#include "../../include/libc/memory.h"
#include "../../include/libc/utils.h"


#define DEFAULT_ARRAY_INITIAL_CAPACITY 4

#define ARRAY_MAX_CAPACITY ((int)UINT_MAX/sizeof(void*))


struct __sneaker_array_s {
  void** content;
  size_t size;
  size_t capacity;
};


/*
 * Expand the capacity of the array, if the specified
 * capacity is greater than capacity of the array.
 */
static int
_array_ensure_capacity(array_t array, size_t capacity)
{
  ASSERT(array);

  int oldCapacity = array->capacity;

  if(capacity > oldCapacity) {
    size_t newCapcity = (oldCapacity == 0) ?
      DEFAULT_ARRAY_INITIAL_CAPACITY : oldCapacity;

    while(newCapcity < capacity) {
      size_t newCap = newCapcity << 1;
      if(newCap < newCapcity || newCap > ARRAY_MAX_CAPACITY) {
        newCap = ARRAY_MAX_CAPACITY;
      }
      newCapcity = newCap;
    }

    void **newContent = NULL;
    if(!array->content) {
      newContent = malloc(newCapcity * sizeof(void*));
      if(!newContent) {
        errno = ENOMEM;
        return -1;
      }
    } else {
      newContent = realloc(array->content, sizeof(void*) * newCapcity);
      if(!newContent) {
        errno = ENOMEM;
        return -1;
      }
    }

    array->capacity = newCapcity;
    array->content = newContent;
  }

  return 1;
}

static
inline void _array_check_bound(array_t array, int index)
{
  ASSERT(array);
  ASSERT(index >= 0 && index < array->size);
}

array_t array_create()
{
  array_t array = NULL;

  array = MALLOC(struct __sneaker_array_s);

  if(!array) {
    errno = ENOMEM;
    return NULL;
  }

  array->content = NULL;
  array->size = 0;
  array->capacity = 0;

  _array_ensure_capacity(array, DEFAULT_ARRAY_INITIAL_CAPACITY);

  return array;
}

void array_free(array_t *array)
{
  array_t _array = *array;

  ASSERT(_array);
  FREE(_array->content);
  FREE(_array);

  *array = _array;
}

int
array_append(array_t array, void* ptr)
{
  ASSERT(array);

  size_t size = array->size;
  int res = _array_ensure_capacity(array, size+1);

  RETURN_VAL_IF_FALSE(res, 0);

  array->content[size] = ptr;
  array->size++;

  return 1;
}

void*
array_get(array_t array, int index)
{
  ASSERT(array);

  _array_check_bound(array, index);

  return array->content[index];
}

void*
array_remove(array_t array, int index)
{
  ASSERT(array);

  _array_check_bound(array, index);

  void *ptr = array->content[index];

  int newSize = array->size - 1;

  if(index != newSize) {
    memmove(
      array->content+index,
      array->content+index+1,
      (sizeof(void*)) * (newSize - index)
    );
  }

  array->size = newSize;

  return ptr;
}

void*
array_set(array_t array, int index, void* ptr)
{
  ASSERT(array);

  _array_check_bound(array, index);

  void* old = array->content[index];
  array->content[index] = ptr;

  return old;
}

int
array_size(array_t array)
{
  ASSERT(array);
  return array->size;
}

const void**
array_content(array_t array)
{
  ASSERT(array);
  return (const void**)array->content;
}