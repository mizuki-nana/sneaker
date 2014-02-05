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

#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include "memory.h"
#include "assert.h"
#include "utils.h"
#include "buffer.h"


struct HexBuffer_s {
  char *data;                   /* byte array */
  union {
    size_t expected;            /* for reader, # of bytes expected */
    size_t remaining;           /* for writing, # of bytes remaining */
  };
  size_t size;                  /* actual size of buffer */
  size_t capacity;              /* amount of memory allocated for the buffer */
};


Buffer
buffer_create(size_t capacity)
{
  Buffer buffer = NULL;

  buffer = MALLOC(struct HexBuffer_s);

  if(!buffer) {
    errno = ENOMEM;
    return NULL;
  }

  buffer->data = malloc(capacity * sizeof(char));
  buffer->size = 0;
  buffer->capacity = capacity;
  buffer->expected = 0;

  memset(buffer->data, 0, buffer->capacity);

  return buffer;
}

void
buffer_free(Buffer *buffer)
{
  Buffer _buffer = *buffer;

  ASSERT(_buffer);
  FREE(_buffer->data);
  FREE(_buffer);

  *buffer = _buffer;
}

char*
buffer_get(Buffer buffer)
{
  ASSERT(buffer);
  return buffer->data;
}

int
buffer_prepare_for_read(Buffer buffer, size_t expected)
{
  ASSERT(buffer);

  if(expected > buffer->expected) {
    char *expanded = realloc(buffer->data, expected);
    if(!expanded) {
      errno = ENOMEM;
      return -1;
    }
    buffer->capacity = expected;
    buffer->data = expanded;
  }

  buffer->expected = expected;

  return 1;
}

ssize_t
buffer_read(Buffer buffer, int fd)
{
  ASSERT(buffer);

  RETURN_VAL_IF_FALSE(buffer->size < buffer->expected, 0);

  ssize_t bytesRead = read(
    fd,
    buffer->data+buffer->size,
    buffer->expected-buffer->size
  );

  if(bytesRead > 0) {
    buffer->size += bytesRead;
    return buffer->size;
  }

  return bytesRead;
}

void
buffer_prepare_for_write(Buffer buffer)
{
  ASSERT(buffer);
  buffer->remaining = buffer->size;
}

ssize_t
buffer_write(Buffer buffer, int fd)
{
  ASSERT(buffer);

  RETURN_VAL_IF_FALSE(buffer->remaining > 0, 0);
  RETURN_VAL_IF_FALSE(buffer->remaining <= buffer->size, 0);

  ssize_t bytesWritten = write(
    fd,
    buffer->data+buffer->size-buffer->remaining,
    buffer->remaining
  );

  if(bytesWritten >= 0) {
    buffer->remaining -= bytesWritten;
    return buffer->remaining;
  }

  return bytesWritten;
}